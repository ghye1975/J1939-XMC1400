#include "app_layer.h"

bool j1939_app_encode_diagnostic_message(uint8_t *payload, uint16_t spn, uint8_t fmi, uint8_t ocurrence_count, uint8_t lamp_status)
{
    if (payload == NULL) {
        return false;
    }

    payload[0] = (uint8_t)((spn >> 8) & 0xFFU);
    payload[1] = (uint8_t)(spn & 0xFFU);
    payload[2] = fmi;
    payload[3] = ocurrence_count;
    payload[4] = lamp_status;
    return true;
}

bool j1939_app_decode_diagnostic_message(const uint8_t *payload, uint16_t *spn, uint8_t *fmi, uint8_t *occurrence_count, uint8_t *lamp_status)
{
    if (payload == NULL || spn == NULL || fmi == NULL || occurrence_count == NULL || lamp_status == NULL) {
        return false;
    }

    *spn = (uint16_t)(((uint16_t)payload[0] << 8) | (uint16_t)payload[1]);
    *fmi = payload[2];
    *occurrence_count = payload[3];
    *lamp_status = payload[4];
    return true;
}
