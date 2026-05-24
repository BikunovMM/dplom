#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <libuv/uv.h>
#include <llhttp/llhttp.h>

#include "utils.h"

/* config_data */
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 2006
#define HOST "localhost"
#define CONFIG_PATH "../configs/config.txt"
#define IP_MAX_LEN 16
#define PORT_MAX_LEN 7
#define MAX_REG_REQ_LEN 468
#define CREATED_AT_STR_LEN 23

/* buffer_sizes */
#define ADD_TO_THE_HISTORY_BUF_LEN 145000

/* urls_targets */
#define REG_USER_URL "/register"
#define LOGIN_USER_URL "/login"
#define VALIDATE_TOKEN_URL "/validate_token"
#define GET_HISTORY_URL "/get_history"
#define GET_BANNER_URL "/get_banner"
#define ADD_TO_THE_HISTORY_URL "/add_to_the_history"

/* http_methods */
#define HTTPGET "GET"
#define HTTPPOST "POST"

/* http_request_header_format_str */
#define HTTP_REQ_HEAD                                  \
    "%s %s HTTP/1.1\r\n"                               \
    "Host: " HOST "\r\n"                               \
    "Date: %3s, %02d %3s %04d %02d:%02d:%02d GMT\r\n" \
    "Content-Type: text/plain; charset=utf-8\r\n"      \
    "Content-Length: %ld\r\n"                          \
    "\r\n"

/* http_body_format_str */
#define REG_USER_STR \
    "%s\n"           \
    "%s\n"           \
    "%s"

#define LOGIN_USER_STR \
    "%s\n"           \
    "%s"

#define INSERT_HISTORY_STR \
    "%s\n"           \
    "%s\n"           \
    "%s"

#define VALIDATE_TOKEN_STR "%s"

#define ADD_TO_THE_HISTORY_STR \
    "%s\n"                     \
    "%s\n"                     \
    "%s"

/* responses_values */
#define VALID_TOKEN   "1"
#define UNVALID_TOKEN "0"

/* status_codes */
#define HTTP_200_INT 200
#define HTTP_401_INT 401

/* types */
typedef struct {
    uv_tcp_t     socket;
    llhttp_t     parser;
    uv_connect_t connect;
    uv_buf_t     buf;
    uv_write_t   wr;

    char *url;
    char *body;
    size_t body_len;
    long content_len;
} client_t;

/* constants */
extern const char *DAY_NAMES[];
extern const char *MONTH_NAMES[];

/* sub functions */
int init_and_run(const char *sip, unsigned short sport);

/* llhttp_callbacks*/
int on_status(llhttp_t *parser, const char *at, size_t len);
int on_header_field(llhttp_t *parser, const char *at, size_t len);
int on_header_value(llhttp_t *parser, const char *at, size_t len);
int on_body(llhttp_t *parser, const char *at, size_t len);
int on_headers_complete(llhttp_t *parser);

/* uv_callbacks*/
void on_conn(uv_connect_t *req, int status);
void on_write(uv_write_t *req, int status);
void on_buf_alloc(uv_handle_t *handle, size_t sugg_size, uv_buf_t *buf);
void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t* buf);
void on_close(uv_handle_t *handle);

#endif /* CLIENT_H */
