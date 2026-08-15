// gc-position-liveness branch: 0xC0000374 repro - ANY script triggers it, the crash
// is in the startup dynamic-module scan (register_dynamic_module -> RtlFreeHeap),
// before user code runs. delete this file when the hunt is over.
[export]
def main {
    print("hi\n")
}
