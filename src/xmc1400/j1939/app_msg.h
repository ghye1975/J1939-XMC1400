#ifndef J1939_APP_MSG_H
#define J1939_APP_MSG_H

#include <stdbool.h>
#include <stdint.h>

#include "j1939_types.h"

#ifdef __cplusplus
extern "C" {
#endif

bool j1939_app_encode_engine_speed(uint8_t *payload, uint16_t rpm);
bool j1939_app_decode_engine_speed(const uint8_t *payload, uint16_t *rpm);
bool j1939_app_encode_vehicle_speed(uint8_t *payload, uint16_t speed_kmh);
bool j1939_app_decode_vehicle_speed(const uint8_t *payload, uint16_t *speed_kmh);
bool j1939_app_encode_coolant_temp(uint8_t *payload, int16_t temp_c);
bool j1939_app_decode_coolant_temp(const uint8_t *payload, int16_t *temp_c);

#ifdef __cplusplus
}
#endif

#endif
