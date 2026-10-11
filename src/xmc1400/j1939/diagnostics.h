#ifndef J1939_DIAGNOSTICS_H
#define J1939_DIAGNOSTICS_H

#include <stdbool.h>
#include <stdint.h>

#include "j1939_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define J1939_DIAGNOSTIC_MAX 16U

typedef struct {
    uint8_t count;
    j1939_dtc_t dtcs[J1939_DIAGNOSTIC_MAX];
} j1939_diag_state_t;

void j1939_diag_init(j1939_diag_state_t *state);
bool j1939_diag_add_dtc(j1939_diag_state_t *state, uint16_t spn, uint8_t fmi, uint8_t occ, uint8_t lamp_status);
bool j1939_diag_clear_dtc(j1939_diag_state_t *state, uint16_t spn, uint8_t fmi);
uint8_t j1939_diag_count(const j1939_diag_state_t *state);

#ifdef __cplusplus
}
#endif

#endif
