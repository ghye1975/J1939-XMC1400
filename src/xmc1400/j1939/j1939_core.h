#ifndef J1939_CORE_H
#define J1939_CORE_H

#include <stdbool.h>
#include <stdint.h>

#include "j1939_types.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t j1939_priority_from_can_id(uint32_t can_id);
uint8_t j1939_pdu_format_from_can_id(uint32_t can_id);
uint8_t j1939_pdu_specific_from_can_id(uint32_t can_id);
uint8_t j1939_source_address_from_can_id(uint32_t can_id);
uint32_t j1939_extract_pgn(uint32_t can_id);
uint32_t j1939_encode_can_id(const j1939_can_id_t *id_info);
void j1939_decode_can_id(uint32_t can_id, j1939_can_id_t *id_info);
bool j1939_is_pdu1(uint32_t can_id);
bool j1939_is_pdu2(uint32_t can_id);
bool j1939_should_accept_frame(uint32_t can_id, uint8_t local_sa);

#ifdef __cplusplus
}
#endif

#endif
