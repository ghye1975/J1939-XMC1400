#include "j1939_core.h"

uint8_t j1939_priority_from_can_id(uint32_t can_id)
{
    return (uint8_t)((can_id >> 26) & 0x07U);
}

uint8_t j1939_pdu_format_from_can_id(uint32_t can_id)
{
    return (uint8_t)((can_id >> 16) & 0xFFU);
}

uint8_t j1939_pdu_specific_from_can_id(uint32_t can_id)
{
    return (uint8_t)((can_id >> 8) & 0xFFU);
}

uint8_t j1939_source_address_from_can_id(uint32_t can_id)
{
    return (uint8_t)(can_id & 0xFFU);
}

uint32_t j1939_extract_pgn(uint32_t can_id)
{
    return (uint32_t)((can_id >> 8) & 0x03FFFFU);
}

uint32_t j1939_encode_can_id(const j1939_can_id_t *id_info)
{
    uint32_t can_id = 0U;

    if (id_info == NULL) {
        return 0U;
    }

    can_id |= ((uint32_t)id_info->priority & 0x07U) << 26;
    can_id |= ((uint32_t)id_info->pdu_format & 0xFFU) << 16;
    can_id |= ((uint32_t)id_info->pdu_specific & 0xFFU) << 8;
    can_id |= (uint32_t)id_info->source_address & 0xFFU;

    return can_id;
}

void j1939_decode_can_id(uint32_t can_id, j1939_can_id_t *id_info)
{
    if (id_info == NULL) {
        return;
    }

    id_info->priority = (uint8_t)((can_id >> 26) & 0x07U);
    id_info->pdu_format = (uint8_t)((can_id >> 16) & 0xFFU);
    id_info->pdu_specific = (uint8_t)((can_id >> 8) & 0xFFU);
    id_info->source_address = (uint8_t)(can_id & 0xFFU);
}

bool j1939_is_pdu1(uint32_t can_id)
{
    uint8_t pdu_format = j1939_pdu_format_from_can_id(can_id);
    return (pdu_format < 0xF0U);
}

bool j1939_is_pdu2(uint32_t can_id)
{
    uint8_t pdu_format = j1939_pdu_format_from_can_id(can_id);
    return (pdu_format >= 0xF0U);
}

bool j1939_should_accept_frame(uint32_t can_id, uint8_t local_sa)
{
    j1939_can_id_t id_info;
    uint32_t pgn;

    j1939_decode_can_id(can_id, &id_info);
    pgn = j1939_extract_pgn(can_id);

    if (j1939_is_pdu2(can_id)) {
        return true;
    }

    if (id_info.pdu_specific == local_sa) {
        return true;
    }

    if (pgn == J1939_PGN_REQUEST || pgn == J1939_PGN_ADDRESS_CLAIM) {
        return true;
    }

    return false;
}
