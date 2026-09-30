options gen2
options indenting = 4
options no_aot // native TLS fixture is controlled through subprocess APIs
require _tls_fixture
require daslib/clargs

[test]
def default_https_refuses_an_untrusted_certificate(t : T?) {
    with_tls_fixture(t) $(root, dns_port, ip_port) {
        GET("https://localhost:{dns_port}/ok") $(response) {
            t |> success(response == null, "a fresh test CA is not a trusted public issuer")
        }
    }
}

[test]
def default_https_does_not_send_a_downgraded_request(t : T?) {
    with_tls_fixture(t) $(root, dns_port, ip_port) {
        let fixture = "{get_das_root()}/tests/dasHV/no_aot/_fixture_https_redirect.das"
        with_process([get_host_binary(), "-dasroot", get_das_root(), fixture, "--", "https://localhost:{dns_port}"], "", ["SSL_CERT_FILE={root}/ca.pem"]) $(var child) {
            let code = unsafe(process_wait(child, 15.0))
            var output : string
            unsafe(process_drain(child) $(line) { output += "{line}\n"; })
            t |> equal(code, 0, output)
        }
    }
}
