#define UTILS_IMPLEMENTATION
#include "main.h"

/* global_variables */
uv_loop_t         *g_loop             = NULL;
llhttp_settings_t  g_settings         = {0};
char               g_dbconn[128]      = {0};
char               g_http500_res[256] = {0};

/* header's_contants */
const char *DAY_NAMES[] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};
const char *MONTH_NAMES[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
    "Aug", "Sep", "Oct", "Nov", "Dec"
};

int main()
{
    uv_tcp_t        server         = {0};
    char            ip[IP_MAX_LEN] = {0};
    unsigned short  port           = 0;
    int             ret            = 0;

    srand((long long)&ret);

    ret = load_config(ip, &port);
    if (ret < 0) {
        log_err("[!][main] Failed to load_config.\n");
        return 1;
    }

    ret = init_and_run(&server, ip, port);
    if (ret < 0) {
        log_err("[!][main] Failed to init_and_run.\n");
        return 1;
    }

    return (ret < 0 ? 1 : 0);
}

int load_config(char           *sip,
                unsigned short *sport)
{
    FILE   *fd                = NULL;
    char    buf[PORT_MAX_LEN] = {0};
    size_t  str_len           = 0;
    int     ret               = 0;

    fd = fopen(CONFIG_PATH, "rb");

    //
    //  SERVER_IP (sip)
    //

    if (fgets(sip, IP_MAX_LEN, fd) == NULL) {
        log_err("[!][load_config] Failed to fgets sip.\n");
        ret = -1;
        goto cleanup;
    }

    str_len = strlen(sip);
    sip[str_len - 2] = '\0';

    //
    //  SERVER_PORT (sport)
    //

    if (fgets(buf, PORT_MAX_LEN, fd) == NULL) {
        log_err("[!][load_config] Failed to fgets sport\'s buf.\n");
        ret = -1;
        goto cleanup;
    }

    str_len = strlen(buf);
    buf[str_len - 2] = '\0';

    *sport = (unsigned short)strtol(buf, NULL, 10);

    //
    //  SERVER_DB_CONNECT_SRT (g_dbconn)
    //

    if (fgets(g_dbconn, DBCONN_MAX_LEN, fd) == NULL) {
        log_err("[!][load_config] Failed to fgets g_dbconn.\n");
        ret = -1;
        goto cleanup;
    }

    str_len = strlen(g_dbconn);
    g_dbconn[str_len - 2] = '\0';

    log_str("ip: %s, port: %s,\ng_dbconn: %s.\n\n", sip, buf, g_dbconn);

cleanup:
    fclose(fd);

    return ret;
}

int init_and_run(uv_tcp_t       *server,
                 const char     *sip,
                 unsigned short  sport)
{
    struct sockaddr_in addr = {0};
    int    ret              = 0;

    g_loop = uv_default_loop();
    if (!g_loop) {
        log_err("[!] [init_and_run] Failed to uv_default_loop.\n");
        return -1;
    }

    llhttp_settings_init(&g_settings);
    g_settings.on_message_begin    = on_message_begin;
    g_settings.on_url              = on_url;
    g_settings.on_status           = on_status;
    g_settings.on_header_field     = on_header_field;
    g_settings.on_header_value     = on_header_value;
    g_settings.on_body             = on_body;
    g_settings.on_message_complete = on_message_complete;

    ret = uv_tcp_init(g_loop, server);
    if (ret < 0) {
        log_err("[!] [init_and_run] Failed to uv_tcp_init.\n");
        goto cleanup;
    }

    ret = uv_ip4_addr(sip, sport, &addr);
    if (ret < 0) {
        log_err("[!] [init_and_run] Failed to uv_ip4_addr.\n");
        goto cleanup;
    }

    ret = uv_tcp_bind(server, (const struct sockaddr*)&addr, 0);
    if (ret < 0) {
        log_err("[!] [init_and_run] Failed to uv_tcp_bind.\n");
        goto cleanup;
    }

    ret = uv_listen((uv_stream_t*)server, DEFAULT_BACKLOG, on_new_conn);
    if (ret < 0) {
        log_err("[!] [init_and_run] Failed to uv_listen.\n");
        goto cleanup;
    }

    log_str("[*] [main] server starts listening.\n");

    ret = uv_run(g_loop, UV_RUN_DEFAULT);
    if (ret < 0) {
        log_err("[!] [init_and_run] Failed to uv_run.\n");
    }

    log_str("[*] [main] g_loop stopped.\n");

cleanup:
    uv_loop_close(g_loop);
    return ret;
}

int on_message_begin(llhttp_t *parser)
{
    (void)parser;
    return 0;
}

/* no_need. erase_if_possible */
int on_url(llhttp_t   *parser,
           const char *at,
           size_t      len)
{
    (void)parser;
    (void)at;
    (void)len;    

    client_t *client = (client_t*)parser->data;

    memcpy(client->url, at, len);
    client->url[len] = '\0';
    
    //log_str("[*][on_url] http:\n%s, (len: %zu)\n", client->url, len);

    return 0;
}

int on_status(llhttp_t *parser, const char *at, size_t len)
{
    (void)parser;
    (void)at;
    (void)len;

    return 0;
}

int on_header_field(llhttp_t *parser, const char *at, size_t len)
{
    (void)parser;
    (void)at;
    (void)len;

    if (strncmp(at, "Content-Length", len) == 0) {
        client_t *client = (client_t*)parser->data;
        client->content_len = -1;
    }

    return 0;
}

int on_header_value(llhttp_t *parser, const char *at, size_t len)
{
    (void)parser;
    (void)at;
    (void)len;

    client_t *client = (client_t*)parser->data;

    if (client->content_len == -1) {
        char buf[10] = {0};
        
        memcpy(buf, at, len);
        client->content_len = strtol(buf, NULL, 10);
    }

    return 0;
}

