// LSP protocol-test fixture (spawned through the supervisor, not a test entry
// point). test_lsp_protocol.das opens this file with BROKEN buffer text while
// this disk content stays clean — proving the --overlay path end-to-end. The
// test derives expected positions by scanning for lsp_probe_helper, so edits
// here don't break it as long as the helper and its call survive.
options gen2

class LspProbeBase {
    def probe_speak() : string => "base"
}

class LspProbeDerived : LspProbeBase {
    def override probe_speak() : string => "derived"
}

class LspProbeFinalized : LspProbeBase {
    def finalize() {
        print("final\n")
    }
}

def lsp_probe_helper() {
    print("probe\n")
}

def lsp_probe_self(n : int) : int => n > 0 ? lsp_probe_self(n - 1) : 0

def lsp_probe_generic(n : auto) : int => n > 0 ? lsp_probe_generic(n - 1) : 0

enum LspProbeColor {
    red
    green
}

typedef LspProbePair = tuple<first : int; second : int>

let LSP_PROBE_LIMIT = 3

def lsp_probe_gen() : iterator<int> {
    return <- generator<int> {
        for (i in range(LSP_PROBE_LIMIT)) {
            yield i
        }
        return false
    }
}

[export]
def main() {
    lsp_probe_helper()
    var d = new LspProbeDerived()
    print("{d->probe_speak()}\n")
    var f = new LspProbeFinalized()
    print("{f->probe_speak()}\n")
    let base = 10
    let add = @(x : int) : int => x + base
    print("{add(1)}\n")
    for (i in range(2)) {
        print("{i}\n")
    }
    print("{lsp_probe_self(2) + lsp_probe_generic(2)}\n")
    var pair : LspProbePair
    pair.first = int(LspProbeColor.green)
    for (g in lsp_probe_gen()) {
        pair.second += g
    }
    print("{pair.first} {pair.second}\n")
}
