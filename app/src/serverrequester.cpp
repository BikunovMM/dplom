#include "serverrequester.h"

#if 1
uv_loop_t         *g_loop     = nullptr;
llhttp_settings_t  g_settings = {};

/* header's_contants*/
const char *ServerRequester::DAY_NAMES[] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};
const char *ServerRequester::MONTH_NAMES[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
    "Aug", "Sep", "Oct", "Nov", "Dec"
};

int initAndRun(const char *sip, unsigned short sport,
               ServerRequester::Client *client);

void onRegUserConn(uv_connect_t *req, int status);
void onRegUserRead(uv_stream_t *stream, ssize_t nread, const uv_buf_t* buf);

void onLogUserConn(uv_connect_t *req, int status);
void onLogUserRead(uv_stream_t *stream, ssize_t nread, const uv_buf_t* buf);

void onAddToHistoryConn(uv_connect_t *req, int status);
void onAddToHistoryRead(uv_stream_t *stream, ssize_t nread, const uv_buf_t* buf);

void onValTokenConn(uv_connect_t *req, int status);
void onValTokenRead(uv_stream_t *stream, ssize_t nread, const uv_buf_t* buf);


int onHeaderField(llhttp_t *parser, const char *at, size_t len);
int onHeaderValue(llhttp_t *parser, const char *at, size_t len);
int onBody(llhttp_t *parser, const char *at, size_t len);
void onWrite(uv_write_t *req, int status);
void onBufAlloc(uv_handle_t *handle, size_t sug_size, uv_buf_t *buf);
void onClose(uv_handle_t *handle);
void onCloseWithoutFreeingBody(uv_handle_t *handle);


int ServerRequester::registerUser(const char *login,
                                  const char *password,
                                  const char *email,
                                  char *token)
{
    Client          client = {};
    char           *ip     = SERVER_IP;
    unsigned short  port   = SERVER_PORT;
    int             ret    = 0;

    client.login    = login;
    client.password = password;
    client.email    = (char*)email;
    client.op       = Operations::REG_USER;
    client.on_conn  = onRegUserConn;
    client.on_write = onWrite;
    client.on_read  = onRegUserRead;
    client.token    = token;

    ret = initAndRun(ip, port, &client);
    if (ret < 0) {
        log_err("[!][registerUser] Failed to init_and_run.\n");
    }

    if (!client.token || client.token[0] == '\0') {
        log_err("[!][registerUser] client.token is NULL or empty.\n");
        return -1;
    }

    log_str("[*][registerUser]\n\tclient->token: ^%s^;\n"
            "\ttoken: ^%s^.\n", client.token, token);

    return client.parser.status_code;
}

int ServerRequester::loginUser(const char *login,
                               const char *password,
                               char *token,
                               char *email,
                               char *created_at)
{
    Client          client = {};
    char           *ip     = SERVER_IP;
    unsigned short  port   = SERVER_PORT;
    int             ret    = 0;

    client.login    = login;
    client.password = password;
    client.token    = token;
    client.email    = email;
    client.created_at = created_at;
    client.op       = Operations::LOG_USER;
    client.on_conn  = onLogUserConn;
    client.on_write = onWrite;
    client.on_read  = onLogUserRead;    

    ret = initAndRun(ip, port, &client);
    if (ret < 0) {
        log_err("[!][loginUser] Failed to init_and_run.\n");
    }

    if (!client.token || client.token[0] == '\0') {
        log_err("[!][loginUser] client.token is NULL or empty.\n");
        return -1;
    }

    log_str("[*][loginUser]\n\tclient->token: ^%s^;\n"
            "\ttoken: ^%s^.\n", client.token, token);

    return client.parser.status_code;
}

int ServerRequester::addToTheHistory(char          *token,
                                     const char    *inpath,
                                     const char    *outpath,
                                     unsigned char **banner_data,
                                     size_t        *banner_data_len)
{
    (void)token;
    (void)inpath;
    (void)outpath;
    (void)banner_data;
    (void)banner_data_len;

    Client          client = {};
    char           *ip     = SERVER_IP;
    unsigned short  port   = SERVER_PORT;
    int             ret    = 0;

    log_str("[*][addToTheHistory] inpath: %s, outpath: %s, "
            "token: %s.\n", inpath, outpath, token);

    client.inpath = inpath;
    client.outpath = outpath;
    client.token = token;
    client.op = Operations::INS_HISTORY;
    client.on_conn  = onAddToHistoryConn;
    client.on_write = onWrite;
    client.on_read  = onAddToHistoryRead;

    log_str("[*] inpath: %s, outpath: %s.\n",
            client.inpath, client.outpath);

    log_str("[*][addToTheHistory] client->body_len: %zu.\n", client.body_len);

    ret = initAndRun(ip, port, &client);
    if (ret < 0) {
        log_err("[!][addToTheHistory] Failed to init_and_run.\n");
    }

    log_str("[*][addToTheHistory] post initAndRun.\n");

    *banner_data = (unsigned char*)client.body;
    *banner_data_len = client.content_len;

    log_str("[*][addToTheHistory] end.\n");

    return client.parser.status_code;
}

