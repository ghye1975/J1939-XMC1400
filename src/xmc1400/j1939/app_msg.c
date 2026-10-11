#include "app_msg.h"

bool j1939_app_encode_engine_speed(uint8_t *payload, uint16_t rpm)
{
    if (payload == NULL) {
        return false;
    }

    payload[0] = (uint8_t)((rpm >> 8) & 0xFFU);
    payload[1] = (uint8_t)(rpm & 0xFFU);
    return true;
}

bool j1939_app_decode_engine_speed(const uint8_t *payload, uint16_t *rpm)
{
    if (payload == NULL || rpm == NULL) {
        return false;
    }

    *rpm = (uint16_t)(((uint16_t)payload[0] << 8) | (uint16_t)payload[1]);
    return true;
}

bool j1939_app_encode_vehicle_speed(uint8_t *payload, uint16_t speed_kmh)
{
    if (payload == NULL) {
        return false;
    }

    payload[0] = (uint8_t)((speed_kmh >> 8) & 0xFFU);
    payload[1] = (uint8_t)(speed_kmh & 0xFFU);
    return true;
}

bool j1939_app_decode_vehicle_speed(const uint8_t *payload, uint16_t *speed_kmh)
{
    if (payload == NULL || speed_kmh == NULL) {
        return false;
    }

    *speed_kmh = (uint16_t)(((uint16_t)payload[0] << 8) | (uint16_t)payload[1]);
    return true;
}

bool j1939_app_encode_coolant_temp(uint8_t *payload, int16_t temp_c)
{
    uint16_t scaled;

    if (payload == NULL) {
        return false;
    }

    scaled = (uint16_t)(temp_c + 40);
    payload[0] = (uint8_t)((scaled >> 8) & 0xFFU);
    payload[1] = (uint8_t)(scaled & 0xFFU);
    return true;
}

bool j1939_app_decode_coolant_temp(const uint8_t *payload, int16_t *temp_c)
{
    uint16_t scaled;

    if (payload == NULL || temp_c == NULL) {
        return false;
    }

    scaled = (uint16_t)(((uint16_t)payload[0] << 8) | (uint16_t)payload[1]);
    *temp_c = (int16_t)scaled - 40;
    return true;
}
