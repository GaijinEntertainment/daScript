// LSP protocol-test fixture (spawned through the supervisor, not a test entry
// point). test_lsp_protocol.das opens this file with BROKEN buffer text while
// this disk content stays clean — proving the --overlay path end-to-end. The
// test derives expected positions by scanning for lsp_probe_helper, so edits
// here don't break it as long as the helper and its call survive.
options gen2

def lsp_probe_helper() {
    print("probe\n")
}

[export]
def main() {
    lsp_probe_helper()
}
