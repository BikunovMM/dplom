#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <libuv/uv.h>
#include <llhttp/llhttp.h>
#include <libpq/libpq-fe.h>

#include "utils.h"

/* config_data */
#define CONFIG_PATH "../configs/config.txt"
#define SERVER_NAME "Server: Apache/2.4.41 (Ubuntu)"
#define IP_MAX_LEN 16
#define PORT_MAX_LEN 7
#define DBCONN_MAX_LEN 256
#define DEFAULT_BACKLOG 128

#define LOGIN_STR_LEN 33
#define PASSWORD_STR_LEN 33
#define EMAIL_STR_LEN 256
#define DATE_STR_LEN 33
#define TOKEN_STR_LEN 37
#define TOKEN_LEN 36
#define FILE_NAME_STR_LEN 256
#define FMT_STR_LEN 4
#define GET_CONVS_RES_LEN 17600
#define MAX_RESPONSE_STR_LEN 18000
#define GET_CONVS_RES_ROW_STR_LEN 560

#define BANNER_PATH00 "../resources/images/banner00.png"
#define BANNER_PATH01 "../resources/images/banner01.jpg"
#define BANNER_PATH02 "../resources/images/banner02.jpg"
#define BANNER_PATH03 "../resources/images/banner03.jpg"

#define BANNER_PATH00_LEN 25
#define BANNER_PATH01_LEN 25
#define BANNER_PATH02_LEN 25
#define BANNER_PATH03_LEN 25

/* urls_targets */
#define REG_USER_URL "/register"
#define LOGIN_USER_URL "/login"
#define VALIDATE_TOKEN_URL "/validate_token"
#define GET_HISTORY_URL "/get_history"
#define GET_BANNER_URL "/get_banner"
#define ADD_TO_THE_HISTORY_URL "/add_to_the_history"
#define GET_USER_DATA_URL "/get_user_data"

/* sql_requests_to_db */
#define SELECT_USER_REQ  "SELECT 1 FROM users WHERE login = $1 AND password = $2;"

//(SELECT ins.id FROM insert_user as ins)
/*
 * 1. login    (varchar)
 * 2. password (varchar)
 * 3. email    (varchar)
 */
#define INSERT_USER_REQ                                      \
    "WITH insert_user AS ( "                                  \
        "INSERT INTO users (login, password, "               \
        "email, created_at) "                                \
        "VALUES ($1, $2, $3, NOW()) "                        \
        "RETURNING id "                                      \
    "), "                                                    \
    "insert_fmts_use AS ( "                                  \
        "INSERT INTO use_of_formats "                        \
        "(user_id, format_id, quantity) "                    \
        "SELECT ins.id, f.id, 0 FROM formats as f, "         \
        "insert_user AS ins "                                \
        "RETURNING  ind.id "                                 \
        "AS user_id "                                        \
    ") "                                                     \
    "SELECT ins.user_id, NOW() "                             \
    "FROM insert_fmts_use AS ins "                           \
    "LIMIT 1;"

/*
 * 1. 
 * 2.
 */
#define INSERT_RETURNING_TOKEN_REQ                          \
    "INSERT INTO tokens (user_id, created_at, expires_at) " \
    "VALUES ($1, NOW(), NOW() + INTERVAL \'30 days\') "     \
    "RETURNING token;"

/* 
 * 1: login    (varchar)
 * 2: password (varchar)
 */
#define LOGIN_USER_REQ                                              \
    "WITH "                                                         \
        "valid_user AS ( "                                          \
            "SELECT id, email, created_at "                         \
            "FROM users "                                           \
            "WHERE login = $1 AND password = $2 "                   \
            "LIMIT 1 "                                              \
        "), "                                                       \
        "existing_token AS ( "                                      \
            "SELECT token, user_id "                                \
            "FROM tokens "                                          \
            "WHERE user_id = (SELECT id FROM valid_user) "          \
            "AND expires_at > NOW() "                               \
            "ORDER BY created_at DESC "                             \
            "LIMIT 1 "                                              \
        "), "                                                       \
        "new_token AS ( "                                           \
            "INSERT INTO tokens (user_id, created_at, expires_at) " \
            "SELECT vu.id, NOW(), NOW() + INTERVAL '30 days' "      \
            "FROM valid_user AS vu "                                \
            "WHERE NOT EXISTS (SELECT 1 FROM existing_token) "      \
            "AND EXISTS (SELECT 1 FROM valid_user) "                \
            "RETURNING token "                                      \
        ") "                                                        \
    "SELECT "                                                       \
        "COALESCE( "                                                \
            "(SELECT token FROM existing_token), "                  \
            "(SELECT token FROM new_token) "                        \
        ") AS token, "                                              \
        "email, "                 \
        "created_at "\
        "FROM valid_user;"