int on_body(llhttp_t *parser, const char *at, size_t len)
{
    (void)parser;
    (void)at;
    (void)len;

    client_t *client = (client_t*)parser->data;

    client->body = (char*)malloc((len + 1) * sizeof(char));
    if (!client->body) {
        log_err("[!] [on_url] Failed malloc client->body.\n");
        return -1;
    }  

    memcpy(client->body, at, len);
    client->body[len] = '\0';

    //log_str("[*][on_body] on_body:\n%s(len: %zu)\n\n", client->body, len);

    return 0;
}

int on_message_complete(llhttp_t *parser)
{
    (void)parser;
    return 0;
}

void on_new_conn(uv_stream_t *server, int status)
{
    if (status < 0) {
        log_err("[!] [on_new_conn] Failed. status < 0.\n");
        return;
    }    

    client_t *client = NULL;
    int       ret    = 0;

    log_str("[*] [on_new_conn] found.\n");

    client = (client_t*)calloc(1, sizeof(client_t));
    if (!client) {
        log_err("[!] [on_new_conn] Failed to calloc client.\n");
        return;
    }

    ret = uv_tcp_init(g_loop, &client->handle);
    if (ret < 0) {
        log_err("[!] [main] Failed to uv_listen.\n");
        free(client);
        return;
    }

    llhttp_init(&client->parser, HTTP_REQUEST, &g_settings);
    client->parser.data = client;

    ret = uv_accept(server, (uv_stream_t*)client);
    if (ret == 0) {
        log_str("\t[*] [on_new_conn] accepted.\n");
        uv_read_start((uv_stream_t*)client, on_buf_alloc, on_read);
    }
    else {
        log_err("[!] [on_new_conn] Failed to uv_tcp_accept.\n");
        uv_close((uv_handle_t*)&client->handle, on_close);
    }
}

void on_buf_alloc(uv_handle_t *handle, size_t sug_size, uv_buf_t *buf)
{
    (void)handle;

    if (sug_size == 0) {
        log_err("[!] [on_buf_alloc] Failed. sug_size == 0.\n");
        buf->len = 0;
        return;
    }

    buf->base = (char*)malloc(sug_size * sizeof(char));
    if (!buf->base) {
        log_err("[!] [on_buf_alloc] Failed to malloc buf->base.\n");
        buf->len = 0;
        return;
    }

    buf->len = sug_size;

    //log_str("[*] [on_buf_alloc] buf allocated.\n");
}

