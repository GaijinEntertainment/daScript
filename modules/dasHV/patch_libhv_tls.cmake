# Verified client TLS, platform trust anchors and bounded HTTP response parsing.

das_hv_patch_begin("ssl/openssl.c")
das_hv_hunk([=[
#include "openssl/err.h"
]=] [=[
#include "openssl/err.h"
#include "openssl/x509v3.h"
#include "hsocket.h"
#if defined(__APPLE__)
#include <TargetConditionals.h>
#if !TARGET_OS_IPHONE
#include <Security/Security.h>
#endif
#elif defined(_WIN32)
#include <windows.h>
#include <wincrypt.h>
#endif

static void load_platform_roots(SSL_CTX* ctx) {
    X509_STORE* store = SSL_CTX_get_cert_store(ctx);
#if defined(__APPLE__) && !TARGET_OS_IPHONE
    CFArrayRef anchors = NULL;
    if (SecTrustCopyAnchorCertificates(&anchors) == errSecSuccess && anchors) {
        for (CFIndex i = 0; i < CFArrayGetCount(anchors); ++i) {
            CFDataRef der = SecCertificateCopyData((SecCertificateRef)CFArrayGetValueAtIndex(anchors, i));
            if (!der) continue;
            const unsigned char* bytes = CFDataGetBytePtr(der);
            X509* certificate = d2i_X509(NULL, &bytes, (long)CFDataGetLength(der));
            if (certificate) { X509_STORE_add_cert(store, certificate); X509_free(certificate); }
            ERR_clear_error();
            CFRelease(der);
        }
        CFRelease(anchors);
    }
#elif defined(_WIN32)
    HCERTSTORE roots = CertOpenSystemStoreA(0, "ROOT");
    if (roots) {
        PCCERT_CONTEXT item = NULL;
        while ((item = CertEnumCertificatesInStore(roots, item)) != NULL) {
            const unsigned char* bytes = item->pbCertEncoded;
            X509* certificate = d2i_X509(NULL, &bytes, item->cbCertEncoded);
            if (certificate) { X509_STORE_add_cert(store, certificate); X509_free(certificate); }
            ERR_clear_error();
        }
        CertCloseStore(roots, 0);
    }
#else
    (void)store;
#endif
}
]=])
das_hv_hunk([=[
    int mode = SSL_VERIFY_NONE;
]=] [=[
    int mode = param ? SSL_VERIFY_NONE : SSL_VERIFY_PEER;
]=])
das_hv_hunk([=[
        SSL_CTX_set_default_verify_paths(ctx);
]=] [=[
        SSL_CTX_set_default_verify_paths(ctx);
        load_platform_roots(ctx);
]=])
das_hv_hunk([=[
    if (ssl == NULL || hostname == NULL) return HSSL_ERROR;
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
    if (SSL_set_tlsext_host_name((SSL*)ssl, hostname) != 1) {
        return HSSL_ERROR;
    }
#endif
    return HSSL_OK;
]=] [=[
    if (ssl == NULL || hostname == NULL || *hostname == '\0') return HSSL_ERROR;
    const int numeric = is_ipaddr(hostname);
    if (SSL_get_verify_mode((SSL*)ssl) & SSL_VERIFY_PEER) {
        X509_VERIFY_PARAM* parameters = SSL_get0_param((SSL*)ssl);
        X509_VERIFY_PARAM_set_hostflags(parameters, X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS);
        if (numeric) {
            if (X509_VERIFY_PARAM_set1_ip_asc(parameters, hostname) != 1) return HSSL_ERROR;
        } else if (X509_VERIFY_PARAM_set1_host(parameters, hostname, 0) != 1) return HSSL_ERROR;
    }
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
    if (!numeric && SSL_set_tlsext_host_name((SSL*)ssl, hostname) != 1) return HSSL_ERROR;
#endif
    return HSSL_OK;
]=])
das_hv_hunk([=[
int hssl_accept(hssl_t ssl) {
    int ret = SSL_accept((SSL*)ssl);
]=] [=[
int hssl_accept(hssl_t ssl) {
    ERR_clear_error();
    int ret = SSL_accept((SSL*)ssl);
]=])
das_hv_hunk([=[
int hssl_connect(hssl_t ssl) {
    int ret = SSL_connect((SSL*)ssl);
]=] [=[
int hssl_connect(hssl_t ssl) {
    ERR_clear_error();
    int ret = SSL_connect((SSL*)ssl);
]=])
das_hv_hunk([=[
    return SSL_read((SSL*)ssl, buf, len);
]=] [=[
    ERR_clear_error();
    return SSL_read((SSL*)ssl, buf, len);
]=])
das_hv_hunk([=[
    return SSL_write((SSL*)ssl, buf, len);
]=] [=[
    ERR_clear_error();
    return SSL_write((SSL*)ssl, buf, len);
]=])
das_hv_hunk([=[
    static int s_initialized = 0;
    if (s_initialized == 0) {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        SSL_library_init();
        SSL_load_error_strings();
#else
        OPENSSL_init_ssl(OPENSSL_INIT_SSL_DEFAULT, NULL);
#endif
        s_initialized = 1;
    }
]=] [=[
#if OPENSSL_VERSION_NUMBER < 0x10100000L
    SSL_library_init();
    SSL_load_error_strings();
#else
    OPENSSL_init_ssl(OPENSSL_INIT_SSL_DEFAULT, NULL);
#endif
]=])
das_hv_patch_end()

das_hv_patch_begin("http/client/HttpClient.cpp")
das_hv_hunk([=[
            if (cli->parser->IsEof()) {
                err = 0;
                goto disconnect;
            }
            if (retry_count-- > 0 && left_time > req->retry_delay + connect_timeout * 1000) {
]=] [=[
            if (cli->parser->IsEof()) {
                err = 0;
                goto disconnect;
            }
            if (err == 0) err = ERR_PARSE;
            if (retry_count-- > 0 && left_time > req->retry_delay + connect_timeout * 1000) {
]=])
das_hv_hunk([=[
        } else if (g_ssl_ctx) {
            ssl_ctx = g_ssl_ctx;
        } else {
]=] [=[
        } else {
]=])
das_hv_hunk([=[
        if (!is_ipaddr(host)) {
            hssl_set_sni_hostname(cli->ssl, host);
        }
]=] [=[
        if (hssl_set_sni_hostname(cli->ssl, host) != HSSL_OK) {
            hssl_free(cli->ssl); cli->ssl = NULL;
            closesocket(connfd);
            return NABS(ERR_SSL);
        }
]=])
das_hv_hunk([=[
    char recvbuf[1024] = {0};
    char* data = NULL;
]=] [=[
    cli->parser->max_body_size = req->response_body_limit;
    char recvbuf[1024] = {0};
    char* data = NULL;
]=])
das_hv_hunk([=[
        int nparse = cli->parser->FeedRecvData(recvbuf, nrecv);
        if (nparse != nrecv) {
            return ERR_PARSE;
        }
]=] [=[
        int nparse = cli->parser->FeedRecvData(recvbuf, nrecv);
        if (cli->parser->body_limit_exceeded) { cli->Close(); return ERR_OVER_LIMIT; }
        if (nparse != nrecv || cli->parser->GetError() != 0) {
            cli->Close();
            return ERR_PARSE;
        }
]=])
das_hv_hunk([=[
static int http_client_redirect(HttpRequest* req, HttpResponse* resp) {
    std::string location = resp->headers["Location"];
    if (!location.empty()) {
        hlogi("redirect %s => %s", req->url.c_str(), location.c_str());
        req->url = location;
        req->ParseUrl();
        req->headers["Host"] = req->host;
        resp->Reset();
        return http_client_send(req, resp);
    }
    return 0;
}
]=] [=[
static int http_client_redirect(http_client_t* cli, HttpRequest* req, HttpResponse* resp) {
    std::string location = resp->headers["Location"];
    if (!location.empty()) {
        const int redirected = req->RedirectTo(location);
        if (redirected != 0) return redirected;
        resp->Reset();
        return http_client_send(cli, req, resp);
    }
    return 0;
}
]=])
das_hv_hunk([=[
        return http_client_redirect(req, resp);
]=] [=[
        return http_client_redirect(cli, req, resp);
]=])
das_hv_hunk([=[
    for (const auto& pair : cli->headers) {
        if (req->headers.find(pair.first) == req->headers.end()) {
]=] [=[
    for (const auto& pair : cli->headers) {
        if (req->redirect_count && (stricmp(pair.first.c_str(), "Authorization") == 0 ||
            stricmp(pair.first.c_str(), "Proxy-Authorization") == 0 || stricmp(pair.first.c_str(), "Cookie") == 0)) continue;
        if (req->headers.find(pair.first) == req->headers.end()) {
]=])
das_hv_patch_end()

das_hv_patch_begin("event/nio.c")
das_hv_hunk([=[
                } else if (g_ssl_ctx) {
                    ssl_ctx = g_ssl_ctx;
                } else {
]=] [=[
                } else {
]=])
das_hv_hunk([=[
            if (io->hostname) {
                hssl_set_sni_hostname(io->ssl, io->hostname);
            }
]=] [=[
            if (io->hostname && hssl_set_sni_hostname(io->ssl, io->hostname) != HSSL_OK) {
                io->error = ERR_SSL;
                goto connect_error;
            }
]=])
das_hv_patch_end()

das_hv_patch_begin("evpp/TcpClient.h")
das_hv_hunk([=[
            if (!is_ipaddr(remote_host.c_str())) {
                channel->setHostname(remote_host);
            }
]=] [=[
            channel->setHostname(remote_host);
]=])
das_hv_patch_end()

das_hv_patch_begin("http/client/AsyncHttpClient.cpp")
das_hv_hunk([=[
            if (!is_ipaddr(host)) {
                hio_set_hostname(connio, host);
            }
]=] [=[
            hio_set_hostname(connio, host);
]=])
das_hv_hunk([=[
    auto iter = conn_pools.find(strAddr);
]=] [=[
    const std::string pool_key = req->scheme + "://" + req->host + ":" + std::to_string(req->port) + "@" + strAddr;
    auto iter = conn_pools.find(pool_key);
]=])
das_hv_hunk([=[
    ctx->task = task;
    channel->onconnect = [&channel]() {
]=] [=[
    ctx->task = task;
    ctx->pool_key = pool_key;
    channel->onconnect = [&channel]() {
]=])
das_hv_hunk([=[
                    hlogi("redirect %s => %s", req->url.c_str(), location.c_str());
                    req->url = location;
                    req->ParseUrl();
                    req->headers["Host"] = req->host;
]=] [=[
                    if (req->RedirectTo(location) != 0) {
                        ctx->errorCallback();
                        channel->close();
                        return;
                    }
]=])
das_hv_hunk([=[
                conn_pools[channel->peeraddr()].add(channel->fd());
]=] [=[
                conn_pools[ctx->pool_key].add(channel->fd());
]=])
das_hv_hunk([=[
        auto iter = conn_pools.find(channel->peeraddr());
]=] [=[
        auto iter = conn_pools.find(ctx->pool_key);
]=])
das_hv_patch_end()

das_hv_patch_begin("http/HttpMessage.h")
das_hv_hunk([=[
    uint32_t            retry_delay;    // unit: ms
]=] [=[
    uint32_t            retry_delay;    // unit: ms
    size_t              response_body_limit = 0;
    unsigned            redirect_count = 0;
]=])
das_hv_hunk([=[
    void ParseUrl();
]=] [=[
    void ParseUrl();
    int RedirectTo(const std::string& location);
]=])
das_hv_patch_end()

das_hv_patch_begin("http/HttpMessage.cpp")
das_hv_hunk([=[
    redirect = 1;
    proxy = 0;
]=] [=[
    redirect = 1;
    redirect_count = 0;
    response_body_limit = 0;
    proxy = 0;
]=])
das_hv_hunk([=[
#include "hurl.h"
]=] [=[
#include "hurl.h"
#include "herr.h"
]=])
das_hv_hunk([=[
void HttpRequest::ParseUrl() {
]=] [=[
int HttpRequest::RedirectTo(const std::string& location) {
    if (redirect_count >= 5) return ERR_OVER_LIMIT;
    if (location.empty() || location.size() > 8192) return ERR_INVALID_PARAM;
    for (unsigned char byte : location) if (byte <= 32 || byte == 127) return ERR_INVALID_PARAM;
    HttpRequest next = *this;
    next.url = location;
    next.ParseUrl();
    if (IsHttps() && !next.IsHttps()) return ERR_INVALID_PROTOCOL;
    if (stricmp(host.c_str(), next.host.c_str()) != 0 || port != next.port || scheme != next.scheme) {
        headers.erase("Authorization");
        headers.erase("Proxy-Authorization");
        headers.erase("Cookie");
        cookies.clear();
    }
    ++redirect_count;
    url = location;
    headers.erase("Host");
    ParseUrl();
    return 0;
}

void HttpRequest::ParseUrl() {
]=])
das_hv_patch_end()

das_hv_patch_begin("http/client/AsyncHttpClient.h")
das_hv_hunk([=[
    HttpClientTaskPtr   task;
]=] [=[
    HttpClientTaskPtr   task;
    std::string        pool_key;
]=])
das_hv_patch_end()
