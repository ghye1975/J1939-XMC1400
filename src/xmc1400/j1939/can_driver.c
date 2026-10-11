#include "can_driver.h"

#include <string.h>

#define J1939_CAN_RX_QUEUE_LEN 32U

static j1939_can_rx_cb_t g_rx_cb = NULL;
static j1939_can_msg_t g_rx_queue[J1939_CAN_RX_QUEUE_LEN];
static volatile uint8_t g_rx_head = 0U;
static volatile uint8_t g_rx_tail = 0U;
static bool g_initialized = false;

static bool j1939_can_queue_push(const j1939_can_msg_t *msg)
{
    uint8_t next_head;

    if (msg == NULL) {
        return false;
    }

    next_head = (uint8_t)((g_rx_head + 1U) % J1939_CAN_RX_QUEUE_LEN);
    if (next_head == g_rx_tail) {
        return false;
    }

    g_rx_queue[g_rx_head] = *msg;
    g_rx_head = next_head;
    return true;
}

static bool j1939_can_queue_pop(j1939_can_msg_t *msg)
{
    if (g_rx_head == g_rx_tail) {
        return false;
    }

    if (msg != NULL) {
        *msg = g_rx_queue[g_rx_tail];
    }

    g_rx_tail = (uint8_t)((g_rx_tail + 1U) % J1939_CAN_RX_QUEUE_LEN);
    return true;
}

int j1939_can_driver_init(const j1939_can_driver_cfg_t *cfg, j1939_can_rx_cb_t rx_cb)
{
    (void)cfg;

    g_rx_cb = rx_cb;
    g_rx_head = 0U;
    g_rx_tail = 0U;
    memset(g_rx_queue, 0, sizeof(g_rx_queue));
    g_initialized = true;

    return 0;
}

void j1939_can_driver_deinit(void)
{
    g_initialized = false;
    g_rx_cb = NULL;
    g_rx_head = 0U;
    g_rx_tail = 0U;
}

bool j1939_can_driver_send(const j1939_can_msg_t *msg)
{
    (void)msg;

    if (!g_initialized || msg == NULL) {
        return false;
    }

    return true;
}

bool j1939_can_driver_receive(j1939_can_msg_t *msg)
{
    if (!g_initialized || msg == NULL) {
        return false;
    }

    return j1939_can_queue_pop(msg);
}

void j1939_can_driver_rx_isr(void)
{
    j1939_can_msg_t msg;

    if (!g_initialized) {
        return;
    }

    memset(&msg, 0, sizeof(msg));
    msg.can_id = 0x18F00411U;
    msg.dlc = 8U;
    msg.data[0] = 0x00U;
    msg.data[1] = 0x00U;
    msg.data[2] = 0x00U;
    msg.data[3] = 0x00U;
    msg.data[4] = 0x00U;
    msg.data[5] = 0x00U;
    msg.data[6] = 0x00U;
    msg.data[7] = 0x00U;

    if (j1939_can_queue_push(&msg)) {
        if (g_rx_cb != NULL) {
            g_rx_cb(&msg);
        }
    }
}
