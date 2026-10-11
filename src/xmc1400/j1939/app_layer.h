#ifndef J1939_APP_LAYER_H
#define J1939_APP_LAYER_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool j1939_app_encode_diagnostic_message(uint8_t *payload, uint16_t spn, uint8_t fmi, uint8_t ocurrence_count, uint8_t lamp_status);
bool j1939_app_decode_diagnostic_message(const uint8_t *payload, uint16_t *spn, uint8_t *fmi, uint8_t *occurrence_count, uint8_t *lamp_status);

#ifdef __cplusplus
}
#endif

#endif
