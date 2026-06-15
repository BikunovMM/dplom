#include "serverrequester.h"

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

void onSendEmailCodeConn(uv_connect_t *req, int status);
void onSendEmailCodeRead(uv_stream_t *stream, ssize_t nread, const uv_buf_t* buf);

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
                                  const char *code,
                                  char *token)
{
    Client          client = {};
    char           *ip     = SERVER_IP;
    unsigned short  port   = SERVER_PORT;
    int             ret    = 0;

    client.login    = login;
    client.password = password;
    client.email    = (char*)email;
    client.code     = code;
    client.op       = Operations::REG_USER;
    client.on_conn  = onRegUserConn;
    client.on_write = onWrite;
    client.on_read  = onRegUserRead;
    client.token    = token;

    log_str("[*][] email: %s, code: %s.\n", client.email, client.code);

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
        log_err("[!][main] Failed to validateToken.\n");
    }

    return client.parser.status_code;
}

int ServerRequester::sendEmailCode(const char *email)
{
    Client          client = {};
    char           *ip     = SERVER_IP;
    unsigned short  port   = SERVER_PORT;
    int             ret    = 0;

    client.email    = (char*)email;
    client.op       = Operations::SEND_EMAIL_CODE;
    client.on_conn  = onSendEmailCodeConn;
    client.on_write = onWrite;
    client.on_read  = onSendEmailCodeRead;

    ret = initAndRun(ip, port, &client);
    if (ret < 0) {
        log_err("[!][main] Failed to sendEmailCode.\n");
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
            + strlen(client->email) + strlen(client->code)+ 3,
            client->login, client->password,
            client->email, client->code);

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

void onSendEmailCodeConn(uv_connect_t *req, int status)
{
    if (status < 0) {
        log_err("[!] [onSendEmailCodeConn] Failed. Status < 0.\n");
        return;
    }

    ServerRequester::Client *client = (ServerRequester::Client*)req->data;
    time_t cur_time = {};
    struct tm *cur_tm = NULL;
    char buf_str[ServerRequester::MAX_REG_REQ_LEN] = {};
    size_t buf_len = 0;

    log_str("[*][onSendEmailCodeConn]\n");

    cur_time = time(NULL);
    cur_tm = localtime(&cur_time);

    client->url = ServerRequester::SEND_EMAIL_CODE_URL;

    buf_len = snprintf(buf_str, sizeof(buf_str),
                       HTTP_REQ_HEAD
                       SEND_EMAIL_CODE_STR,
                       ServerRequester::HTTPPOST, client->url,
                       ServerRequester::DAY_NAMES[cur_tm->tm_wday],
                       cur_tm->tm_mday,
                       ServerRequester::MONTH_NAMES[cur_tm->tm_mon],
                       cur_tm->tm_year + 1900, cur_tm->tm_hour,
                       cur_tm->tm_min, cur_tm->tm_sec,
                       strlen(client->email),
                       client->email);

    log_str("[*][onSendEmailCodeConn] str:\n^%s^(len: %zu).\n\n",
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

void onSendEmailCodeRead(uv_stream_t *stream, ssize_t nread, const uv_buf_t* buf)
{
    log_str("[*][onSendEmailCodeRead]\n");

    if (nread <= 0) {
        log_err("[!] [onSendEmailCodeRead] Failed. nread <= 0.\n");
        return;
    }

    ServerRequester::Client          *client = NULL;
    llhttp_t          *parser = NULL;
    enum llhttp_errno  err = llhttp_errno::HPE_OK;
    int                ret = 0;

    (void)ret;

    client = (ServerRequester::Client*)stream;
    parser = &client->parser;

    log_str("[*][onSendEmailCodeRead] buf->base:\n^%s^\n", buf->base);

    err = llhttp_execute(parser, buf->base, nread);
    if (err == HPE_OK &&
        parser->type == HTTP_RESPONSE)
    {
        log_str("[*][onSendEmailCodeRead] ok.\n");
    }
    else {
        log_err("[!][onSendEmailCodeRead] Failed to llhttp_execute.\n");
    }

    uv_close((uv_handle_t*)&client->socket, onClose);
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
