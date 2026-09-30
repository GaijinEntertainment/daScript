// Excerpts from pinned libhv 303f50c7, used to test patch anchors.

#include "openssl/err.h"

    int mode = SSL_VERIFY_NONE;

        SSL_CTX_set_default_verify_paths(ctx);

    if (ssl == NULL || hostname == NULL) return HSSL_ERROR;
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
    if (SSL_set_tlsext_host_name((SSL*)ssl, hostname) != 1) {
        return HSSL_ERROR;
    }
#endif
    return HSSL_OK;

int hssl_accept(hssl_t ssl) {
    int ret = SSL_accept((SSL*)ssl);

int hssl_connect(hssl_t ssl) {
    int ret = SSL_connect((SSL*)ssl);

    return SSL_read((SSL*)ssl, buf, len);

    return SSL_write((SSL*)ssl, buf, len);

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