int ServerRequester::validateToken(char *token)
{
    Client          client = {};
    char           *ip     = SERVER_IP;
    unsigned short  port   = SERVER_PORT;
    int             ret    = 0;

    client.token = token;
    client.op = Operations::VAL_TOKEN;
    client.on_conn  = onValTokenConn;
    client.on_write = onWrite;
    client.on_read  = onValTokenRead;    

    ret = initAndRun(ip, port, &client);
    if (ret < 0) {
        log_err("[!][main] Failed to init_and_run.\n");
    }

    return client.parser.status_code;
}

int initAndRun(const char *sip, unsigned short sport,
               ServerRequester::Client *client)
{
    struct    sockaddr_in addr = {};
    int       ret              = -1;

    g_loop = uv_default_loop();
    if (!g_loop) {
        log_err("[!] [init_and_run] Failed to uv_default_loop.\n");
        return ret;
    }

    llhttp_settings_init(&g_settings);
    g_settings.on_header_field = onHeaderField;
    g_settings.on_header_value = onHeaderValue;
    g_settings.on_body = onBody;

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
                         (const struct sockaddr*)&addr,
                         client->on_conn);
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

    return ret;
}

int onHeaderField(llhttp_t *parser, const char *at, size_t len)
{
    (void)parser;
    (void)at;
    (void)len;

    ServerRequester::Client *client =
        (ServerRequester::Client*)parser->data;

    if (client->content_len == 0 && strncmp(at, "Content-Length", len) == 0) {        
        client->content_len = -1;
    }

    return 0;
}

int onHeaderValue(llhttp_t *parser, const char *at, size_t len)
{
    (void)parser;
    (void)at;
    (void)len;

    ServerRequester::Client *client =
        (ServerRequester::Client*)parser->data;

    if (client->content_len == -1) {
        char buf[10] = {0};

        memcpy(buf, at, len);
        client->content_len = strtol(buf, NULL, 10);
    }

    return 0;
}

int onBody(llhttp_t *parser, const char *at, size_t len)
{
    (void)parser;
    (void)at;
    (void)len;

    log_str("[*][on_body] line: %d.\n", __LINE__);

    ServerRequester::Client *client =
        (ServerRequester::Client*)parser->data;

    if (!client->body) {
        log_str("\n\n[*][onBody] allocating body!\n\n");
        client->body = (char*)malloc(client->content_len * sizeof(char));
        if (!client->body) {
            log_err("[!] [on_status] Failed malloc client->body.\n");
            return -1;
        }

        log_str("[*][onBody] len: %zu.\n", client->content_len);

        client->body_len = 0;
    }

    log_str("ok1, client->body_len: %zu.\n", client->body_len);

    memcpy(client->body + client->body_len, at, len);

    client->body_len += len;

    log_str("[*][on_body] end_line: %d.\n", __LINE__);

    //log_str("[*][on_body] body: ^%s^(len: %zu).\n", client->body, len);

    return 0;
}

void onRegUserConn(uv_connect_t *req, int status)
{
    if (status < 0) {
        log_err("[!] [on_conn] Failed. Status < 0.\n");
        return;
    }

    ServerRequester::Client *client = (ServerRequester::Client*)req->data;
    time_t     cur_time                       = {};
    struct tm *cur_tm                         = NULL;
    char       buf_str[ServerRequester::MAX_REG_REQ_LEN]       = {};
    size_t     buf_len                        = 0;

    log_str("[*][onRegUserConn]\n");

    cur_time = time(NULL);
    cur_tm = localtime(&cur_time);

    client->url = ServerRequester::REG_USER_URL;

    buf_len = snprintf(buf_str, sizeof(buf_str),
            HTTP_REQ_HEAD
            REG_USER_STR,
            ServerRequester::HTTPPOST, client->url,
            ServerRequester::DAY_NAMES[cur_tm->tm_wday],
            cur_tm->tm_mday,
            ServerRequester::MONTH_NAMES[cur_tm->tm_mon],
            cur_tm->tm_year + 1900, cur_tm->tm_hour,
            cur_tm->tm_min, cur_tm->tm_sec,
            strlen(client->login) + strlen(client->password)
            + strlen(client->email) + 2,
            client->login, client->password,
            client->email);

    log_str("[*][onRegUserConn] request:\n^%s^(len: %zu).\n\n",
            buf_str, buf_len);

    client->buf = uv_buf_init(buf_str, buf_len);
    client->wr.data = client;

    llhttp_init(&client->parser, HTTP_RESPONSE, &g_settings);
    client->parser.data = client;

    uv_write(&client->wr, (uv_stream_t*)&client->socket,
             &client->buf, 1, client->on_write);
}

