options gen2
options indenting = 4
require _tls_fixture
require daslib/clargs

[test]
def default_https_refuses_an_untrusted_certificate(t : T?) {
    with_tls_fixture(t) $(_root, dns_port, _ip_port) {
        GET("https://localhost:{dns_port}/ok") $(response) {
            t |> success(response == null, "a fresh test CA is not a trusted public issuer")
        }
    }
}

[test]
def default_https_does_not_send_a_downgraded_request(t : T?) {
    with_tls_fixture(t) $(root, dns_port, _ip_port) {
        let fixture = "{get_das_root()}/tests/dasHV/no_aot/_fixture_https_redirect.das"
        var output : string
        let code = run_and_capture([get_host_binary(), "-dasroot", get_das_root(), fixture, "--", "https://localhost:{dns_port}"], "", ["SSL_CERT_FILE={root}/ca.pem"], output, 15.0)
        t |> equal(code, 0, output)
    }
}