/* 1: token (uuid) */
#define VALIDATE_TOKEN_REQ                     \
    "SELECT 1 "                                \
    "FROM tokens "                             \
    "WHERE token = $1 AND expires_at > NOW();"

/* 1: token           (uuid)
 * 2: in_file_name    (varchar)
 * 3: out_file_name   (varchar)
 * 4: in_format_name  (varchar)
 * 5: out_format_name (varchar)
 */
#define INSERT_INTO_HISTORY_REQ                                                   \
    "WITH get_user AS ( "                                                         \
        "SELECT user_id AS id "                                                   \
        "FROM tokens "                                                            \
        "WHERE token = $1 "                                                       \
    "), "                                                                         \
    "update_infmt_use AS ( "                                                      \
        "UPDATE use_of_formats "                                                  \
        "SET quantity = quantity + 1 "                                            \
        "WHERE "                                                                  \
            "user_id = (SELECT id FROM get_user) AND "                            \
            "format_id = (SELECT id FROM formats WHERE name = $4) "               \
        "RETURNING format_id AS id "                                              \
    "), "                                                                         \
    "update_outfmt_use AS ( "                                                     \
        "UPDATE use_of_formats "                                                  \
        "SET quantity = quantity + 1 "                                            \
        "WHERE "                                                                  \
            "user_id = (SELECT id FROM get_user) AND "                            \
            "format_id = (SELECT id FROM formats WHERE name = $5) "               \
        "RETURNING format_id AS id "                                              \
    "), "                                                                         \
    "insert_infile AS ( "                                                         \
        "INSERT INTO files (name, format_id) "                                    \
        "SELECT $2, f.id FROM update_infmt_use as f "                             \
        "RETURNING id "                                                           \
    "), "                                                                         \
    "insert_outfile AS ( "                                                        \
        "INSERT INTO files (name, format_id) "                                    \
        "SELECT $3, f.id FROM update_outfmt_use as f "                            \
        "RETURNING id "                                                           \
    ") "                                                                          \
    "INSERT INTO convertations (user_id, in_file_id, out_file_id, converted_at) " \
    "SELECT "                                                                     \
        "get_user.id, "                                                           \
        "insert_infile.id, "                                                      \
        "insert_outfile.id, "                                                     \
        "NOW()"                                                                   \
    "FROM "                                                                       \
        "get_user, "                                                              \
        "insert_infile, "                                                         \
        "insert_outfile;"

#define GET_BANNER_REQ ""

/* 1. token uuid */
#define GET_USER_DATA_REQ                                        \
    "SELECT u.login, u.email, u.created_at "                     \
    "FROM users AS u "                                           \
    "WHERE u.id = (SELECT user_id FROM tokens WHERE token = $1);"

/* 1. token uuid */
#define GET_CONVERTATIONS_REQ                                                  \
    "SELECT "                                                                  \
        "c.id, in_file.name, out_file.name, "                                  \
        "in_format.name, out_format.name, c.converted_at "                     \
    "FROM "                                                                    \
        "convertations AS c "                                                  \
        "JOIN tokens   AS t          ON c.user_id = t.user_id "                \
        "JOIN files    AS in_file    ON c.in_file_id = in_file.id "            \
        "JOIN files    AS out_file   ON c.out_file_id = out_file.id "          \
        "JOIN formats  AS in_format  ON in_file.format_id = in_format.id "     \
        "JOIN formats  AS out_format ON out_format.format_id = out_format.id " \
    "WHERE t.token = $1;"