void onLogUserConn(uv_connect_t *req, int status)
{
    if (status < 0) {
        log_err("[!] [on_conn] Failed. Status < 0.\n");
        return;
    }

    ServerRequester::Client *client = (ServerRequester::Client*)req->data;
    time_t cur_time = {};
    struct tm *cur_tm = NULL;
    char buf_str[ServerRequester::MAX_REG_REQ_LEN] = {};
    size_t buf_len = 0;

    log_str("[*][onLogUserConn]\n");

    cur_time = time(NULL);
    cur_tm = localtime(&cur_time);

    client->url = ServerRequester::LOGIN_USER_URL;

    buf_len = snprintf(buf_str, sizeof(buf_str),
             HTTP_REQ_HEAD
             LOGIN_USER_STR,
             ServerRequester::HTTPGET, client->url,
             ServerRequester::DAY_NAMES[cur_tm->tm_wday],
             cur_tm->tm_mday,
             ServerRequester::MONTH_NAMES[cur_tm->tm_mon],
             cur_tm->tm_year + 1900, cur_tm->tm_hour,
             cur_tm->tm_min, cur_tm->tm_sec,
             strlen(client->login) + strlen(client->password) + 1,
             client->login, client->password);

    log_str("[*][onLogUserConn] str:\n^%s^(len: %zu).\n\n",
            buf_str, buf_len);

    client->buf = uv_buf_init(buf_str, buf_len);
    client->wr.data = client;

    llhttp_init(&client->parser, HTTP_RESPONSE, &g_settings);
    client->parser.data = client;

    uv_write(&client->wr, (uv_stream_t*)&client->socket,
        &client->buf, 1, client->on_write);
}

void onAddToHistoryConn(uv_connect_t *req, int status)
{
    if (status < 0) {
        log_err("[!] [on_conn] Failed. Status < 0.\n");
        return;
    }

    ServerRequester::Client *client = (ServerRequester::Client*)req->data;
    time_t cur_time = {};
    struct tm *cur_tm = NULL;
    char buf_str[ServerRequester::MAX_REG_REQ_LEN] = {};
    size_t buf_len = 0;

    log_str("[*][onAddToHistoryConn] begining.\n");

    cur_time = time(NULL);
    cur_tm = localtime(&cur_time);

    client->url = ServerRequester::ADD_TO_THE_HISTORY_URL;

    log_str("[*][onAddToHistoryConn] pre snprintf.\n");

    log_str("[*][onAddToHistoryConn] inpath: %s, outpath: %s.\n",
            client->inpath, client->outpath);

    buf_len = snprintf(buf_str, sizeof(buf_str),
             HTTP_REQ_HEAD
             ADD_TO_THE_HISTORY_STR,
             ServerRequester::HTTPPOST, client->url,
             ServerRequester::DAY_NAMES[cur_tm->tm_wday],
             cur_tm->tm_mday,
             ServerRequester::MONTH_NAMES[cur_tm->tm_mon],
             cur_tm->tm_year + 1900, cur_tm->tm_hour,
             cur_tm->tm_min, cur_tm->tm_sec,
             strlen(client->token) + strlen(client->inpath)
             + strlen(client->outpath) + 2,
             client->token, client->inpath, client->outpath);
    log_str("[*][onAddHistoryConn] post snprintf.\n");

    log_str("[*][onAddToHistoryConn] str:\n^%s^(len: %zu).\n\n",
            buf_str, buf_len);

    client->buf = uv_buf_init(buf_str, buf_len);
    client->wr.data = client;

    llhttp_init(&client->parser, HTTP_RESPONSE, &g_settings);
    client->parser.data = client;

    uv_write(&client->wr, (uv_stream_t*)&client->socket,
        &client->buf, 1, client->on_write);
}

