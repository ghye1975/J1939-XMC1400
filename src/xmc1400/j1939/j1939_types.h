#ifndef J1939_TYPES_H
#define J1939_TYPES_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define J1939_MAX_FRAME_DATA 8U
#define J1939_MAX_TP_DATA 1785U
#define J1939_TOTAL_MSG_BYTES 1785U

#define J1939_PGN_REQUEST 0x00EA00U
#define J1939_PGN_ADDRESS_CLAIM 0x00EE00U
#define J1939_PGN_ENGINE_SPEED 0x00F003U
#define J1939_PGN_ENGINE_TORQUE 0x00F004U
#define J1939_PGN_VEHICLE_SPEED 0x00F002U
#define J1939_PGN_COOLANT_TEMP 0x00F000U
#define J1939_PGN_DIAGNOSTIC_MSG 0x00FEEFU

#define J1939_PRIORITY_LOW 7U
#define J1939_PRIORITY_NORMAL 6U
#define J1939_PRIORITY_HIGH 3U

#define J1939_PDU_FORMAT_PDU1 0x00U
#define J1939_PDU_FORMAT_PDU2 0xF0U

typedef struct {
    uint8_t priority;
    uint8_t pdu_format;
    uint8_t pdu_specific;
    uint8_t source_address;
} j1939_can_id_t;

typedef struct {
    uint32_t can_id;
    uint8_t dlc;
    uint8_t data[J1939_MAX_FRAME_DATA];
} j1939_can_msg_t;

typedef struct {
    uint8_t sa;
    uint8_t da;
    uint32_t pgn;
    uint8_t *payload;
    uint16_t payload_length;
} j1939_app_message_t;

typedef struct {
    uint8_t local_sa;
    uint64_t ecu_name;
    uint8_t tx_buffer[J1939_MAX_TP_DATA];
    uint8_t rx_buffer[J1939_MAX_TP_DATA];
    uint16_t tx_len;
    uint16_t rx_len;
    uint16_t rx_index;
    uint8_t seq;
    bool tp_busy;
} j1939_tp_context_t;

typedef struct {
    uint16_t spn;
    uint8_t fmi;
    uint8_t ocurrence_count;
    uint8_t lamp_status;
} j1939_dtc_t;

#ifdef __cplusplus
}
#endif

#endif
