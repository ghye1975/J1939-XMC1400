#include "j1939_node.h"

#include <string.h>

static void j1939_node_send_pgn(j1939_node_t *node, uint32_t pgn, const uint8_t *payload, uint8_t payload_len)
{
    j1939_can_msg_t msg;
    j1939_can_id_t can_id;

    if (node == NULL || payload == NULL || payload_len == 0U) {
        return;
    }

    memset(&msg, 0, sizeof(msg));
    can_id.priority = J1939_PRIORITY_NORMAL;
    can_id.pdu_format = (uint8_t)((pgn >> 16) & 0xFFU);
    can_id.pdu_specific = 0xFFU;
    can_id.source_address = node->local_sa;
    msg.can_id = j1939_encode_can_id(&can_id);
    msg.dlc = payload_len;

    if (payload_len > J1939_MAX_FRAME_DATA) {
        payload_len = J1939_MAX_FRAME_DATA;
    }

    memcpy(msg.data, payload, payload_len);
    j1939_can_driver_send(&msg);
}

void j1939_node_init(j1939_node_t *node, uint8_t local_sa, uint64_t ecu_name)
{
    if (node == NULL) {
        return;
    }

    memset(node, 0, sizeof(*node));
    node->local_sa = local_sa;
    node->ecu_name = ecu_name;
    j1939_tp_init(&node->tp, local_sa, ecu_name);
    j1939_diag_init(&node->diag);
}

void j1939_node_send_address_claim(j1939_node_t *node)
{
    j1939_can_msg_t msg;
    j1939_can_id_t can_id;
    uint8_t payload[8] = {0};

    if (node == NULL) {
        return;
    }

    memset(&msg, 0, sizeof(msg));
    can_id.priority = J1939_PRIORITY_HIGH;
    can_id.pdu_format = (uint8_t)((J1939_PGN_ADDRESS_CLAIM >> 16) & 0xFFU);
    can_id.pdu_specific = 0xFFU;
    can_id.source_address = node->local_sa;

    payload[0] = (uint8_t)(node->ecu_name & 0xFFU);
    payload[1] = (uint8_t)((node->ecu_name >> 8) & 0xFFU);
    payload[2] = (uint8_t)((node->ecu_name >> 16) & 0xFFU);
    payload[3] = (uint8_t)((node->ecu_name >> 24) & 0xFFU);
    payload[4] = (uint8_t)((node->ecu_name >> 32) & 0xFFU);
    payload[5] = (uint8_t)((node->ecu_name >> 40) & 0xFFU);
    payload[6] = (uint8_t)((node->ecu_name >> 48) & 0xFFU);
    payload[7] = (uint8_t)((node->ecu_name >> 56) & 0xFFU);

    msg.can_id = j1939_encode_can_id(&can_id);
    msg.dlc = 8U;
    memcpy(msg.data, payload, 8U);
    j1939_can_driver_send(&msg);
}

void j1939_node_send_engine_speed(j1939_node_t *node, uint16_t rpm)
{
    uint8_t payload[2];

    if (node == NULL) {
        return;
    }

    if (j1939_app_encode_engine_speed(payload, rpm)) {
        j1939_node_send_pgn(node, J1939_PGN_ENGINE_SPEED, payload, 2U);
    }
}

void j1939_node_send_vehicle_speed(j1939_node_t *node, uint16_t speed_kmh)
{
    uint8_t payload[2];

    if (node == NULL) {
        return;
    }

    if (j1939_app_encode_vehicle_speed(payload, speed_kmh)) {
        j1939_node_send_pgn(node, J1939_PGN_VEHICLE_SPEED, payload, 2U);
    }
}

void j1939_node_process_can_msg(j1939_node_t *node, const j1939_can_msg_t *msg)
{
    uint32_t pgn;
    j1939_can_id_t id_info;

    if (node == NULL || msg == NULL) {
        return;
    }

    pgn = j1939_extract_pgn(msg->can_id);
    node->pgn_last_received = pgn;

    j1939_decode_can_id(msg->can_id, &id_info);

    if (pgn == J1939_PGN_REQUEST) {
        node->request_count++;
        return;
    }

    if (pgn == J1939_PGN_ADDRESS_CLAIM) {
        return;
    }

    switch (pgn) {
        case J1939_PGN_ENGINE_SPEED:
            {
                uint16_t rpm = 0U;
                if (j1939_app_decode_engine_speed(msg->data, &rpm)) {
                    (void)rpm;
                }
            }
            break;

        case J1939_PGN_VEHICLE_SPEED:
            {
                uint16_t speed = 0U;
                if (j1939_app_decode_vehicle_speed(msg->data, &speed)) {
                    (void)speed;
                }
            }
            break;

        default:
            break;
    }
}