void onValTokenConn(uv_connect_t *req, int status)
{
    if (status < 0) {
        log_err("[!] [onValTokenConn] Failed. Status < 0.\n");
        return;
    }

    ServerRequester::Client *client = (ServerRequester::Client*)req->data;
    time_t cur_time = {};
    struct tm *cur_tm = NULL;
    char buf_str[ServerRequester::MAX_REG_REQ_LEN] = {};
    size_t buf_len = 0;

    log_str("[*][onValTokenConn]\n");

    cur_time = time(NULL);
    cur_tm = localtime(&cur_time);

    client->url = ServerRequester::VALIDATE_TOKEN_URL;

    buf_len = snprintf(buf_str, sizeof(buf_str),
                       HTTP_REQ_HEAD
                       VALIDATE_TOKEN_STR,
                       ServerRequester::HTTPGET, client->url,
                       ServerRequester::DAY_NAMES[cur_tm->tm_wday],
                       cur_tm->tm_mday,
                       ServerRequester::MONTH_NAMES[cur_tm->tm_mon],
                       cur_tm->tm_year + 1900, cur_tm->tm_hour,
                       cur_tm->tm_min, cur_tm->tm_sec,
                       strlen(client->token),
                       client->token);

    log_str("[*][onValTokenConn] str:\n^%s^(len: %zu).\n\n",
            buf_str, buf_len);

    client->buf = uv_buf_init(buf_str, buf_len);
    client->wr.data = client;

    llhttp_init(&client->parser, HTTP_RESPONSE, &g_settings);
    client->parser.data = client;

    uv_write(&client->wr, (uv_stream_t*)&client->socket,
             &client->buf, 1, client->on_write);
}

void onWrite(uv_write_t *req, int status)
{
    log_str("[*][onWrite]\n");

    if (status < 0) {
        log_err("[!] [onWrite_end] Failed. Status < 0.\n");
        return;
    }

    ServerRequester::Client *client = (ServerRequester::Client*)req->data;
    client->buf.base = NULL; /* maybe alloc in on_conn buf on the heap??? */
    client->buf.len = 0;

    uv_read_start((uv_stream_t*)client, onBufAlloc, client->on_read);
}

void onBufAlloc(uv_handle_t *handle,
                size_t sug_size,
                uv_buf_t *buf)
{
    (void)handle;
    (void)buf;

    log_str("[*][onBufAlloc]\n");

    if (sug_size == 0) {
        log_err("[!] [onBufAlloc] Failed. sug_size == 0.\n");
        buf->len = 0;
        return;
    }

    ServerRequester::Client *client = (ServerRequester::Client*)handle;

    if (!client->buf.base || client->buf.len == 0) {
        log_str("[*][onBufAlloc] calloc new buf.\n");
        client->buf.base = (char*)calloc(sug_size, sizeof(char));
        if (!client->buf.base) {
            log_err("[!] [on_buf_alloc] Failed to malloc client->buf.base.\n");
            client->buf.len = 0;
            return;
        }

        client->buf.len = sug_size;
    }
    else if (client->buf.len < sug_size) {
        log_str("[*][onBufAlloc] reallocating new buf.\n");
        free(client->buf.base);

        client->buf.base = (char*)malloc(sug_size * sizeof(char));
        if (!client->buf.base) {
            log_err("[!] [on_buf_alloc] Failed to malloc client->buf.base.\n");
            client->buf.len = 0;
            return;
        }

        log_str("ok\n");

        client->buf.len = sug_size;
    }

    buf->base = client->buf.base;
    buf->len = client->buf.len;
}

void onRegUserRead(uv_stream_t *stream, ssize_t nread,
                   const uv_buf_t* buf)
{
    log_str("[*][onRegUserRead]\n");

    if (nread <= 0) {
        log_err("[!] [onRegUserRead] Failed. nread <= 0.\n");
        return;
    }

    ServerRequester::Client            *client = NULL;
    llhttp_t          *parser = NULL;
    enum llhttp_errno  err = llhttp_errno::HPE_OK;

    client = (ServerRequester::Client*)stream;
    parser = &client->parser;

    err = llhttp_execute(parser, buf->base, nread);
    if (err == HPE_OK &&
        parser->type == HTTP_RESPONSE)
    {
        if (client->body_len < client->content_len) {
            log_str("[*][on_read] waiting for other packeges.\n");
            return;
        }
        
        if (parser->status_code == ServerRequester::HTTP_201_INT) { /* success */
            memcpy(client->token, client->body, strlen(client->body));
        }
        else if (parser->status_code == ServerRequester::HTTP_409_INT) { /* user_already_exists */

        }
        else { /* parser->status_code == HTTP_500_INT server_error*/

        }
        log_str("[*][on_read] REG_USER_URL, token: ^%s^.\n", client->body);
    }
    else {
        log_err("[!][onRegUserRead] Failed to llhttp_execute.\n");
    }

    uv_close((uv_handle_t*)&client->socket, onClose);
}

