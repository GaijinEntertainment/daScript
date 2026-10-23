options gen2
options rtti
expect 31200

[export]
def main() {
    sscan_json("[]", default<array<int>>)
}