/* 1. token uuid */
#define GET_ADD_KRIT_REQ                                                                       \
    "WITH "                                                                                    \
    "get_user AS ( "                                                                           \
        "SELECT user_id AS id "                                                                \
        "FROM tokens "                                                                         \
        "WHERE token = $1 "                                                                    \
    "), "                                                                                      \
    "users_convertations_count AS ( "                                                          \
        "SELECT COUNT(user_id) AS convertations_count "                                        \
        "FROM convertations "                                                                  \
        "GROUP BY user_id "                                                                    \
    "), "                                                                                      \
    "users_max_convertations_count AS ( "                                                      \
        "SELECT MAX(convertations_count) AS max_count "                                        \
        "FROM users_convertations_count "                                                      \
    "), "                                                                                      \
    "max_last_converation_date AS ( "                                                          \
        "SELECT EXTRACT(DAY FROM AGE(NOW(), MIN(converted_at))) AS max_date "                  \
        "FROM convertations "                                                                  \
    "), "                                                                                      \
    "max_registration_date AS ( "                                                              \
        "SELECT EXTRACT(DAY FROM AGE(MIN(created_at))) AS max_date "                           \
        "FROM users "                                                                          \
    "), "                                                                                      \
    "dates_difs AS ( "                                                                         \
        "SELECT "                                                                              \
            "user_id, "                                                                        \
            "converted_at AS pres_date, "                                                      \
            "LAG(converted_at) OVER(ORDER BY converted_at) AS past_date "                      \
        "FROM convertations, get_user "                                                        \
        "WHERE user_id = get_user.id "                                                         \
    "), "                                                                                      \
    "all_dates_difs AS ( "                                                                     \
        "SELECT  "                                                                             \
            "user_id, "                                                                        \
            "converted_at AS pres_date, "                                                      \
            "LAG(converted_at) OVER(PARTITION BY user_id ORDER BY converted_at) AS past_date " \
        "FROM convertations "                                                                  \
        "GROUP BY user_id, pres_date "                                                         \
    "), "                                                                                      \
    "all_frequencies AS ( "                                                                    \
        "SELECT AVG(EXTRACT(DAYS FROM AGE(pres_date, past_date))) as frequency "               \
        "FROM all_dates_difs "                                                                 \
        "GROUP BY user_id "                                                                    \
    "), "                                                                                      \
    "most_used_format AS ( "                                                                   \
        "SELECT MAX(quantity) AS max_quantity "                                                \
        "FROM use_of_formats, get_user "                                                       \
        "WHERE user_id = get_user.id "                                                         \
    "), "                                                                                      \
    "max_format_quantity AS ( "                                                                \
        "SELECT MAX(quantity) AS max_quantity "                                                \
        "FROM use_of_formats "                                                                 \
    "), "                                                                                      \
    "max_format_type AS ( "                                                                    \
        "SELECT "                                                                              \
            "CASE "                                                                            \
                "WHEN fmts.type = 'video' THEN 1 "                                             \
                "WHEN fmts.type = 'audio' THEN 0.75 "                                          \
                "ELSE 0.25 "                                                                   \
            "END AS max_krit "                                                                 \
        "FROM use_of_formats AS use_of_fmts "                                                  \
        "LEFT JOIN formats AS fmts "                                                           \
        "ON use_of_fmts.format_id = fmts.id "                                                  \
        "WHERE quantity = (SELECT max_quantity FROM max_format_quantity) "                     \
        "ORDER BY max_krit DESC "                                                              \
        "LIMIT 1 "                                                                             \
    "), "                                                                                      \
    "total_count_of_all_convertations AS ( "                                                   \
        "SELECT "                                                                              \
            "COUNT(user_id) / (SELECT max_count FROM users_max_convertations_count) AS krit "  \
        "FROM "                                                                                \
            "convertations, get_user "                                                         \
        "WHERE "                                                                               \
            "user_id = get_user.id "                                                           \
    "), "                                                                                      \
    "days_from_the_last_convertation AS ( "                                                    \
        "SELECT "                                                                              \
            "EXTRACT(DAY FROM AGE(NOW(), converted_at)) "                                      \
            "/ COALESCE(NULLIF(max_last_converation_date.max_date, 0), 1) AS krit "            \
        "FROM convertations, max_last_converation_date, get_user "                             \
        "WHERE user_id = get_user.id "                                                         \
        "ORDER BY krit ASC "                                                                   \
        "LIMIT 1 "                                                                             \
    "), "                                                                                      \
    "days_from_registration AS ( "                                                             \
        "SELECT "                                                                              \
            "EXTRACT(DAY FROM AGE(NOW(), created_at)) "                                        \
            "/ (SELECT COALESCE(NULLIF(max_date, 0), 1) FROM max_registration_date) AS krit "  \
        "FROM users, get_user "                                                                \
        "WHERE users.id = get_user.id "                                                        \
    "), "                                                                                      \
    "frequency AS ( "                                                                          \
        "SELECT "                                                                              \
            "ROUND(AVG(EXTRACT(DAYS FROM AGE(dates_difs.pres_date, dates_difs.past_date))) "   \
            "/ (SELECT MAX(frequency) FROM all_frequencies), 2) AS krit "                      \
        "FROM dates_difs "                                                                     \
    "), "                                                                                      \
    "most_used_format_type AS ( "                                                              \
        "SELECT "                                                                              \
            "CASE "                                                                            \
                "WHEN fmts.type = 'video' THEN 1 "                                             \
                "WHEN fmts.type = 'audio' THEN 0.75 "                                          \
                "ELSE 0.25 "                                                                   \
            "END / (SELECT max_krit FROM max_format_type) AS krit "                            \
        "FROM use_of_formats AS use_of_fmts "                                                  \
        "LEFT JOIN formats as fmts ON use_of_fmts.format_id = fmts.id "                        \
        "WHERE quantity = (SELECT max_quantity FROM most_used_format) "                        \
        "ORDER BY krit DESC "                                                                  \
        "LIMIT 1 "                                                                             \
    ") "                                                                                       \
    "SELECT "                                                                                  \
        "ROUND( "                                                                              \
            "convs_count.krit  "                                                               \
            "- last_conv.krit * 0.25  "                                                         \
            "+ days_from_reg.krit * 0.5"                                                           \
            "+ freq.krit  "                                                                    \
            "+ most_used_type.krit  * 0.75, 2 "                                                \
        ") AS add_krit "                                                                       \
    "FROM "                                                                                    \
        "total_count_of_all_convertations AS convs_count, "                                    \
        "days_from_the_last_convertation AS last_conv, "                                       \
        "days_from_registration AS days_from_reg, "                                            \
        "frequency AS freq, "                                                                  \
        "most_used_format_type AS most_used_type;"