void onLogUserRead(uv_stream_t *stream, ssize_t nread,
                   const uv_buf_t* buf)
{
    log_str("[*][onLogUserRead]\n");

    if (nread <= 0) {
        log_err("[!] [onLogUserRead] Failed. nread <= 0.\n");
        return;
    }

    ServerRequester::Client          *client = NULL;
    llhttp_t          *parser = NULL;
    enum llhttp_errno  err = llhttp_errno::HPE_OK;
    int                ret = 0;

    (void)ret;

    client = (ServerRequester::Client*)stream;
    parser = &client->parser;

    err = llhttp_execute(parser, buf->base, nread);
    if (err == HPE_OK &&
        parser->type == HTTP_RESPONSE)
    {
        if (client->body_len < client->content_len) {
            log_str("[*][on_read] waiting for other packeges.\n");
            return;
        }

        if (parser->status_code == ServerRequester::HTTP_200_INT) { /* success */
            // token, email, created_at,
            const char *strs[3]      = {0};
            size_t      strs_len[3] = {0};
            int         cur_str      = 0;
            int         last_str     = 0;
            const char *last_str_ptr = client->body;

            for (int i = 0; i < client->content_len + 1; ++i) {
                if (client->body[i] == '\n' || client->body[i] == '\0') {
                    strs[cur_str] = last_str_ptr;
                    strs_len[cur_str] = client->body + i - last_str_ptr;

                    ++cur_str;
                    last_str = cur_str;

                    last_str_ptr = client->body + i + 1;
                }
            }

            memcpy(client->token, strs[0], strs_len[0]);
            memcpy((char*)client->email, strs[1], strs_len[1]);
            memcpy(client->created_at, strs[2], strs_len[2]);
        }
        else if (parser->status_code == ServerRequester::HTTP_401_INT) { /* user doesn't exits */

        }
        else { /* parser->status_code == HTTP_500_INT server_error */

        }
        log_str("[*][on_read] LOGIN_USER_URL, token: ^%s^.\n", client->body);
    }
    else {
        log_err("[!][onLogUserRead] Failed to llhttp_execute.\n");
    }

    uv_close((uv_handle_t*)&client->socket, onClose);
}

void onAddToHistoryRead(uv_stream_t *stream, ssize_t nread, const uv_buf_t* buf)
{
    log_str("[*][onAddToHistoryRead]\n");

    if (nread <= 0) {
        log_err("[!] [onAddToHistoryRead] Failed. nread <= 0.\n");
        return;
    }

    ServerRequester::Client          *client = NULL;
    llhttp_t          *parser = NULL;
    enum llhttp_errno  err = llhttp_errno::HPE_OK;
    int                ret = 0;

    (void)ret;

    client = (ServerRequester::Client*)stream;
    parser = &client->parser;

    err = llhttp_execute(parser, buf->base, nread);
    if (err == HPE_OK &&
        parser->type == HTTP_RESPONSE)
    {
        if (client->body_len < client->content_len) {
            log_str("[*][on_read] waiting for other packeges.\n");
            return;
        }

        if (parser->status_code == ServerRequester::HTTP_201_INT) { /* success */

        }
        else { /* parser->status_code == HTTP_500_INT server_error*/

        }
    }
    else {
        log_err("[!][onAddToHistoryRead] Failed to llhttp_execute.\n");
    }

    uv_close((uv_handle_t*)&client->socket, onCloseWithoutFreeingBody);
}

