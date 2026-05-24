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

#if 1



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
    GET_HISTORY
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
#define REG_USER_STR "%s\n%s\n%s"

#define LOGIN_USER_STR "%s\n%s"

#define INSERT_HISTORY_STR "%s\n%s\n%s"

#define VALIDATE_TOKEN_STR "%s"

#define ADD_TO_THE_HISTORY_STR "%s\n%s\n%s"

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

extern const char *DAY_NAMES[];
extern const char *MONTH_NAMES[];
}
#endif

#endif /* SERVERREQUESTER_H */

#if 0

#include <cstdlib>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <boost/asio.hpp>
#include <boost/json.hpp>
#include <iostream>

/* PLATFORN DEPENDED MACROS FOR DEALING WITH SYSTEM PATHS */
#ifdef _WIN32
#define OS_SEPERATOR "\\"
#else
#define OS_SEPERATOR "/"
#endif

namespace ip   = boost::asio::ip;
namespace asio = boost::asio;
namespace json = boost::json;
using     tcp  = boost::asio::ip::tcp;

namespace Server {
class Requester
{
public:
    Requester()  = default;
    ~Requester() = default;

    static int register_user(const char *const login,
                             const char *const password,
                             const char *const email,
                             int64_t    *user_id) noexcept;
    static int login_user(const char *const  login,
                          const char *const  password,
                          int64_t *user_id) noexcept;
    static int add_convertation_to_history(const char *inpath,
                                           const char *outpath,
                                           char   *banner_data,
                                           size_t *banner_data_len,
                                           int64_t userid) noexcept;

    static int   set_up();
    static char* ip()   noexcept;
    static int   port() noexcept;
    static constexpr size_t MAX_PATH_LEN = 128;

private:
    static char     ip_[16];
    static unsigned short port_;
};

constexpr char *OP_ERRORS[6] = {
    (char*)"Failed to connect to the server",
    (char*)"Login not found",
    (char*)"Wrong password",
    (char*)"This login has already been taken",
    (char*)"Request error",
    (char*)"Application error"
};

/* OPERATIONS THAT SERVER CAN DEAL WITH */
enum SQL_OPERATIONS {
    SQL_SELECT_USER,
    SQL_INSERT_USER,
    SQL_SELECT_CONVERTS,
    SQL_INSERT_CONVERTS
};

enum OP_STATUS {
    SERVER_ERR,
    LOGIN_ERR,
    PASSWORD_ERR,
    LOGIN_TAKEN_ERR,
    REQ_ERR,
    APP_ERR,
    OK
};
}

#endif

/*

err = llhttp_execute(parser, buf->base, nread);
    if (err == HPE_OK &&
        parser->type == HTTP_RESPONSE)
    {
        if (client->body_len < client->content_len) {
            log_str("[*][on_read] waiting for other packeges.\n");
            return;
        }

        if (strcmp(client->url, LOGIN_USER_URL) == 0) {
            if (parser->status_code == HTTP_200_INT) {

                memcpy(client->token, client->body, strlen(client->body));
            }
            else if (parser->status_code == HTTP_401_INT) {

            }
            else {

            }
            log_str("[*][on_read] LOGIN_USER_URL, token: ^%s^.\n", client->body);
        }
        else if (strcmp(client->url, VALIDATE_TOKEN_URL) == 0) {
            if (parser->status_code == HTTP_200_INT) {

            }
            else if (parser->status_code == HTTP_401_INT) {

            }
            else {

            }
            log_str("[*][on_read] VALIDATE_TOKEN_URL, token: ^%s^.\n", client->body);
        }       
        else if (strcmp(client->url, GET_HISTORY_URL) == 0) {
            if (parser->status_code == HTTP_200_INT) {

            }
            else if (parser->status_code == HTTP_204_INT) {
            }
            else {

            }
        }
        else if (strcmp(client->url, REG_USER_URL) == 0) {
            if (parser->status_code == HTTP_201_INT) {
                memcpy(client->token, client->body, strlen(client->body));
            }
            else if (parser->status_code == HTTP_409_INT) {

            }
            else {

            }
            log_str("[*][on_read] REG_USER_URL, token: ^%s^.\n", client->body);
        } 
        else if (strcmp(client->url, ADD_TO_THE_HISTORY_URL) == 0) {
            if (parser->status_code == HTTP_201_INT) {
                FILE *img_fd = NULL;
                size_t chars_wrote = 0;

                img_fd = fopen("img00.jpg", "wb");
                if (!img_fd) {
                    log_err("[!][on_read] Failed to fopen!\n");
                    uv_close((uv_handle_t*)&client->socket, onClose);
                    return;
                }

                log_str("[*][on_read] nread: %zu, "
                    "client->content_len: %ld, header_len: %lld.\n",
                    nread, client->content_len, nread - client->content_len);

                chars_wrote = fwrite(client->body, 1,
                    client->content_len, img_fd);
                log_str("[!][on_read] chars_wrote: %zu.\n", chars_wrote);

                fclose(img_fd);
            }
            else {

            }            
        }
        else {
            log_str("[!][on_read] unknown url.\n");
        }
    }
    else {
        log_err("[!][onRegUserRead] Failed to llhttp_execute.\n");
    }

*/
