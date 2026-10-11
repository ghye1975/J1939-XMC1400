#include "diagnostics.h"

#include <string.h>

void j1939_diag_init(j1939_diag_state_t *state)
{
    if (state == NULL) {
        return;
    }

    memset(state, 0, sizeof(*state));
}

bool j1939_diag_add_dtc(j1939_diag_state_t *state, uint16_t spn, uint8_t fmi, uint8_t occ, uint8_t lamp_status)
{
    if (state == NULL || state->count >= J1939_DIAGNOSTIC_MAX) {
        return false;
    }

    state->dtcs[state->count].spn = spn;
    state->dtcs[state->count].fmi = fmi;
    state->dtcs[state->count].ocurrence_count = occ;
    state->dtcs[state->count].lamp_status = lamp_status;
    state->count++;
    return true;
}

bool j1939_diag_clear_dtc(j1939_diag_state_t *state, uint16_t spn, uint8_t fmi)
{
    uint8_t i;

    if (state == NULL) {
        return false;
    }

    for (i = 0U; i < state->count; ++i) {
        if (state->dtcs[i].spn == spn && state->dtcs[i].fmi == fmi) {
            memmove(&state->dtcs[i], &state->dtcs[i + 1U],
                    (size_t)(state->count - i - 1U) * sizeof(j1939_dtc_t));
            state->count--;
            return true;
        }
    }

    return false;
}

uint8_t j1939_diag_count(const j1939_diag_state_t *state)
{
    if (state == NULL) {
        return 0U;
    }

    return state->count;
}