void onValTokenRead(uv_stream_t *stream,
                    ssize_t nread,
                    const uv_buf_t* buf)
{
    log_str("[*][onValTokenRead]\n");

    if (nread <= 0) {
        log_err("[!] [onValTokenRead] Failed. nread <= 0.\n");
        return;
    }

    ServerRequester::Client          *client = NULL;
    llhttp_t          *parser = NULL;
    enum llhttp_errno  err = llhttp_errno::HPE_OK;
    int                ret = 0;

    (void)ret;

    client = (ServerRequester::Client*)stream;
    parser = &client->parser;

    log_str("[*][onValTokenRead] buf->base:\n^%s^\n", buf->base);    

    err = llhttp_execute(parser, buf->base, nread);
    if (err == HPE_OK &&
        parser->type == HTTP_RESPONSE)
    {
        log_str("[*][onValTokenRead] ok.\n");

        if (client->body_len < client->content_len) {
            log_str("[*][onValTokenRead] waiting for other packeges.\n");
            return;
        }

        if (parser->status_code == ServerRequester::HTTP_200_INT) {

        }
        else if (parser->status_code == ServerRequester::HTTP_401_INT) {

        }
        else {

        }        
    }
    else {
        log_err("[!][onValTokenRead] Failed to llhttp_execute.\n");
    }

    log_str("[*][onValTokenRead] err: %s.\n",
            llhttp_errno_name(err));

    uv_close((uv_handle_t*)&client->socket, onCloseWithoutFreeingBody);
}

void onClose(uv_handle_t *handle)
{
    log_str("[*][onClose]\n");

    ServerRequester::Client *client = (ServerRequester::Client*)handle;

    if (!client) {
        log_err("[!][onClose] client is null!\n");
    }
    if (!client->body) {
        log_err("[!][onClose] client->body is null!\n");
    }
    if (!client->buf.base) {
        log_err("[!][onClose] client->buf.base is null!\n");
    }
    free(client->body);
    free(client->buf.base);
}

void onCloseWithoutFreeingBody(uv_handle_t *handle)
{
    log_str("[*][onCloseWithoutFreeingBody]\n");

    if (!handle) {
        log_err("!handle.\n");
    }

    ServerRequester::Client *client = (ServerRequester::Client*)handle;

    if (!client) {
        log_err("[!][onCloseWithoutFreeingBody] client is null!\n");
    }
    if (!client->buf.base) {
        log_err("[!][onCloseWithoutFreeingBody] client->buf.base is null!\n");
    }

    log_str("onCloseWithoutFreeingBody is ok.\n");

    //if (client->buf.base) {
    //    free(client->buf.base);
    //}

    log_str("onCloseWithoutFreeingBody is done.\n");
}

#endif





#if 0

#include <boost/json/src.hpp>

int Server::Requester::register_user(const char *const  login,
                                     const char *const  password,
                                     const char *const  email,
                                     int64_t           *user_id) noexcept
{
    int res = 0;
    try {
        asio::io_context  io_ctx;
        json::object      req_json;
        std::string       req_str{};
        tcp::socket      *socket = new tcp::socket(io_ctx);

        req_json["operation"] = Server::SQL_INSERT_USER;
        req_json["login"]     = login;
        req_json["password"]  = password;
        req_json["email"]     = email;

        req_str = json::serialize(req_json);

        /* Connecting to server */
        socket->async_connect(
            tcp::endpoint(ip::make_address(Server::Requester::ip()),
            Server::Requester::port()),
            [socket, req_str, user_id, &res]
                (const boost::system::error_code& err) mutable
            {
                if (err) {
                    fprintf(stderr, "Failed to async_connect register_user.\n");
                    delete(socket);
                    res = Server::OP_STATUS::SERVER_ERR;
                    return;
                }

                /* Sending user's register data */
                asio::async_write(*socket,
                  asio::buffer(req_str),
                    [socket, user_id, &res]
                        (const boost::system::error_code& err,
                         size_t bytes) mutable
                    {
                        if (err) {
                            delete(socket);
                            res = Server::OP_STATUS::REQ_ERR;
                            return;
                        }

                        char *buffer = (char*)malloc(128 * sizeof(char));
                        if (!buffer) {
                            delete(socket);
                            res = Server::OP_STATUS::APP_ERR;
                            return;
                        }

                        /* Reading serevr's response on user's register */
                        socket->async_read_some(
                            asio::buffer(buffer, 127),
                            [socket, buffer, user_id, &res]
                            (const boost::system::error_code& err,
                                size_t bytes) mutable
                            {
                                if (err) {
                                    free(buffer);
                                    delete(socket);
                                    res = Server::OP_STATUS::REQ_ERR;
                                    return;
                                }

                                buffer[bytes] = '\0';
                                json::object res_json = json::parse(buffer).as_object();

                                *user_id = res_json["iduser"].as_int64();

                                /* cleanup */
                                free(buffer);
                                delete(socket);

                                res = (int)res_json["status"].as_int64();
                            }
                        );
                    }
                );
            }
        );

        io_ctx.run();
    }
    catch (std::exception& e) {
        fprintf(stderr, "Failed to register user. Error: %s.\n",
                e.what());
        return Server::OP_STATUS::APP_ERR;
    }

    return res;
}

