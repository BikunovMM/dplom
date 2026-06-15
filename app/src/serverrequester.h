#ifndef SERVERREQUESTER_H
#define SERVERREQUESTER_H

#include <ctime>
#include <cstring>
#include <cstdlib>

#include <iostream>

#include <QFile>

extern "C" {
#include <libuv/uv.h>
#include <llhttp/llhttp.h>
}
#include "utils.h"

namespace ServerRequester
{

constexpr int LOGIN_STR_LEN = 33;
constexpr int PASSWORD_STR_LEN = 33;
constexpr int EMAIL_STR_LEN = 256;
constexpr int DATE_STR_LEN = 33;
constexpr int TOKEN_STR_LEN = 37;
constexpr int FILE_NAME_STR_LEN = 256;

enum class Operations {
    REG_USER,
    LOG_USER,
    VAL_TOKEN,
    INS_HISTORY,
    GET_HISTORY,
    SEND_EMAIL_CODE
};

typedef struct {
    uv_tcp_t     socket;
    llhttp_t     parser;
    uv_connect_t connect;
    uv_buf_t     buf;
    uv_write_t   wr;

    Operations  op;
    const char *login;
    const char *password;
    char *email;
    char *created_at;
    const char *inpath;
    const char *outpath;
    char *token;
    const char *code;

    uv_connect_cb on_conn;
    uv_write_cb   on_write;
    uv_read_cb    on_read;

    char *url;
    char *body;
    size_t body_len;
    long content_len;
} Client;

/* config_data */
constexpr char *SERVER_IP = (char*)"127.0.0.1";
constexpr int SERVER_PORT = 2006;
#define HOST "localhost"
constexpr char *CONFIG_PATH = (char*)"../configs/config.txt";
constexpr int IP_MAX_LEN = 16;
constexpr int PORT_MAX_LEN = 7;
constexpr int MAX_REG_REQ_LEN = 468;
constexpr int CREATED_AT_STR_LEN = 23;

/* buffer_sizes */
constexpr int ADD_TO_THE_HISTORY_BUF_LEN = 145000;

/* urls_targets */
constexpr char *REG_USER_URL = (char*)"/register";
constexpr char *LOGIN_USER_URL = (char*)"/login";
constexpr char *VALIDATE_TOKEN_URL = (char*)"/validate_token";
constexpr char *GET_HISTORY_URL = (char*)"/get_history";
constexpr char *GET_BANNER_URL = (char*)"/get_banner";
constexpr char *ADD_TO_THE_HISTORY_URL = (char*)"/add_to_the_history";
constexpr char *SEND_EMAIL_CODE_URL = (char*)"/upsert_email_code";

/* http_methods */
constexpr char *HTTPGET = (char*)"GET";
constexpr char *HTTPPOST = (char*)"POST";

/* http_request_header_format_str */
#define HTTP_REQ_HEAD                                  \
    "%s %s HTTP/1.1\r\n"                               \
    "Host: " HOST "\r\n"                               \
    "Date: %3s, %02d %3s %04d %02d:%02d:%02d GMT\r\n" \
    "Content-Type: text/plain; charset=utf-8\r\n"      \
    "Content-Length: %ld\r\n"                          \
    "\r\n"

/* http_body_format_str */
#define REG_USER_STR "%s\n%s\n%s\n%s"
#define LOGIN_USER_STR "%s\n%s"
#define INSERT_HISTORY_STR "%s\n%s\n%s"
#define VALIDATE_TOKEN_STR "%s"
#define ADD_TO_THE_HISTORY_STR "%s\n%s\n%s"
#define SEND_EMAIL_CODE_STR "%s"

/* responses_values */
constexpr char *VALID_TOKEN   = (char*)"1";
constexpr char *UNVALID_TOKEN = (char*)"0";

/* status_codes */
constexpr int HTTP_200_INT = 200;
constexpr int HTTP_201_INT = 201;
constexpr int HTTP_204_INT = 204;
constexpr int HTTP_401_INT = 401;
constexpr int HTTP_409_INT = 409;
constexpr int HTTP_500_INT = 500;

/* constants */
extern const char *DAY_NAMES[];
extern const char *MONTH_NAMES[];

int registerUser(const char *login,
                 const char *password,
                 const char *email,
                 const char *code,
                 char *token);
int loginUser(const char *login,
              const char *password,
              char *token,
              char *email,
              char *created_at);
int validateToken(char *token); /* todo */
int addToTheHistory(char         *token,
                    const char    *inpath,
                    const char    *outpath,
                    unsigned char **banner_data,
                    size_t        *banner_data_len);
int sendEmailCode(const char *email);

extern const char *DAY_NAMES[];
extern const char *MONTH_NAMES[];
}

#endif /* SERVERREQUESTER_H */
