options gen2
require null_entry_breaker

[export]
def main {
    // dropped by the breaker, so nothing below may reference it
    var probe = 1
    print("ok\n")
}