int Server::Requester::login_user(const char *const  login,
                                  const char *const  password,
                                  int64_t           *user_id) noexcept
{
    int res = 0;
    try {
        asio::io_context  io_ctx;
        json::object      req_json;
        std::string       req_str{};
        tcp::socket      *socket = new tcp::socket(io_ctx);

        req_json["operation"] = Server::SQL_SELECT_USER;
        req_json["login"]     = login;
        req_json["password"]  = password;

        req_str = json::serialize(req_json);

        /* Connecting to server */
        socket->async_connect(
            tcp::endpoint(ip::make_address(Server::Requester::ip()),
            Server::Requester::port()),
            [socket, req_str, user_id, &res]
            (const boost::system::error_code& err) mutable
            {
                if (err) {
                    fprintf(stderr, "Failed to async_connect login_user.\n");
                    fflush(stderr);
                    delete(socket);
                    res = Server::OP_STATUS::SERVER_ERR;
                    return;
                }

                /* Writing to server login data */
                asio::async_write(*socket,
                    asio::buffer(req_str),
                    [socket, user_id, &res]
                    (const boost::system::error_code& err,
                    size_t bytes) mutable
                    {
                        if (err) {
                            delete(socket);
                            res = Server::OP_STATUS::REQ_ERR;
                            return;
                        }

                        char *buffer = (char*)malloc(128 * sizeof(char));
                        if (!buffer) {
                            delete(socket);
                            res = Server::OP_STATUS::APP_ERR;
                            return;
                        }

                        /* Reading server's response on user's login */
                        socket->async_read_some(
                            asio::buffer(buffer, 127),
                            [socket, buffer, user_id, &res]
                            (const boost::system::error_code& err,
                            size_t bytes) mutable
                            {
                                if (err) {
                                    free(buffer);
                                    delete(socket);
                                    res = Server::OP_STATUS::REQ_ERR;
                                    return;
                                }

                                buffer[bytes] = '\0';
                                json::object res_json = json::parse(buffer).as_object();

                                *user_id = res_json["iduser"].as_int64();

                                /* cleanup */
                                free(buffer);
                                delete(socket);

                                res = (int)res_json["status"].as_int64();
                            }
                        );
                    }
                );
            }
        );

        io_ctx.run();
    }
    catch (std::exception& e) {
        fprintf(stderr, "Failed to register user. Error: %s.\n",
                e.what());
        return Server::OP_STATUS::APP_ERR;
    }

    return res;
}

