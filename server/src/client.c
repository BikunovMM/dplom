#define UTILS_IMPLEMENTATION
#include "client.h"

/* global_variables */
uv_loop_t         *g_loop    = NULL;
llhttp_settings_t  g_settings = {0};

/* header's_contants*/
const char *DAY_NAMES[] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};
const char *MONTH_NAMES[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
    "Aug", "Sep", "Oct", "Nov", "Dec"
};

int main()
{
    struct          sockaddr_in addr = {0};
    char            ip[IP_MAX_LEN]   = SERVER_IP;
    unsigned short  port             = SERVER_PORT;
    int             ret              = 0;

    ret = init_and_run(ip, port);
    if (ret < 0) {
        log_err("[!][main] Failed to init_and_run.\n");        
    }

    return (ret < 0 ? 1 : 0);
}

int init_and_run(const char *sip, unsigned short sport)
{
    client_t *client           = NULL;
    struct    sockaddr_in addr = {0};
    int       ret              = -1;

    client = (client_t*)malloc(sizeof(client_t));
    if (!client) {
        log_err("[!][init_and_run] Failed to malloc client.\n");
        return ret;
    }

    g_loop = uv_default_loop();
    if (!g_loop) {
        log_err("[!] [init_and_run] Failed to uv_default_loop.\n");
        goto free_client;
    }

    llhttp_settings_init(&g_settings);
    g_settings.on_status           = on_status;
    g_settings.on_header_field     = on_header_field;
    g_settings.on_header_value     = on_header_value;
    g_settings.on_body             = on_body;
    g_settings.on_headers_complete = on_headers_complete;

    ret = uv_tcp_init(g_loop, &client->socket);
    if (ret < 0) {
        log_err("[!] [init_and_run] Failed to uv_tcp_init.\n");
        goto cleanup;
    }

    ret = uv_ip4_addr(sip, sport, &addr);
    if (ret < 0) {
        log_err("[!] [init_and_run] Failed to uv_ip4_addr.\n");
        goto cleanup;
    }

    client->connect.data = client;

    ret = uv_tcp_connect(&client->connect, &client->socket,
                         (const struct sockaddr*)&addr, on_conn);
    if (ret < 0) {
        log_err("[!] [init_and_run] Failed to uv_tcp_connect.\n");
        goto cleanup;
    }

    ret = uv_run(g_loop, UV_RUN_DEFAULT);
    if (ret < 0) {
        log_err("[!] [init_and_run] Failed to uv_tcp_connect.\n");
    }

    log_str("[*][main] connection closed.\n");

cleanup:
    uv_loop_close(g_loop);

free_client:
    free(client);

    return ret;
}

int on_status(llhttp_t *parser, const char *at, size_t len)
{
    (void)parser;
    (void)at;
    (void)len;
/*
    client_t *client = NULL;

    client->status = (char*)malloc(len * sizeof(char));
    if (!client->status) {
        log_err("[!] [on_status] Failed malloc client->status.\n");
        return -1;
    }

    memcpy(client->status, at, len);

    char code_str[4] = {0};

    memcpy(code_str, strchr(client->status, ' '), 3);

    log_str("[*][on_status] status: ^%s^(len: %zu).\n"
        "code: %s.\n", client->status, len, code_str);
*/
    return 0;
}