/* http_headers_statuses*/
#define HTTP_200_STR "HTTP/1.1 200 OK"
#define HTTP_201_STR "HTTP/1.1 201 Created"
#define HTTP_204_STR "HTTP/1.1 204 No Content"
#define HTTP_401_STR "HTTP/1.1 401 Unauthorized"
#define HTTP_404_STR "HTTP/1.1 404 Not Found"
#define HTTP_409_STR "HTTP/1.1 409 Conflict"
#define HTTP_500_STR "HTTP/1.1 500 Internal Server Error"
#define HTTP_501_STR "HTTP/1.1 501 Not Implemented"

/*  */
#define HTTP_CONN_CLOSE "\r\nConnection: close"

/* http_headers_requests_format_strs */
#define HTML_HEADER                                  \
    "%s\r\n"                                         \
    SERVER_NAME"\r\n"                                \
    "Date: %3s, %02d %3s %4d %02d:%02d:%02d GMT\r\n" \
    "Content-Type: text/html; charset=utf-8\r\n"     \
    "Content-Length: %ld\r\n"                        \
    "\r\n"

#define TEXT_HEADER                                  \
    "%s\r\n"                                         \
    SERVER_NAME"\r\n"                                \
    "Date: %3s, %02d %3s %4d %02d:%02d:%02d GMT\r\n" \
    "Content-Type: text/plain; charset=utf-8\r\n"    \
    "Content-Length: %ld\r\n"                        \
    "\r\n"

#define HEADER_WITHOUT_BODY                          \
    "%s\r\n"                                         \
    SERVER_NAME"\r\n"                                \
    "Date: %3s, %02d %3s %4d %02d:%02d:%02d GMT\r\n" \
    "Content-Length: 0\r\n"                          \
    "\r\n"

#define JPEG_HEADER                                  \
    "%s\r\n"                                         \
    SERVER_NAME"\r\n"                                \
    "Date: %3s, %02d %3s %4d %02d:%02d:%02d GMT\r\n" \
    "Content-Type: image/jpeg\r\n"                   \
    "Content-Length: %ld\r\n"                        \
    "\r\n"

/* predefined responses on 500 statuses */
#define HTTP500_RESPONSE    \
    HTTP_500_STR"\r\n"      \
    SERVER_NAME"\r\n"       \
    "Content-Length: 0\r\n" \
    "Connection: close\r\n" \
    "\r\n"

/* http_body_format_strs */
#define VALIDATE_TOKEN_BODY "%s"
#define REG_USER_BODY "%s\n%s"
#define LOGIN_USER_BODY "%s\n%s\n%s"
#define INSERT_HISTORY_BODY "%s"
#define GET_HISTORY_BODY  "%s"
#define ADD_HISTORY_BODY  "%s"

/* types */
typedef struct {
    uv_tcp_t handle;
    llhttp_t parser;

    char  url[32];
    char *body;
    long content_len;
} client_t;

typedef struct {
    uv_write_t  req;
    uv_buf_t    buf;
    client_t   *client;
    unsigned char buf_allocated;
} write_req_t;

/* constants */
extern const char *DAY_NAMES[];
extern const char *MONTH_NAMES[];

/* sub-functions */
int load_config(char *sip, unsigned short *sport);
int init_and_run(uv_tcp_t *server, const char *sip, unsigned short sport);

/* llhttp callbacks */
int on_message_begin(llhttp_t *parser);
int on_url(llhttp_t *parser, const char *at, size_t len);
int on_status(llhttp_t *parser, const char *at, size_t len);
int on_header_field(llhttp_t *parser, const char *at, size_t len);
int on_body(llhttp_t *parser, const char *at, size_t len);
int on_header_value(llhttp_t *parser, const char *at, size_t len);
int on_message_complete(llhttp_t *parser);

/* uv callbacks */
void on_new_conn(uv_stream_t *server, int status);
void on_buf_alloc(uv_handle_t *handle, size_t sug_size, uv_buf_t *buf);
void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);
void on_write(uv_write_t *req, int status);
void on_close(uv_handle_t *handle);
void on_shutdown(uv_shutdown_t *req, int status);

#endif /* MAIN_H */