void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
    if (nread < 0) {
        if (nread == UV_EOF) {
            log_str("[*] [on_read] client closed connection (EOF).\n");            
        }
        else {
            log_err("[!] [on_read] failed to read.\n");
        }
        goto close;
    }
    else if (nread == 0){
        log_err("[!] [on_read] no data provided.\n");
        free(buf->base);
        return;
    }

    if (!buf->base || buf->len <= 0) {
        log_err("[!] [on_read] Failed. buf->base is NULL or buf->len <= 0.\n");
        goto close;
    }

    client_t          *client      = NULL;
    write_req_t       *req         = NULL;
    char              *res_str     = NULL;
    enum llhttp_errno  err         = 0;
    llhttp_t          *parser      = NULL;
    time_t             cur_time    = {0};
    struct tm         *cur_time_tm = NULL;
    size_t             res_str_len = 0;
    int                ret         = 0;

    client = (client_t*)stream;
    parser = &client->parser;

    req = (write_req_t*)malloc(sizeof(write_req_t));
    if (!req) {
        log_err("[!] [on_read] Failed to malloc req.\n");
        goto close;
    }

    req->client = client;
    req->buf_allocated = 1;

    err = llhttp_execute(parser, buf->base, nread);
    if (err == HPE_OK &&
        parser->type == HTTP_REQUEST &&
        parser->status_code == 0)
    {
        const char *req_url  = client->url;
        char       *body_str = NULL;
        //char        buf_str[MAX_RESPONSE_STR_LEN] = {0};

        time_t cur_time   = {0};
        struct tm *cur_tm = NULL;

        PGconn   *conn = NULL;
        PGresult *res  = NULL;
        PGresult *res1 = NULL;

        size_t i = 0;
        char *status = HTTP_200_STR;
        char *header = TEXT_HEADER;

        size_t body_len = 0;

        body_str = client->body;

        if (!client->body || client->content_len <= 0) {
            res_str = HTTP400_RESPONSE;
            res_str_len = strlen(res_str);
            req->buf_allocated = 0;
            goto write_buf;
        }

        cur_time = time(NULL);
        cur_tm = localtime(&cur_time);

        if (parser->method == HTTP_GET) {
            if (strcmp(req_url, LOGIN_USER_URL) == 0) {
                char  login_str[LOGIN_STR_LEN]       = {0};
                char  password_str[PASSWORD_STR_LEN] = {0};
                char *email                          = NULL;
                char *token                          = NULL;
                char *created_at                     = NULL;

                const char *strs[3]      = {0};
                size_t      strs_lens[3] = {0};
                int         cur_str      = 0;
                int         last_str     = 0;
                const char *last_str_ptr = body_str;

                const char *params[3] = {0};

                //
                //  PARSING_BODY_STR
                //

                for (i = 0; i < client->content_len + 1; ++i) {
                    if (body_str[i] == '\n' || body_str[i] == '\0') {
                        strs[cur_str] = last_str_ptr;
                        strs_lens[cur_str] = body_str + i - last_str_ptr;

                        ++cur_str;
                        last_str = cur_str;

                        last_str_ptr = body_str + i + 1;
                    }
                }

                if (!strs[0] || !strs[1]) {
                    res_str = HTTP400_RESPONSE;
                    res_str_len = strlen(res_str);
                    req->buf_allocated = 0;
                    goto write_buf;
                }

                memcpy(login_str, strs[0], strs_lens[0]);
                memcpy(password_str, strs[1], strs_lens[1]);

                //cur_time = time(NULL);
                //cur_tm = localtime(&cur_time);

                params[0] = login_str;
                params[1] = password_str;

                log_str("[*][on_read] body:\n^%s^\nclient->content_len: %ld, "
                    "login: %s, password: %s.\n",
                    body_str, client->content_len, login_str, password_str);

                //
                //  DB_REQUEST (VALIDATE_TOKEN)
                //

                conn = PQconnectdb(g_dbconn);
                if (PQstatus(conn) != CONNECTION_OK) {
                    log_err("[!][on_read] Failed to PQconnect.\n");
                    goto err_login_500;
                }

                res = PQexecParams(conn, LOGIN_USER_REQ, 2, NULL,
                                   (const char * const*)params,
                                   NULL, NULL, 0);
                if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                    log_err("[!][on_read] Failed to PQexecParams "
                            "LOGIN_USER_REQ.\n");
                    goto err_login_500_close_db;
                }

                if (PQntuples(res) == 0) {
                    log_str("[*][on_read] user doesn\' exists.\n");
                    status = HTTP_401_STR;
                    goto err_login_close_db;
                }

                token = PQgetvalue(res, 0, 0);
                email = PQgetvalue(res, 0, 1);
                created_at = PQgetvalue(res, 0, 2);                

                PQclear(res);
                PQfinish(conn);

                //
                //  FORMATING_HTTP_RESPONSE
                //                

                body_len = strlen(token) + strlen(email) + strlen(created_at) + 2;

                log_str("[*][on_read] token: %s, email: %s, "
                    "created_at: %s, body_len: %zu. ntuples: %zu.\n",
                    token, email, created_at, body_len, PQntuples(res));

                res_str_len = snprintf(NULL, 0,
                                       TEXT_HEADER LOGIN_USER_BODY, HTTP_200_STR,
                                       DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                                       MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                                       cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec,
                                       body_len,
                                       token, email, created_at);

                res_str_len += 1; /* null-terminator */

                res_str = (char*)malloc((res_str_len) * sizeof(char));
                if (!res_str) {
                    log_err("[!][on_read] Failed to malloc.\n");
                    goto err_login_500;
                }

                res_str_len = snprintf(res_str, res_str_len,
                                       TEXT_HEADER LOGIN_USER_BODY, HTTP_200_STR,
                                       DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                                       MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                                       cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec,
                                       body_len,
                                       token, email, created_at);

                res_str[res_str_len] = '\0';

                goto write_buf;

                /* on_errors */
                err_login_close_db:

                PQclear(res);
                PQfinish(conn);

                err_login_format_response:

                res_str_len = snprintf(NULL, 0,
                                       TEXT_HEADER, status,
                                       DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                                       MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                                       cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec, 0);

                res_str_len += 1; /* null-terminator */

                res_str = (char*)malloc((res_str_len) * sizeof(char));
                if (!res_str) {
                    log_err("[!][on_read] Failed to malloc.\n");
                    goto err_login_500;
                }

                res_str_len = snprintf(res_str, res_str_len,
                                       TEXT_HEADER, status,
                                       DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                                       MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                                       cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec, 0);

                goto write_buf;

                /* on_status_500 */
                err_login_500_close_db:
                PQclear(res);
                PQfinish(conn);
                    
                err_login_500:
                res_str = HTTP500_RESPONSE;
                res_str_len = strlen(res_str);
                req->buf_allocated = 0;
            }
            else if (strcmp(req_url, VALIDATE_TOKEN_URL) == 0) {
                int rows_nb = 0;

                //
                //  DB_REQUEST (VALIDATE_TOKEN)
                //

                log_str("[*][on_read] body_str: ^%s^.\n", body_str);

                conn = PQconnectdb(g_dbconn);
                if (PQstatus(conn) != CONNECTION_OK) {
                    log_err("[!][on_read] Failed to PQconnect.\n");
                    goto err_token_500;
                }

                res = PQexecParams(conn, VALIDATE_TOKEN_REQ, 1, NULL,
                    (const char * const*)&body_str, NULL, NULL, 0);
                if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                    log_err("[!][on_read] Failed to PQexecParams.\n");
                    goto err_token_500_close_db;
                }

                rows_nb = PQntuples(res);
                if (rows_nb == 0) {
                    status = HTTP_401_STR;
                }

                err_token_close_db:
                PQclear(res);
                PQfinish(conn);
            
                //
                //  FORMATING_HTTP_RESPONSE
                //
                
                err_token_format_response:
                //cur_time = time(NULL);
                //cur_tm = localtime(&cur_time);

                res_str_len = snprintf(NULL, 0,
                    HEADER_WITHOUT_BODY, status,
                    DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                    MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                    cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec);

                res_str_len += 1; /* null-terminator */

                res_str = (char*)malloc((res_str_len) * sizeof(char));
                if (!res_str) {
                    log_err("[!][on_read] Failed to malloc.\n");
                    goto err_token_500;
                }

                log_str("res_str1: %zu.\n", res_str_len);

                res_str_len = snprintf(res_str, res_str_len,
                    HEADER_WITHOUT_BODY, status,
                    DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                    MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                    cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec);

                res_str_len -= 1;

                goto write_buf;

                /* on_status_500 */
                err_token_500_close_db:
                PQclear(res);
                PQfinish(conn);

                err_token_500:
                res_str = HTTP500_RESPONSE;
                res_str_len = strlen(res_str);
                req->buf_allocated = 0;
            }
            else if (strcmp(req_url, GET_HISTORY_URL) == 0) {
                char   response[GET_CONVS_RES_LEN] = {0};
                int    rows_nb                     = 0;
                size_t convs_rows_len              = 0;

                size_t cur_pos = 0;
                size_t col_len = 0;
                const char *col_str = NULL;
                
                //cur_time = time(NULL);
                //cur_tm = localtime(&cur_time);

                //
                //  DB_REQUEST (VALIDATE_TOKEN)
                //

                log_str("[*][on_read] getting_user_convertations.\n");

                conn = PQconnectdb(g_dbconn);
                if (PQstatus(conn) != CONNECTION_OK) {
                    log_err("[!][on_read] Failed to PQconnect.\n");
                    goto err_get_history_500;
                }

                log_str("[*][on_read] pre GET_CONVERTATIONS_REQ, body_str: ^%s^.\n", body_str);

                res = PQexecParams(conn, GET_CONVERTATIONS_REQ, 1, NULL,
                                   (const char * const*)&body_str,
                                   NULL, NULL, 0);
                if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                    log_err("[!][on_read] Failed to PQexecParams "
                            "GET_CONVERTATIONS_REQ.\n");
                    goto err_get_history_500_close_db;
                }

                log_str("[*][on_read] post GET_CONVERTATIONS_REQ.\n");

                rows_nb = PQntuples(res);

                log_str("[*][on_read] ntuples: %d.\n", rows_nb);

                if (rows_nb == 0) {
                    log_str("[*][on_read] no content were found.\n");
                    status = HTTP_204_STR;
                    goto err_get_history_close_db;
                }                

                for (int i = 0; i < rows_nb; ++i) {
                    convs_rows_len += strlen(PQgetvalue(res, i, 0)) +
                        strlen(PQgetvalue(res, i, 1)) + strlen(PQgetvalue(res, i, 2)) +
                        strlen(PQgetvalue(res, i, 3)) + strlen(PQgetvalue(res, i, 4)) +
                        strlen(PQgetvalue(res, i, 5)) + 7;
                }                

                convs_rows_len -= 2;

                log_str("[*][on_read] convs_rows_len: %zu.\n", convs_rows_len);
                log_str("[*][on_read] going to format res_str.\n");

                res_str_len = snprintf(NULL, 0,
                                       TEXT_HEADER, HTTP_200_STR,
                                       DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                                       MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                                       cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec,
                                       convs_rows_len);

                res_str = (char*)calloc(convs_rows_len + res_str_len + 2, sizeof(char)); /* +2 cause of '\n\n' in the res_str, while formatting */
                if (!res_str) {
                    log_err("[!][on_read] Failed to calloc res_str.\n");
                    goto err_get_history_500;
                }

                res_str_len = snprintf(res_str, convs_rows_len + res_str_len,
                                       TEXT_HEADER, HTTP_200_STR,
                                       DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                                       MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                                       cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec,
                                       convs_rows_len);

                cur_pos += res_str_len;

                for (size_t i = 0; i < rows_nb; ++i) {
                    col_str = PQgetvalue(res, i, 0);
                    col_len = strlen(col_str);                    
                    memcpy(res_str + cur_pos, col_str, col_len);
                    cur_pos += col_len;
                    res_str[cur_pos] = '\n';
                    cur_pos += 1;

                    col_str = PQgetvalue(res, i, 1);
                    col_len = strlen(col_str);                    
                    memcpy(res_str + cur_pos, col_str, col_len);
                    cur_pos += col_len;
                    res_str[cur_pos] = '\n';
                    cur_pos += 1;

                    col_str = PQgetvalue(res, i, 2);
                    col_len = strlen(col_str);                    
                    memcpy(res_str + cur_pos, col_str, col_len);
                    cur_pos += col_len;
                    res_str[cur_pos] = '\n';
                    cur_pos += 1;

                    col_str = PQgetvalue(res, i, 3);
                    col_len = strlen(col_str);                    
                    memcpy(res_str + cur_pos, col_str, col_len);
                    cur_pos += col_len;
                    res_str[cur_pos] = '\n';
                    cur_pos += 1;

                    col_str = PQgetvalue(res, i, 4);
                    col_len = strlen(col_str);                    
                    memcpy(res_str + cur_pos, col_str, col_len);
                    cur_pos += col_len;
                    res_str[cur_pos] = '\n';
                    cur_pos += 1;

                    col_str = PQgetvalue(res, i, 5);
                    col_len = strlen(col_str);                    
                    memcpy(res_str + cur_pos, col_str, col_len);
                    cur_pos += col_len;
                    res_str[cur_pos] = '\n';
                    res_str[cur_pos + 1] = '\n';
                    cur_pos += 2;
                }

                cur_pos -= 2;
                res_str[cur_pos] = '\0';
                res_str_len = cur_pos;

                PQclear(res);
                PQfinish(conn);                

                log_str("\n-----\n[*][on_read] res_str:\n^%s^\n"
                    "(res_str_len: %zu, cur_pos: %zu)\n\n",
                    res_str, res_str_len, cur_pos);

                goto write_buf;

                /* on_error */

            err_get_history_close_db:

                PQclear(res);
                PQfinish(conn);

                err_get_history_format_response:

                res_str_len = snprintf(NULL, 0,
                                       header, status,
                                       DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                                       MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                                       cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec, 0);

                res_str += 1; /* null-terminator */

                res_str = (char*)malloc((res_str_len) * sizeof(char));
                if (!res_str) {
                    log_err("[!][on_read] Failed to malloc.\n");
                    goto err_get_history_500;
                }

                res_str_len = snprintf(res_str, res_str_len,
                                       header, status,
                                       DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                                       MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                                       cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec, 0);

                goto write_buf;

                /* on_status_500 */
            err_get_history_500_close_db:
                PQclear(res);
                PQfinish(conn);

            err_get_history_500:
                res_str = HTTP500_RESPONSE;
                res_str_len = strlen(res_str);
                req->buf_allocated = 0;
            }
            else if (strcmp(req_url, GET_USER_DATA_URL) == 0) {
                //
            }
            else {
                res_str = HTTP404_RESPONSE;
                res_str_len = strlen(res_str);
                req->buf_allocated = 0;

                log_str("[?][on_read] unknown http url/target.\n");
            }
        }        
        else if (parser->method == HTTP_POST) {
            if (strcmp(req_url, UPSERT_EMAIL_CODE_URL) == 0) {                
                char code_str[EMAIL_CODE_STR_LEN] = {0};
                char *params[3]       = {0};
                size_t message_len = 0;
                
                code_str[0] = rand() % 10 + 48;
                code_str[1] = rand() % 10 + 48;
                code_str[2] = rand() % 10 + 48;
                code_str[3] = rand() % 10 + 48;
                code_str[4] = rand() % 10 + 48;

                params[0] = body_str;
                params[1] = code_str;

                log_str("[*][on_read] email: ^%s^, code: ^%s^.\n", body_str, code_str);

                //
                //  UPSERTING EMAIL_CODES
                //

                conn = PQconnectdb(g_dbconn);
                if (PQstatus(conn) != CONNECTION_OK) {
                    log_err("[!][on_read] Failed to PQconnect.\n");
                    goto err_upset_email_code_500;
                }

                res = PQexecParams(conn, CHECK_USERS_EMAIL_REQ, 1, NULL,
                                   (const char *const *)params, NULL, NULL, 0);
                if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                    log_err("[!][on_read] Failed to PQexecParams "
                        "CHECK_USERS_EMAIL_REQ.\n");
                    goto err_upset_email_code_500_close_db;
                }

                if (PQntuples(res) != 0) {
                    log_err("[!][on_read] email is already taken.\n");
                    status = HTTP_409_STR;
                    goto upsert_email_code_err;
                }

                res = PQexecParams(conn, UPSERT_EMAIL_CODE_REQ, 2, NULL,
                                   (const char *const *)params, NULL, NULL, 0);
                if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                    log_err("[!][on_read] Failed to PQexecParams "
                        "UPSERT_EMAIL_CODE_REQ.\n");
                    goto err_upset_email_code_500_close_db;
                }

            upsert_email_code_err:
                PQclear(res);
                PQfinish(conn);

                //
                //
                //

                res_str_len = snprintf(NULL, 0,
                    TEXT_HEADER, status,
                    DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                    MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                    cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec, 0);

                res_str_len += 1; /* null-terminator */

                res_str = (char*)malloc((res_str_len) * sizeof(char));
                if (!res_str) {
                    log_err("[!][on_read] Failed to malloc.\n");
                    goto err_upset_email_code_500;
                }

                res_str_len = snprintf(res_str, res_str_len,
                    TEXT_HEADER, status,
                    DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                    MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                    cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec, 0);

                //
                //  CURL
                //

                CURL               *curl        = NULL;
                CURLcode            result         = CURLE_OK;
                struct curl_slist *recipients   = NULL;
                char               message[256] = {0};

                curl = curl_easy_init();
                if (!curl) {
                    log_err("[!][on_read] Failed to curl_easy_init.\n");
                    goto err_upset_email_code_500;
                }

                curl_easy_setopt(curl, CURLOPT_URL, SMTP_SERVER_URL);

                curl_easy_setopt(curl, CURLOPT_USERNAME, SMTP_SERVER_USERNAME);
                curl_easy_setopt(curl, CURLOPT_PASSWORD, SMTP_SERVER_PASSWORD);
                
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
                curl_easy_setopt(curl, CURLOPT_CAINFO, CERTIFICATE_PATH);


                curl_easy_setopt(curl, CURLOPT_MAIL_FROM, SMTP_SENDER);
                recipients = curl_slist_append(recipients, body_str);
                curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

                message_len = snprintf(message, sizeof(message),
                    SEND_CODE_MSG_STR, body_str, code_str);

                log_str("[*][on_read] message:\n%s\n(msg_len: %zu)\n", message, message_len);

                curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
                curl_easy_setopt(curl, CURLOPT_READDATA, message);
                curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
                curl_easy_setopt(curl, CURLOPT_INFILESIZE, message_len);

                curl_easy_setopt(curl, CURLOPT_TIMEOUT, 16L);
                curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 8L);

                result = curl_easy_perform(curl);
                if (result != CURLE_OK) {
                    curl_slist_free_all(recipients);
                    curl_easy_cleanup(curl);
                    log_err("[!][on_read] Failed to curl_easy_perform. Err: %s.\n",
                        curl_easy_strerror(result));
                    goto err_upset_email_code_500;
                }

                curl_slist_free_all(recipients);
                curl_easy_cleanup(curl);                

                //
                //
                //

                goto write_buf;

                /* on_status_500 */

            err_upset_email_code_500_close_db:
                PQclear(res);
                PQfinish(conn);

            err_upset_email_code_500:
                res_str = HTTP500_RESPONSE;
                res_str_len = strlen(res_str);
                req->buf_allocated = 0;
            }
            else if (strcmp(req_url, REG_USER_URL) == 0) {
                char  login_str[LOGIN_STR_LEN]       = {0};
                char  password_str[PASSWORD_STR_LEN] = {0};
                char  email_str[EMAIL_STR_LEN]       = {0};
                char  code_str[EMAIL_CODE_STR_LEN]   = {0};
                char *token                          = NULL;
                char *created_at                     = NULL;                

                const char *strs[4]      = {0};
                size_t      strs_lens[4] = {0};
                int         cur_str      = 0;
                int         last_str     = 0;
                const char *last_str_ptr = body_str;

                const char *params[4] = {0};

                //
                //  PARSING_BODY_STR
                //
             
                for (i = 0; i < client->content_len + 1; ++i) {
                    if (body_str[i] == '\n' || body_str[i] == '\0') {
                        strs[cur_str] = last_str_ptr;
                        strs_lens[cur_str] = body_str + i - last_str_ptr;

                        ++cur_str;
                        last_str = cur_str;

                        last_str_ptr = body_str + i + 1;
                    }
                }

                if (!strs[0] || !strs[1] || !strs[2]) {
                    res_str = HTTP400_RESPONSE;
                    res_str_len = strlen(res_str);
                    req->buf_allocated = 0;
                    goto write_buf;
                }

                memcpy(login_str, strs[0], strs_lens[0]);
                memcpy(password_str, strs[1], strs_lens[1]);
                memcpy(email_str, strs[2], strs_lens[2]);
                memcpy(code_str, strs[3], strs_lens[3]);

                //cur_time = time(NULL);
                //cur_tm = localtime(&cur_time);

                params[0] = login_str;
                params[1] = password_str;
                params[2] = email_str;
                params[3] = code_str;

                log_str("login: %s, password: %s, email: %s, code: %s.\n",
                    login_str, password_str, email_str, code_str);

                //
                //  DB_REQUEST (VALIDATE_TOKEN)
                //

                conn = PQconnectdb(g_dbconn);
                if (PQstatus(conn) != CONNECTION_OK) {
                    log_err("[!][on_read] Failed to PQconnect.\n");
                    goto err_reg_user_500_close_db;
                }

                res = PQexecParams(conn, SELECT_USER_REQ, 2, NULL,
                                   (const char * const*)params,
                                   NULL, NULL, 0);
                if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                    log_err("[!][on_read] Failed to PQexecParams "
                        "SELECT_USER_REQ.\n");
                    goto err_reg_user_500_close_db;
                }

                if (PQntuples(res) != 0) {
                    log_str("[*][on_read] user already exists!\n");
                    status = HTTP_409_STR;
                    goto err_reg_user_close_db;
                }

                log_str("[*][on_read] ok, user doesn\'t exists.\n");
                
                PQclear(res);

                res = PQexecParams(conn, INSERT_USER_REQ, 4, NULL,
                                   (const char * const*)params,
                                   NULL, NULL, 0);
                if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                    log_err("[!][on_read] Failed to PQexecParams "
                        "INSERT_USER_REQ.\n");
                    goto err_reg_user_500_close_db;
                }                

                if (PQntuples(res) == 0) {
                    log_str("[*][on_read] wrong email code.\n");
                    status = HTTP_400_STR;
                    goto err_reg_user_close_db;
                }

                log_str("[*][on_read] ok, code is correct. user was written into the db.\n");

                log_str("[*][on_read] inserted user and "
                        "use_of_formats.\n");

                params[0] = PQgetvalue(res, 0, 0);
                created_at = PQgetvalue(res, 0, 1);

                //strncpy(created_at, PQgetvalue(res, 0, 1), sizeof(created_at));

                //PQclear(res);

                //params[1] = created_at_str;

                log_str("[*][on_read] goint to get_token.\n");

                res1 = PQexecParams(conn, INSERT_RETURNING_TOKEN_REQ, 1, NULL,
                                   (const char * const*)params,
                                   NULL, NULL, 0);
                if (PQresultStatus(res1) != PGRES_TUPLES_OK) {
                    log_err("[!][on_read] Failed to PQexecParams "
                        "INSERT_RETURNING_TOKEN_REQ.\n");
                    goto err_reg_user_500_close_db;
                }

                log_str("[*][on_read] get_token successed!\n");

                if (PQntuples(res1) == 0) {
                    PQclear(res1);
                    goto err_reg_user_500_close_db;                    
                }
                
                token = PQgetvalue(res1, 0, 0);
                //strncpy()
                
                PQfinish(conn);

                body_len = strlen(token) + strlen(created_at) + 1;

                res_str_len = snprintf(NULL, 0,
                    TEXT_HEADER REG_USER_BODY, HTTP_201_STR,
                    DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                    MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                    cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec,
                    body_len, token, created_at);

                res_str_len += 1; /* null-terminator */

                log_str("[*][on_read] res_str_len: %zu.\n", res_str_len);

                res_str = (char*)malloc((res_str_len) * sizeof(char));
                if (!res_str) {
                    log_err("[!][on_read] Failed to malloc.\n");
                    PQclear(res1);
                    goto err_reg_user_500;
                }

                res_str_len = snprintf(res_str, res_str_len,
                    TEXT_HEADER REG_USER_BODY, HTTP_201_STR,
                    DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                    MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                    cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec,
                    body_len, token, created_at);

                log_str("[*][on_read] res_str_len: %zu, strlen(res_str): %zu.\n",
                    res_str_len, strlen(res_str));

                log_str("[*][on_read] created_at: ^%s^\n", created_at);

                PQclear(res);
                PQclear(res1);

                goto write_buf;

                err_reg_user_close_db:

                PQclear(res);
                PQfinish(conn);

                err_reg_user_format_response:

                //
                //  FORMATING_HTTP_RESPONSE
                //

                res_str_len = snprintf(NULL, 0,
                    header, status,
                    DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                    MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                    cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec, 0);

                res_str_len += 1; /* null-terminator */

                res_str = (char*)malloc((res_str_len) * sizeof(char));
                if (!res_str) {
                    log_err("[!][on_read] Failed to malloc.\n");
                    goto err_reg_user_500;
                }

                res_str_len = snprintf(res_str, res_str_len,
                    header, status,
                    DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                    MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                    cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec, 0);

                goto write_buf;

                /* on_status_500 */
                err_reg_user_500_close_db:
                PQclear(res);
                PQfinish(conn);

                err_reg_user_500:
                res_str = HTTP500_RESPONSE;
                res_str_len = strlen(res_str);
                req->buf_allocated = 0;
            }
            else if (strcmp(req_url, ADD_TO_THE_HISTORY_URL) == 0) {
                char  token[TOKEN_STR_LEN]             = {0};
                char  in_file_name[FILE_NAME_STR_LEN]  = {0};
                char  out_file_name[FILE_NAME_STR_LEN] = {0};
                char *in_fmt_name                      = NULL;
                char *out_fmt_name                     = NULL;

                const char *strs[3]      = {0};
                size_t      strs_lens[3] = {0};
                int         cur_str      = 0;
                int         last_str     = 0;
                const char *last_str_ptr = body_str;

                const char *params[6] = {0};

                double add_krit = 0.0f;
                const char *banner_path = NULL;
                size_t banner_path_len = 0;

                FILE *banner_fd  = NULL;
                long  banner_len = 0;
                size_t chars_readed = 0;

                //
                //  PARSING_BODY_STR
                //
             
                log_str("client->content_len: %ld.\n", client->content_len);

                log_str("client->body:\n^%s^\n", client->body);

                for (i = 0; i < client->content_len + 1; ++i) {
                    if (body_str[i] == '\n' || body_str[i] == '\0') {
                        strs[cur_str] = last_str_ptr;
                        strs_lens[cur_str] = body_str + i - last_str_ptr;

                        ++cur_str;
                        last_str = cur_str;

                        last_str_ptr = body_str + i + 1;
                    }
                }

                log_str("line: %d.\n", __LINE__);

                //
                //  PREPARING PARAMS
                //

                if (!strs[0] || !strs[1] || !strs[2]) {
                    res_str = HTTP400_RESPONSE;
                    res_str_len = strlen(res_str);
                    req->buf_allocated = 0;
                    goto write_buf;
                }
                
                memcpy(token, strs[0], strs_lens[0]);
                memcpy(in_file_name, strs[1], strs_lens[1]);
                memcpy(out_file_name, strs[2], strs_lens[2]);

                in_fmt_name = strrchr(in_file_name, '.') + 1;
                out_fmt_name = strrchr(out_file_name, '.') + 1;

                //cur_time = time(NULL);
                //cur_tm = localtime(&cur_time);

                params[0] = token;
                params[1] = in_file_name;
                params[2] = out_file_name;
                params[3] = in_fmt_name;
                params[4] = out_fmt_name;

                log_str("line: %d.\n", __LINE__);

                //
                //  DB_REQUEST
                //

                conn = PQconnectdb(g_dbconn);
                if (PQstatus(conn) != CONNECTION_OK) {
                    log_err("[!][on_read] Failed to PQconnect.\n");
                    goto err_add_history_500;
                }

                log_str("line: %d.\n", __LINE__);

                res = PQexecParams(conn, INSERT_INTO_HISTORY_REQ, 5, NULL,
                                   (const char * const*)params,
                                   NULL, NULL, 0);
                if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                    log_err("[!][on_read] Failed to PQexecParams "
                        "ADD_TO_THE_HISTORY_URL.\n");
                    goto err_add_history_500_close_db;
                }

                PQclear(res);

                log_str("line: %d.\n", __LINE__);

                res = PQexecParams(conn, GET_ADD_KRIT_REQ, 1, NULL,
                                   (const char * const*)params,
                                   NULL, NULL, 0);
                if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                    log_err("[!][on_read] Failed to PQexecParams "
                        "GET_ADD_KRIT_REQ.\n");
                    goto err_add_history_500_close_db;
                }

                if (PQntuples(res) == 0) {
                    log_err("[!][on_read] Failed to PQexecParams "
                        "GET_ADD_KRIT_REQ.\n");
                    goto err_add_history_500_close_db;
                }

                log_str("line: %d.\n", __LINE__);

                
                add_krit = strtod(PQgetvalue(res, 0, 0), NULL) / 3 * 100;

                PQclear(res);
                PQfinish(conn);

                if (add_krit <= 25.0f) {
                    banner_path = BANNER_PATH00;
                    banner_path_len = BANNER_PATH00_LEN;
                }
                else if (add_krit <= 50.0f) {
                    banner_path = BANNER_PATH01;
                    banner_path_len = BANNER_PATH01_LEN;
                }
                else if (add_krit <= 75.0f) {
                    banner_path = BANNER_PATH02;
                    banner_path_len = BANNER_PATH02_LEN;
                }
                else { /* add_krit <= 100.0f */
                    banner_path = BANNER_PATH03;
                    banner_path_len = BANNER_PATH03_LEN;
                }

                log_str("line: %d.\n", __LINE__);

                //
                //  READING_FD_DATA
                //

                banner_fd = fopen(banner_path, "rb");
                if (!banner_fd) {
                    log_err("[!][on_read] Failed to fopen.\n");
                    goto err_add_history_500;
                }

                ret = fseek(banner_fd, 0, SEEK_END);
                if (res < 0) {
                    log_err("[!][on_read] Failed to fseek.\n");
                    goto err_add_history_500_close_fd;
                }

                banner_len = ftell(banner_fd);
                if (banner_len == -1L) {
                    log_err("[!][on_read] Failed to ftell.\n");
                    goto err_add_history_500_close_fd;
                }

                ret = fseek(banner_fd, 0, SEEK_SET);
                if (ret < 0) {
                    log_err("[!][on_read] Failed to fseek.\n");
                    goto err_add_history_500_close_fd;
                }

                log_str("line: %d.\n", __LINE__);
                

                //
                //  FORMATING_RESPONSE_STR
                //

                res_str_len = snprintf(NULL, 0, //res_str, sizeof(buf_str)
                    JPEG_HEADER, HTTP_201_STR,
                    DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                    MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                    cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec, banner_len);

                res_str = (char*)malloc((res_str_len + banner_len + 1) * sizeof(char));
                if (!res_str) {
                    log_err("[!][on_read] Failed to malloc.\n");
                    goto err_add_history_500_close_fd;
                }

                log_str("[*][on_read] res_str_len: %zu, banner_len: %zu.\n",
                    res_str_len, banner_len);

                res_str_len = snprintf(res_str, res_str_len + 1, /* +1 cause of null-terminater pasted by snprintf*/
                    JPEG_HEADER, HTTP_201_STR,
                    DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                    MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                    cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec, banner_len);

                log_str("[*][on_read] res_str:\n*%s*\n"
                    "res_str_len: %zu.\nres_str + res_str_len:\n*%s*\n"
                    "strlen(res_str): %zu.\n",
                    res_str, res_str_len, res_str + res_str_len, strlen(res_str));

                chars_readed = fread(res_str + res_str_len,
                    sizeof(unsigned char), banner_len, banner_fd);
                if (chars_readed != banner_len) {
                    log_err("[!][on_read] Failed to fread.\n");
                    goto err_add_history_500_free_str;
                }

                fclose(banner_fd);

                res_str_len += banner_len;
                res_str[res_str_len] = '\0';

                //log_str("\nresponse:\n^");
                //for (size_t i = 0; i < res_str_len; ++i) {
                //    if (i >= 143) {
                //        log_str("%02x ", res_str[i]);
                //    }
                //    else {
                //        log_str("%c", res_str[i]);
                //    }
                //}
                //log_str("^\nend of response:\n\n");

                //FILE *new_fd = fopen("new_file03.jpg", "wb");

                //fwrite(res_str + res_str_len - banner_len,
                //    sizeof(unsigned char), banner_len, new_fd);

                //fclose(new_fd);

                // Записать в res_str заголовок,
                // затем fread данные баннера в него после заголовков (дать указатель)

                goto write_buf;
            
                /* on_errors */
                err_add_history_close_db:

                PQclear(res);
                PQfinish(conn);                

                err_add_history_format_response:

                res_str_len = snprintf(NULL, 0,
                    header, status,
                    DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                    MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                    cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec, 0);

                res_str = (char*)malloc((res_str_len + 1) * sizeof(char));
                if (!res_str) {
                    log_err("[!][on_read] Failed to malloc.\n");
                    goto err_add_history_500;
                }

                res_str_len = snprintf(res_str, res_str_len,
                    header, status,
                    DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday,
                    MONTH_NAMES[cur_tm->tm_mon], cur_tm->tm_year + 1900,
                    cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec, 0);

                res_str[res_str_len] = '\0';

                goto write_buf;

                /* on_status_500 */            
                err_add_history_500_close_db:
                PQclear(res);
                PQfinish(conn);

                err_add_history_500_free_str:
                free(res_str);

                err_add_history_500_close_fd:
                fclose(banner_fd);

                err_add_history_500:
                res_str = HTTP500_RESPONSE;
                res_str_len = strlen(res_str);
                req->buf_allocated = 0;
            }
            else {
                res_str = HTTP404_RESPONSE;
                res_str_len = strlen(res_str);
                req->buf_allocated = 0;

                log_str("[?][on_read] unknown http utl/target.\n");
            }
        }
        else { /* any other method */
            res_str = HTTP404_RESPONSE;
            res_str_len = strlen(res_str);
            req->buf_allocated = 0;

            log_str("[?][on_read] unknown http method.\n");
        }
    }
    else {
        res_str = HTTP404_RESPONSE;
        res_str_len = strlen(res_str);
        req->buf_allocated = 0;

        log_err("[!] [on_read] Failed to llhttp_execute.\n");
        goto free_req;
    }

