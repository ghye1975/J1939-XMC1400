#ifndef J1939_CAN_DRIVER_H
#define J1939_CAN_DRIVER_H

#include <stdbool.h>
#include <stdint.h>

#include "j1939_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define J1939_CAN_FILTER_MAX 8U

typedef void (*j1939_can_rx_cb_t)(const j1939_can_msg_t *msg);

typedef struct {
    uint32_t baudrate;
    bool use_29bit_id;
    uint8_t local_sa;
    uint32_t filter_ids[J1939_CAN_FILTER_MAX];
    uint32_t filter_masks[J1939_CAN_FILTER_MAX];
    uint8_t filter_count;
} j1939_can_driver_cfg_t;

int j1939_can_driver_init(const j1939_can_driver_cfg_t *cfg, j1939_can_rx_cb_t rx_cb);
void j1939_can_driver_deinit(void);
bool j1939_can_driver_send(const j1939_can_msg_t *msg);
bool j1939_can_driver_receive(j1939_can_msg_t *msg);
void j1939_can_driver_rx_isr(void);

#ifdef __cplusplus
}
#endif

#endif