int on_header_field(llhttp_t *parser, const char *at, size_t len)
{
    (void)parser;
    (void)at;
    (void)len;

    client_t *client = (client_t*)parser->data;

    if (client->content_len == 0 && strncmp(at, "Content-Length", len) == 0) {        
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

    log_str("[*][on_body] line: %d.\n", __LINE__);

    client_t *client = (client_t*)parser->data;

    if (!client->body) {
        client->body = (char*)malloc(client->content_len * sizeof(char));
        if (!client->body) {
            log_err("[!] [on_status] Failed malloc client->body.\n");
            return -1;
        }        
    }

    memcpy(client->body + client->body_len, at, len);    

    client->body_len += len;

    log_str("[*][on_body] body (len: %zu, body_len: %zu).\n", len, client->body_len);

    return 0;
}

int on_headers_complete(llhttp_t *parser)
{
    (void)parser;

    log_str("[*][on_headers_complete] status_code: %d.\n",
        parser->status_code);
}

void on_conn(uv_connect_t *req, int status)
{    
    if (status < 0) {
        log_err("[!] [on_conn] Failed. Status < 0.\n");
        return;
    }

    client_t *client = (client_t*)req->data;
    time_t cur_time = {0};
    struct tm *cur_tm = NULL;
    char *ulogin = "Misha2006";
    char *upassword = "12345";
    char *uemail = "misha2006@gmail.com";
    char *token = "ee7c0061-0349-4466-9355-aad0783c8af2";
    char *in_file_name = "C:\\Users\\m933783\\Desktop\\img00.png";
    char *out_file_name = "C:\\Users\\m933783\\Desktop\\img01.ico";
    char buf_str[MAX_REG_REQ_LEN] = {0};

    log_str("[*][on_conn]\n");

    cur_time = time(NULL);
    cur_tm = localtime(&cur_time);

    client->url = ADD_TO_THE_HISTORY_URL;
    client->content_len = 0;
    client->body_len = 0;
    
    snprintf(buf_str, sizeof(buf_str),
        HTTP_REQ_HEAD ADD_TO_THE_HISTORY_STR,
        HTTPPOST, client->url,
        DAY_NAMES[cur_tm->tm_wday], cur_tm->tm_mday, MONTH_NAMES[cur_tm->tm_mon],
        cur_tm->tm_year + 1900, cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec,
        strlen(token) + strlen(in_file_name) + strlen(out_file_name) + 2,
        token, in_file_name, out_file_name);

    log_str("[*][on_conn] str:\n^%s^(len: %zu).\n\n", buf_str, strlen(buf_str));

    client->buf = uv_buf_init(buf_str, strlen(buf_str));
    client->wr.data = client;

    llhttp_init(&client->parser, HTTP_RESPONSE, &g_settings);
    client->parser.data = client;
    
    uv_write(&client->wr, (uv_stream_t*)&client->socket, &client->buf, 1, on_write);
}

void on_write(uv_write_t *req, int status)
{
    log_str("[*][on_write]\n");

    if (status < 0) {
        log_err("[!] [on_write_end] Failed. Status < 0.\n");
        return;
    }

    client_t *client = (client_t*)req->data;
    client->buf.base = NULL;
    client->buf.len = 0;

    uv_read_start((uv_stream_t*)client, on_buf_alloc, on_read);
}

void on_buf_alloc(uv_handle_t *handle, size_t sug_size, uv_buf_t *buf)
{
    (void)handle;
    (void)buf;

    log_str("[*][on_buf_alloc]\n");

    if (sug_size == 0) {
        log_err("[!] [on_buf_alloc] Failed. sug_size == 0.\n");
        buf->len = 0;
        return;
    }

    client_t *client = (client_t*)handle;

    //if (strcmp(client->url, ADD_TO_THE_HISTORY_URL) == 0) {
    //    sug_size = ADD_TO_THE_HISTORY_BUF_LEN;
    //}

    log_str("\nsug_size: %zu, buf.len: %zu, buf.base: %s.\n\n",
        sug_size, client->buf.len, client->buf.base ? "ok" : "NULL");

    if (!client->buf.base || client->buf.len == 0) {
        log_str("[*][on_buf_alloc] allocationg new client->buf.base.\n");        

        client->buf.base = (char*)calloc(sug_size, sizeof(char));
        if (!client->buf.base) {
            log_err("[!] [on_buf_alloc] Failed to malloc client->buf.base.\n");
            client->buf.len = 0;
            return;
        }

        log_str("ok\n");

        client->buf.len = sug_size;

        log_str("end of allocating.\n");
    }
    else if (client->buf.len < sug_size) {
        log_str("[*][on_buf_alloc] reallocationg client->buf.base.\n");
        
        free(client->buf.base);

        client->buf.base = (char*)malloc(sug_size * sizeof(char));
        if (!client->buf.base) {
            log_err("[!] [on_buf_alloc] Failed to malloc client->buf.base.\n");
            client->buf.len = 0;
            return;
        }

        log_str("ok\n");

        client->buf.len = sug_size;

        log_str("end of allocating.\n");
    }

    buf->base = client->buf.base;
    buf->len = client->buf.len;
}

void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t* buf)
{
    log_str("[*][on_read]\n");

    if (nread <= 0) {
        log_err("[!] [on_read] Failed. nread <= 0.\n");
        return;
    }

    //uv_shutdown_t *shutdown = NULL;
    client_t          *client = NULL;
    llhttp_t          *parser = NULL;
    enum llhttp_errno  err = 0;
    int                ret = 0;

    client = (client_t*)stream;
    parser = &client->parser;

    err = llhttp_execute(parser, buf->base, nread);
    if (err == HPE_OK &&
        parser->type == HTTP_RESPONSE)
    {
        if (client->body_len < client->content_len) {
            log_str("[*][on_read] waiting for other packeges.\n");
            return;
        }

        if (strcmp(client->url, REG_USER_URL) == 0) {
            log_str("[*][on_read] REG_USER_URL, token: ^%s^.\n", client->body);
        }
        else if (strcmp(client->url, LOGIN_USER_URL) == 0) {
            log_str("[*][on_read] LOGIN_USER_URL, token: ^%s^.\n", client->body);
        }
        else if (strcmp(client->url, VALIDATE_TOKEN_URL) == 0) {
            log_str("[*][on_read] VALIDATE_TOKEN_URL, token: ^%s^.\n", client->body);
        }
        else if (strcmp(client->url, ADD_TO_THE_HISTORY_URL) == 0) {
            FILE *img_fd = NULL;
            size_t chars_wrote = 0;

            img_fd = fopen("img00.jpg", "wb");
            if (!img_fd) {
                log_err("[!][on_read] Failed to fopen!\n");
                goto on_read_end;
            }

            log_str("[*][on_read] nread: %zu, "
                "client->content_len: %ld, header_len: %lld.\n",
                nread, client->content_len, nread - client->content_len);

            chars_wrote = fwrite(client->body, 1,
                client->content_len, img_fd);
            log_str("[!][on_read] chars_wrote: %zu.\n", chars_wrote);

            fclose(img_fd);

            //log_str("\nresponse:\n^");
            //for (size_t i = 0; i < nread; ++i) {
            //    if (i >= 143) {
            //        if (!(buf->base + i)) {
            //            log_err("\n! null!\n");
            //            break;
            //        }
            //        log_str("%02x ", buf->base[i]);
            //    }
            //    else {
            //        log_str("%c", buf->base[i]);
            //    }
            //}
            //log_str("^\nend of response:\n\n");
        }
        else {
            log_str("[!][on_read] status code != 200 OK.\n");
        }
    }
    else {
        log_err("[!][on_read] Failed to llhttp_execute.\n");        
    }

    log_str("[*][on_read] response:\n^%s^\n\n", buf->base);

/*
    if (*client->body == *VALID_TOKEN) {
        log_str("[*][on_read] the token is valid!\n");
    }
    else {
        log_str("[*][on_read] the token is unvalid!!!\n");
    }  
*/

on_read_end:

    uv_close((uv_handle_t*)&client->socket, on_close);
}

void on_close(uv_handle_t *handle)
{
    log_str("[*][on_close]\n");

    client_t *client = (client_t*)handle;

    free(client->body);
    free(client->buf.base);
}
