options gen2

require _inline_scope_helper

expect 30100

[export]
def main() {
    feint(must_inline_visibility_root())
}
