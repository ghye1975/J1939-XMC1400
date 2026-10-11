#ifndef J1939_TRANSPORT_H
#define J1939_TRANSPORT_H

#include <stdbool.h>
#include <stdint.h>

#include "j1939_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define J1939_TP_BAM 0x10U
#define J1939_TP_CONN_MANAGE 0x11U
#define J1939_TP_RTS 0x13U
#define J1939_TP_CTS 0x14U
#define J1939_TP_EOM 0x14U
#define J1939_TP_ACK 0x15U
#define J1939_TP_ABORT 0xFFU

int j1939_tp_init(j1939_tp_context_t *ctx, uint8_t local_sa, uint64_t ecu_name);
int j1939_tp_send_bam(j1939_tp_context_t *ctx, uint8_t destination_sa, uint32_t pgn, const uint8_t *data, uint16_t data_len);
int j1939_tp_receive_bam(j1939_tp_context_t *ctx, const j1939_can_msg_t *msg, uint8_t *out_data, uint16_t *out_len, uint32_t *out_pgn);
void j1939_tp_reset(j1939_tp_context_t *ctx);

#ifdef __cplusplus
}
#endif

#endif