write_buf:

    log_str("[*][on_read] response:\n^%s^\n\n", res_str);

    req->buf = uv_buf_init(res_str, res_str_len);

    ret = uv_write((uv_write_t*)req, stream, &req->buf, 1, on_write);
    if (ret < 0) {
        log_err("[!] [on_read] Failed to uv_write.\n");
        goto free_res_str;
    }

    free(buf->base);

    return;

free_res_str:
    if (req->buf_allocated) {
        free(res_str);
    }

free_req:    
    free(req);

close:
    free(buf->base);

    uv_close((uv_handle_t*)&client->handle, on_close);
}

void on_write(uv_write_t *req, int status)
{
    if (status < 0) {
        log_err("[!] [on_write] Failed. status < 0.\n");
        return;
    }

    write_req_t   *wr       = NULL;
    uv_shutdown_t *shutdown = NULL;
    int            ret      = 0;

    wr = (write_req_t*)req;
    wr->client->handle.data = wr;

    shutdown = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
    if (!shutdown) {
        log_err("[!] [on_write] Failed to malloc shutdown.\n");
        goto free_wr;
    }

    shutdown->data = wr;

    ret = uv_shutdown(shutdown, (uv_stream_t*)wr->client, on_shutdown);
    if (ret < 0) {
        log_err("[!] [on_write] Failed to uv_shutdown.\n");
        goto free_shutdown;
    }

    return;

free_shutdown:
    free(shutdown);

free_wr:
    uv_close((uv_handle_t*)wr->client, on_close);

    if (wr->buf_allocated) {
        free(wr->buf.base);
    }
    free(wr);
}