int Server::Requester::add_convertation_to_history(const char *inpath,
                                                   const char *outpath,
                                                   char       *banner_data,
                                                   size_t     *banner_data_len,
                                                   int64_t     userid) noexcept
{
    int res = 0;
    try {
        asio::io_context  io_ctx;
        json::object      req_json;
        std::string       req_str{};
        tcp::socket      *socket   = new tcp::socket(io_ctx);
        size_t            path_len = 0;

        /* Creating json with "registration_info" data */
        req_json["operation"] = Server::SQL_INSERT_CONVERTS;

        path_len = std::strlen(inpath);
        if (path_len > Server::Requester::MAX_PATH_LEN) {
            req_json["infn"] = &inpath[path_len
                - Server::Requester::MAX_PATH_LEN - 1];
            std::cout << "inpath: " << inpath[path_len - Server::Requester::MAX_PATH_LEN - 1] << std::endl;
        }
        else {
            req_json["infn"] = inpath;
        }

        path_len = std::strlen(outpath);
        if (path_len > Server::Requester::MAX_PATH_LEN) {
            req_json["outfn"] = &outpath[path_len
                - Server::Requester::MAX_PATH_LEN - 1];
            std::cout << "outpath: " << outpath[path_len - Server::Requester::MAX_PATH_LEN - 1] << std::endl;
        }
        else {
            req_json["outfn"] = outpath;
        }

        req_json["iduser"] = userid;

        req_str = json::serialize(req_json);

        /* Connecting to server */
        socket->async_connect(
            tcp::endpoint(ip::make_address(Server::Requester::ip()),
            Server::Requester::port()),
            [socket, req_str, banner_data, banner_data_len, &res/*, show_banner*/]
            (const boost::system::error_code& err) mutable
            {
                if (err) {
                    fprintf(stderr, "Failed to async_connect 288.\n");
                    delete(socket);
                    /*show_banner(nullptr, 0);*/
                    puts("show_bunner(nullptr, 0) was invoked.\n");
                    fflush(stdout);
                    fflush(stderr);
                    res = Server::OP_STATUS::SERVER_ERR;
                    return;
                }

                /* Sending "convertation_info" to server */
                asio::async_write(*socket, asio::buffer(req_str),
                    [socket, banner_data, banner_data_len, &res/*, show_banner*/]
                    (const boost::system::error_code& err,
                     size_t bytes) mutable
                    {
                        if (err) {
                            fprintf(stderr,
                                    "Failed to async_write.\n");
                            delete(socket);
                            /*show_banner(nullptr, 0);*/
                            res = Server::OP_STATUS::REQ_ERR;
                            return;
                        }

                        char *buffer = (char*)
                            malloc(11 * sizeof(char));
                        if (!buffer) {
                            delete(socket);
                            /*show_banner(nullptr, 0);*/
                            res = Server::OP_STATUS::APP_ERR;
                            return;
                        }

                        /* Reading banner_size from server if convertation added to history */
                        socket->async_read_some(
                            asio::buffer(buffer, 10),
                            [socket, buffer, banner_data, banner_data_len, &res/*, show_banner*/]
                            (const boost::system::error_code& err,
                             size_t bytes) mutable
                            {
                                if (err) {
                                    fprintf(stderr,
                                        "Failed to async_connect 326.\n");
                                    delete(socket);
                                    free(buffer);
                                    /*show_banner(nullptr, 0);*/
                                    puts("show_bunner on 330 line.");
                                    fflush(stdout);
                                    fflush(stderr);
                                    res = Server::OP_STATUS::REQ_ERR;
                                    return;
                                }

                                const long long buffer_len =
                                    std::strtoll(buffer, nullptr, 10);

                                //*banner_data_len = buffer_len;

                                char *buffer_new = (char*)realloc(buffer, (buffer_len + 1) * sizeof(char));
                                if (!buffer_new) {
                                    fprintf(stderr,
                                            "Failed to realloc buffer.\n");
                                    delete(socket);
                                    free(buffer);
                                    /*show_banner(nullptr, 0);*/
                                    res = Server::OP_STATUS::APP_ERR;
                                    return;
                                }

                                //buffer = buffer_new;

                                /* Reading banner_img from server */
                                asio::async_read(*socket,
                                    asio::buffer(buffer, buffer_len),
                                    asio::transfer_exactly(buffer_len),
                                    [socket, buffer, banner_data, banner_data_len, &res/*, show_banner*/]
                                    (const boost::system::error_code& err,
                                     size_t bytes) mutable
                                    {
                                        if (err) {
                                            fprintf(stderr,
                                                "! Failed to async_read.\n");
                                            delete(socket);
                                            free(buffer);
                                            //show_banner(nullptr, 0);
                                            res = Server::OP_STATUS::REQ_ERR;
                                            return;
                                        }

                                        //show_banner(buffer, buffer_len);
                                        //banner_data = buffer;

                                        /* cleanup */
                                        delete(socket);

                                        res = Server::OP_STATUS::OK;
                                    }
                                );
                            }
                        );
                    }
                );
            }
        );

        io_ctx.run();
    }
    catch (std::exception& e) {
        fprintf(stderr,
                "Failed to add_convertation_to_history. Err: %s.\n",
                e.what());
        return Server::OP_STATUS::APP_ERR;
    }

    return res;
}

int Server::Requester::set_up()
{
    std::string str_buff{};
    QFile       config_file("configs" OS_SEPERATOR "server_config.txt");

    if (!config_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        fprintf(stderr, "! Can not open file server_config.txt.\n");
        return Server::OP_STATUS::APP_ERR;
    }

    str_buff = config_file.readLine();
    std::strncpy(ip_, str_buff.c_str(), str_buff.length() - 1);
    str_buff = config_file.readLine();
    port_ = std::strtol(str_buff.c_str(), nullptr, 10);

    config_file.close();

    return Server::OP_STATUS::OK;
}

char* Server::Requester::ip() noexcept {
    return ip_;
}
int Server::Requester::port() noexcept {
    return port_;
}

char           Server::Requester::ip_[16] = "127.0.0.1";
unsigned short Server::Requester::port_   = 8765;


#endif
