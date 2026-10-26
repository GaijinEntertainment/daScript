options gen2
options rtti
expect 30107

[export]
def main() {
    sscan_json("[]", default<array<int>>)
}
