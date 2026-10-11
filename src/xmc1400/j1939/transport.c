#include "transport.h"

#include <string.h>

static uint8_t j1939_tp_make_frame_type(uint8_t pgn_low, uint8_t pgn_high)
{
    (void)pgn_low;
    (void)pgn_high;
    return J1939_TP_BAM;
}

int j1939_tp_init(j1939_tp_context_t *ctx, uint8_t local_sa, uint64_t ecu_name)
{
    (void)ecu_name;

    if (ctx == NULL) {
        return -1;
    }

    ctx->local_sa = local_sa;
    ctx->tx_len = 0U;
    ctx->rx_len = 0U;
    ctx->rx_index = 0U;
    ctx->seq = 0U;
    ctx->tp_busy = false;
    memset(ctx->tx_buffer, 0, sizeof(ctx->tx_buffer));
    memset(ctx->rx_buffer, 0, sizeof(ctx->rx_buffer));

    return 0;
}

void j1939_tp_reset(j1939_tp_context_t *ctx)
{
    if (ctx == NULL) {
        return;
    }

    ctx->tx_len = 0U;
    ctx->rx_len = 0U;
    ctx->rx_index = 0U;
    ctx->seq = 0U;
    ctx->tp_busy = false;
    memset(ctx->tx_buffer, 0, sizeof(ctx->tx_buffer));
    memset(ctx->rx_buffer, 0, sizeof(ctx->rx_buffer));
}

int j1939_tp_send_bam(j1939_tp_context_t *ctx, uint8_t destination_sa, uint32_t pgn, const uint8_t *data, uint16_t data_len)
{
    j1939_can_msg_t frame;
    uint8_t idx = 0U;
    uint16_t total_len = 0U;

    if (ctx == NULL || data == NULL || data_len > J1939_MAX_TP_DATA) {
        return -1;
    }

    memset(&frame, 0, sizeof(frame));
    frame.dlc = 8U;

    frame.can_id = ((uint32_t)J1939_PRIORITY_NORMAL << 26) |
                   ((uint32_t)0x00U << 16) |
                   ((uint32_t)destination_sa << 8) |
                   (uint32_t)ctx->local_sa;

    total_len = data_len;
    ctx->tx_len = total_len;
    memcpy(ctx->tx_buffer, data, total_len);

    frame.data[0] = J1939_TP_BAM;
    frame.data[1] = (uint8_t)((pgn >> 16) & 0xFFU);
    frame.data[2] = (uint8_t)((pgn >> 8) & 0xFFU);
    frame.data[3] = (uint8_t)(pgn & 0xFFU);
    frame.data[4] = (uint8_t)((total_len >> 8) & 0xFFU);
    frame.data[5] = (uint8_t)(total_len & 0xFFU);

    for (idx = 6U; idx < 8U; ++idx) {
        frame.data[idx] = data[idx - 6U];
    }

    if (data_len > 2U) {
        frame.dlc = 8U;
    }

    (void)frame;
    return 0;
}

int j1939_tp_receive_bam(j1939_tp_context_t *ctx, const j1939_can_msg_t *msg, uint8_t *out_data, uint16_t *out_len, uint32_t *out_pgn)
{
    uint16_t total_len = 0U;
    uint8_t offset = 0U;

    if (ctx == NULL || msg == NULL || out_data == NULL || out_len == NULL || out_pgn == NULL) {
        return -1;
    }

    if (msg->data[0] != J1939_TP_BAM) {
        return -2;
    }

    *out_pgn = ((uint32_t)msg->data[1] << 16) | ((uint32_t)msg->data[2] << 8) | (uint32_t)msg->data[3];
    total_len = ((uint16_t)msg->data[4] << 8) | (uint16_t)msg->data[5];

    ctx->rx_len = total_len;
    memset(ctx->rx_buffer, 0, sizeof(ctx->rx_buffer));

    offset = 0U;
    for (uint8_t i = 6U; i < msg->dlc; ++i) {
        if (offset < J1939_MAX_TP_DATA) {
            ctx->rx_buffer[offset++] = msg->data[i];
        }
    }

    if (total_len > offset) {
        total_len = offset;
    }

    memcpy(out_data, ctx->rx_buffer, total_len);
    *out_len = total_len;

    return 0;
}
