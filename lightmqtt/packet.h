#ifndef _LIGHTMQTT_PACKET_H_
#define _LIGHTMQTT_PACKET_H_

#include <lightmqtt/core.h>

#define LMQTT_ENCODE_FINISHED 0
#define LMQTT_ENCODE_AGAIN 1
#define LMQTT_ENCODE_ERROR 2

#define LMQTT_DECODE_FINISHED 0
#define LMQTT_DECODE_AGAIN 1
#define LMQTT_DECODE_ERROR 2

#define LMQTT_ERR_FINISHED 0
#define LMQTT_ERR_AGAIN 1

#define LMQTT_FIXED_HEADER_MAX_SIZE 5
#define LMQTT_CONNECT_HEADER_SIZE 10
#define LMQTT_CONNACK_HEADER_SIZE 2

#define LMQTT_MAX(a, b) ((a) >= (b) ? (a) : (b))

typedef struct _lmqtt_string_t {
    int len;
    char* buf;
} lmqtt_string_t;

typedef struct _lmqtt_fixed_header_t {
    int type;
    int dup;
    int qos;
    int retain;
    int remaining_length;
    struct {
        int bytes_read;
        int failed;
        int remain_len_multiplier;
        int remain_len_accumulator;
        int remain_len_finished;
    } internal;
} lmqtt_fixed_header_t;

typedef struct _lmqtt_connect_t {
    u16 keep_alive;
    int clean_session;
    int qos;
    int will_retain;
    lmqtt_string_t client_id;
    lmqtt_string_t will_topic;
    lmqtt_string_t will_message;
    lmqtt_string_t user_name;
    lmqtt_string_t password;
    struct {
        int buf_len;
        u8 buf[LMQTT_MAX(LMQTT_FIXED_HEADER_MAX_SIZE, LMQTT_CONNECT_HEADER_SIZE)];
    } internal;
} lmqtt_connect_t;

typedef struct _lmqtt_connack_t {
    int session_present;
    int return_code;
    struct {
        int bytes_read;
        int failed;
    } internal;
} lmqtt_connack_t;

typedef int (*lmqtt_encode_t)(void *data, int offset, u8 *buf, int buf_len,
    int *bytes_written);

typedef struct _lmqtt_tx_buffer_state_t {
    lmqtt_encode_t *recipe;
    void *data;
    struct {
        int recipe_pos;
        int recipe_offset;
    } internal;
} lmqtt_tx_buffer_state_t;

typedef struct _lmqtt_callbacks_t {
    int (*on_connack)(void *data, lmqtt_connack_t *connack);
    int (*on_pingresp)(void *data);
} lmqtt_callbacks_t;

typedef struct _lmqtt_rx_buffer_state_t {
    lmqtt_callbacks_t *callbacks;
    void *callbacks_data;

    struct {
        lmqtt_fixed_header_t header;
        int header_finished;
        union {
            lmqtt_connack_t connack;
        } payload;

        int failed;
    } internal;
} lmqtt_rx_buffer_state_t;

int encode_tx_buffer(lmqtt_tx_buffer_state_t *state, u8 *buf, int buf_len,
    int *bytes_written);

int decode_rx_buffer(lmqtt_rx_buffer_state_t *state, u8 *buf, int buf_len,
    int *bytes_read);

#endif
