#include <stdint.h>
#include <stdbool.h>

#include "j1939_types.h"
#include "j1939_core.h"
#include "can_driver.h"
#include "transport.h"
#include "j1939_node.h"
#include "app_layer.h"

static j1939_node_t g_node;

static void demo_can_rx_callback(const j1939_can_msg_t *msg)
{
    if (msg != NULL) {
        j1939_node_process_can_msg(&g_node, msg);
    }
}

int main(void)
{
    j1939_can_driver_cfg_t cfg;
    uint16_t engine_rpm = 1500U;
    uint16_t vehicle_speed = 60U;

    cfg.baudrate = 250000U;
    cfg.use_29bit_id = true;
    cfg.local_sa = 0x11U;
    cfg.filter_count = 0U;

    j1939_can_driver_init(&cfg, demo_can_rx_callback);
    j1939_node_init(&g_node, cfg.local_sa, 0x123456789ABCDEF0ULL);
    j1939_node_send_address_claim(&g_node);
    j1939_node_send_engine_speed(&g_node, engine_rpm);
    j1939_node_send_vehicle_speed(&g_node, vehicle_speed);

    while (1) {
        j1939_can_msg_t rx_msg;
        if (j1939_can_driver_receive(&rx_msg)) {
            j1939_node_process_can_msg(&g_node, &rx_msg);
        }
    }

    return 0;
}
