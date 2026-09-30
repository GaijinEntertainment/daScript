#include <hv/HttpServer.h>
#include <hv/hsocket.h>
#include <hv/hlog.h>
#include <hv/EventLoop.h>
#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/pem.h>
#include <openssl/x509v3.h>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <memory>
#include <string>
#include <thread>

using Key = std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)>;
using Cert = std::unique_ptr<X509, decltype(&X509_free)>;
static Key key() {
    EVP_PKEY_CTX * context = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, nullptr);
    EVP_PKEY * result = nullptr;
    if (context && EVP_PKEY_keygen_init(context) > 0 &&
        EVP_PKEY_CTX_set_ec_paramgen_curve_nid(context, NID_X9_62_prime256v1) > 0) {
        EVP_PKEY_keygen(context, &result);
    }
    EVP_PKEY_CTX_free(context);
    return Key(result, EVP_PKEY_free);
}
static bool extension(X509 * cert, X509 * issuer, int nid, const char * text) {
    X509V3_CTX context;
    X509V3_set_ctx(&context, issuer, cert, nullptr, nullptr, 0);
    X509_EXTENSION * ext = X509V3_EXT_conf_nid(nullptr, &context, nid, const_cast<char *>(text));
    const bool ok = ext && X509_add_ext(cert, ext, -1) == 1;
    X509_EXTENSION_free(ext);
    return ok;
}
static Cert certificate(EVP_PKEY * subject_key, X509 * issuer, EVP_PKEY * signer, int serial, const char * san) {
    Cert result(X509_new(), X509_free);
    if (!result) return result;
    X509_set_version(result.get(), 2);
    ASN1_INTEGER_set(X509_get_serialNumber(result.get()), serial);
    X509_gmtime_adj(X509_getm_notBefore(result.get()), -60);
    X509_gmtime_adj(X509_getm_notAfter(result.get()), 86400);
    X509_set_pubkey(result.get(), subject_key);
    auto name = X509_get_subject_name(result.get());
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC,
        reinterpret_cast<const unsigned char *>(san ? "localhost" : "Ephemeral test CA"), -1, -1, 0);
    X509_set_issuer_name(result.get(), issuer ? X509_get_subject_name(issuer) : name);
    if (!extension(result.get(), issuer ? issuer : result.get(), NID_basic_constraints,
                   san ? "critical,CA:FALSE" : "critical,CA:TRUE")) return Cert(nullptr, X509_free);
    if (san && (!extension(result.get(), issuer, NID_subject_alt_name, san) ||
                !extension(result.get(), issuer, NID_ext_key_usage, "serverAuth"))) return Cert(nullptr, X509_free);
    if (!X509_sign(result.get(), signer, EVP_sha256())) return Cert(nullptr, X509_free);
    return result;
}
static bool save_cert(const std::string & path, X509 * cert) {
    FILE * file = fopen(path.c_str(), "wb");
    if (!file) return false;
    const bool ok = PEM_write_X509(file, cert) == 1;
    fclose(file);
    return ok;
}
static bool save_key(const std::string & path, EVP_PKEY * value) {
    FILE * file = fopen(path.c_str(), "wb");
    if (!file) return false;
    const bool ok = PEM_write_PrivateKey(file, value, nullptr, nullptr, 0, nullptr, nullptr) == 1;
    fclose(file);
    return ok;
}
static int start(hv::HttpServer & server, const std::string & cert, const std::string & key_file) {
    hssl_ctx_opt_t options{};
    options.crt_file = cert.c_str(); options.key_file = key_file.c_str(); options.endpoint = HSSL_SERVER;
    if (server.newSslCtx(&options) != 0) return -1;
    int fd = Listen(0, "127.0.0.1");
    if (fd < 0) return -1;
    sockaddr_u address{}; socklen_t size = sizeof(address);
    if (getsockname(fd, &address.sa, &size) != 0) { closesocket(fd); return -1; }
    const int port = sockaddr_port(&address);
    server.setThreadNum(1);
    server.setPort(0, 0);
    server.setListenFD(-1, fd);
    if (server.start() != 0) return -1;
    return port;
}
int main(int argc, char ** argv) {
    if (argc != 2) return 2;
    const std::string root = argv[1];
    hlog_disable();
    auto ca_key = key(), server_key = key(), wrong_key = key();
    if (!ca_key || !server_key || !wrong_key) return 3;
    auto ca = certificate(ca_key.get(), nullptr, ca_key.get(), 1, nullptr);
    auto wrong = certificate(wrong_key.get(), nullptr, wrong_key.get(), 2, nullptr);
    if (!ca || !wrong) return 4;
    auto dns = certificate(server_key.get(), ca.get(), ca_key.get(), 3, "DNS:localhost");
    auto ip = certificate(server_key.get(), ca.get(), ca_key.get(), 4, "DNS:localhost,IP:127.0.0.1");
    if (!dns || !ip || !save_cert(root + "/ca.pem", ca.get()) || !save_cert(root + "/wrong.pem", wrong.get()) ||
        !save_cert(root + "/dns.pem", dns.get()) || !save_cert(root + "/ip.pem", ip.get()) ||
        !save_key(root + "/key.pem", server_key.get())) return 5;
    std::atomic<int> count{0}, plaintext_hits{0};
    int plaintext_port = 0;
    hv::HttpService routes;
    routes.GET("/ok", [&](HttpRequest *, HttpResponse * response) { ++count; response->body = "verified"; return 200; });
    routes.GET("/large", [&](HttpRequest *, HttpResponse * response) { ++count; response->body.assign(8192, 'x'); return 200; });
    routes.GET("/count", [&](HttpRequest *, HttpResponse * response) { response->body = std::to_string(count.load()); return 200; });
    routes.GET("/redirect", [&](HttpRequest *, HttpResponse * response) { response->headers["Location"] = "http://127.0.0.1:" + std::to_string(plaintext_port) + "/leak"; return 302; });
    routes.GET("/leak", [&](HttpRequest *, HttpResponse * response) { ++plaintext_hits; response->body = "leaked"; return 200; });
    routes.GET("/plaintext-count", [&](HttpRequest *, HttpResponse * response) { response->body = std::to_string(plaintext_hits.load()); return 200; });
    routes.GET("/truncated", http_ctx_handler([&](const HttpContextPtr & context) {
        ++count;
        auto writer = context->writer;
        hv::tlsEventLoop()->queueInLoop([writer]() {
            writer->write("HTTP/1.1 200 OK\r\nContent-Length: 100\r\nConnection: close\r\n\r\nshort");
            writer->close(true);
        });
        return HTTP_STATUS_UNFINISHED;
    }));
    hv::HttpServer plaintext_server(&routes);
    const int plain_fd = Listen(0, "127.0.0.1");
    if (plain_fd < 0) return 9;
    sockaddr_u plain_address{}; socklen_t plain_size = sizeof(plain_address);
    if (getsockname(plain_fd, &plain_address.sa, &plain_size) != 0) return 10;
    plaintext_port = sockaddr_port(&plain_address);
    plaintext_server.setListenFD(plain_fd, -1);
    plaintext_server.setThreadNum(1);
    if (plaintext_server.start() != 0) return 11;
    hv::HttpServer dns_server(&routes), ip_server(&routes);
    const int dns_port = start(dns_server, root + "/dns.pem", root + "/key.pem");
    const int ip_port = start(ip_server, root + "/ip.pem", root + "/key.pem");
    if (dns_port <= 0 || ip_port <= 0) return 6;
    FILE * ready = fopen((root + "/ready.tmp").c_str(), "wb");
    if (!ready) return 7;
    fprintf(ready, "%d\n%d\n", dns_port, ip_port); fclose(ready);
    if (rename((root + "/ready.tmp").c_str(), (root + "/ready").c_str()) != 0) return 8;
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(120);
    while (std::chrono::steady_clock::now() < deadline) {
        FILE * release = fopen((root + "/release").c_str(), "rb");
        if (release) { fclose(release); break; }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    dns_server.stop(); ip_server.stop(); plaintext_server.stop();
    return 0;
}
