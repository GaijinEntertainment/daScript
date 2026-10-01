#include <hv/HttpServer.h>
#include <hv/HttpClient.h>
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
#include <future>
#include <cstdlib>
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
using Bio = std::unique_ptr<BIO, decltype(&BIO_free)>;
static bool save_pem(const std::string & path, BIO * encoded) {
    char * data = nullptr;
    const long length = BIO_get_mem_data(encoded, &data);
    if (length <= 0) return false;
    FILE * file = fopen(path.c_str(), "wb");
    if (!file) return false;
    const bool written = fwrite(data, 1, static_cast<size_t>(length), file) == static_cast<size_t>(length);
    const bool closed = fclose(file) == 0;
    return written && closed;
}
static bool save_cert(const std::string & path, X509 * cert) {
    Bio encoded(BIO_new(BIO_s_mem()), BIO_free);
    return encoded && PEM_write_bio_X509(encoded.get(), cert) == 1 && save_pem(path, encoded.get());
}
static bool save_key(const std::string & path, EVP_PKEY * value) {
    Bio encoded(BIO_new(BIO_s_mem()), BIO_free);
    return encoded && PEM_write_bio_PrivateKey(encoded.get(), value, nullptr, nullptr, 0, nullptr, nullptr) == 1 && save_pem(path, encoded.get());
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
static bool sync_reply(hv::HttpClient & client, const std::string & url, const std::string & body) {
    HttpRequest request;
    request.url = url; request.timeout = 3; request.retry_count = 0;
    HttpResponse response;
    return client.send(&request, &response) == 0 && response.status_code == 200 && response.body == body;
}
static int async_status(hv::HttpClient & client, const std::string & url) {
    auto promise = std::make_shared<std::promise<int>>();
    auto future = promise->get_future();
    auto request = std::make_shared<HttpRequest>();
    request->url = url; request->timeout = 3; request->retry_count = 0;
    request->headers["Authorization"] = "fixture-secret";
    request->headers["Cookie"] = "fixture-cookie";
    if (client.sendAsync(request, [promise](const HttpResponsePtr & response) {
        promise->set_value(response ? int(response->status_code) : 0);
    }) != 0) return -1;
    return future.wait_for(std::chrono::seconds(5)) == std::future_status::ready ? future.get() : -2;
}
static bool client_probes(const std::string & root, int dns_port, const std::atomic<int> & plaintext_hits) {
    const std::string ca_file = root + "/ca.pem";
#ifdef _WIN32
    _putenv_s("SSL_CERT_FILE", ca_file.c_str());
#else
    setenv("SSL_CERT_FILE", ca_file.c_str(), 1);
#endif
    const std::string dns_url = "https://localhost:" + std::to_string(dns_port);
    hv::HttpClient sync;
    hssl_ctx_opt_t options{};
    options.endpoint = HSSL_CLIENT; options.verify_peer = 1; options.ca_file = ca_file.c_str();
    if (sync.newSslCtx(&options) != 0) return false;
    sync.setHeader("Authorization", "fixture-secret");
    sync.setHeader("Cookie", "fixture-cookie");
    sync.setHeader("Proxy-Authorization", "fixture-proxy");
    const bool same = sync_reply(sync, dns_url + "/same", "fixture-secret|fixture-proxy|fixture-cookie");
    const bool cross = sync_reply(sync, dns_url + "/cross", "||");
    HttpRequest loop; loop.url = dns_url + "/loop"; loop.timeout = 3; loop.retry_count = 0;
    HttpResponse response;
    const bool bounded = sync.send(&loop, &response) == 1022;
    hv::HttpClient async;
    const bool first = async_status(async, dns_url + "/ok") == 200;
    const bool different_host = async_status(async, "https://127.0.0.1:" + std::to_string(dns_port) + "/ok") == 0;
    const bool downgrade = async_status(async, dns_url + "/redirect") == 0 && plaintext_hits.load() == 0;
    FILE * output = fopen((root + "/client-results").c_str(), "wb");
    if (!output) return false;
    fprintf(output, "%d %d %d %d %d %d", same, cross, bounded, first, different_host, downgrade);
    fclose(output);
    return true;
}
int main(int argc, char ** argv) {
    if (argc < 2 || argc > 3) return 2;
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
    int plaintext_port = 0, dns_port = 0, ip_port = 0;
    hv::HttpService routes;
    routes.GET("/inspect", [&](HttpRequest * request, HttpResponse * response) { response->body = request->GetHeader("Authorization") + "|" + request->GetHeader("Proxy-Authorization") + "|" + request->GetHeader("Cookie"); return 200; });
    routes.GET("/same", [&](HttpRequest *, HttpResponse * response) { response->headers["Location"] = "https://localhost:" + std::to_string(dns_port) + "/inspect"; return 302; });
    routes.GET("/cross", [&](HttpRequest *, HttpResponse * response) { response->headers["Location"] = "https://127.0.0.1:" + std::to_string(ip_port) + "/inspect"; return 302; });
    routes.GET("/loop", [&](HttpRequest *, HttpResponse * response) { response->headers["Location"] = "https://localhost:" + std::to_string(dns_port) + "/loop"; return 302; });
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
    dns_port = start(dns_server, root + "/dns.pem", root + "/key.pem");
    ip_port = start(ip_server, root + "/ip.pem", root + "/key.pem");
    if (dns_port <= 0 || ip_port <= 0) return 6;
    if (argc == 3 && std::string(argv[2]) == "clients" && !client_probes(root, dns_port, plaintext_hits)) return 12;
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
