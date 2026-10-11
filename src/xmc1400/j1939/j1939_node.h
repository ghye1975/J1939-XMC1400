#ifndef J1939_NODE_H
#define J1939_NODE_H

#include <stdbool.h>
#include <stdint.h>

#include "j1939_types.h"
#include "j1939_core.h"
#include "can_driver.h"
#include "transport.h"
#include "app_msg.h"
#include "diagnostics.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t local_sa;
    uint64_t ecu_name;
    uint32_t pgn_last_received;
    uint8_t request_count;
    j1939_tp_context_t tp;
    j1939_diag_state_t diag;
} j1939_node_t;

void j1939_node_init(j1939_node_t *node, uint8_t local_sa, uint64_t ecu_name);
void j1939_node_process_can_msg(j1939_node_t *node, const j1939_can_msg_t *msg);
void j1939_node_send_address_claim(j1939_node_t *node);
void j1939_node_send_engine_speed(j1939_node_t *node, uint16_t rpm);
void j1939_node_send_vehicle_speed(j1939_node_t *node, uint16_t speed_kmh);

#ifdef __cplusplus
}
#endif

#endif