void on_close(uv_handle_t *handle)
{
    log_str("[*] [on_close] freeing client tcp.\n");

    client_t *client = (client_t*)handle;

    //free(client->url);
    free(client->body);
    free(client);
}

void on_shutdown(uv_shutdown_t *req, int status)
{
    if (status < 0) {
        log_err("[!] [on_shutdown] Failed. status < 0.\n");
    }    

    write_req_t *wr = (write_req_t*)req->data;

    log_str("[*][on_shutdown] closing.\n");
    
    uv_close((uv_handle_t*)wr->client, on_close);

    log_str("[*][on_shutdown] freeing wr\'s data.\n");

    if (wr->buf_allocated) {
        log_str("[*][on_shutdown] freeing wr->buf.base.\n");
        free(wr->buf.base);
    }

    log_str("[*][on_shutdown] freeing wr.\n");
    
    free(wr);

    log_str("[*][on_shutdown] freeing req.\n");

    free(req);
    //free(wr->buf.base);
    //free(wr);    
    //free(req);

    log_str("[*][on_shutdown] end.\n");
}

size_t payload_source(void *ptr, size_t size, size_t nmemb, void *udata)
{
    const char *data = (const char*)udata;
    size_t len = strlen(data);

    if (len == 0) {
        log_err("[!][payload_source] udata len == 0.\n");
        return 0;
    }

    memcpy(ptr, data, len);

    return len;
}