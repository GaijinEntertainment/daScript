options gen2
options rtti
expect 31200

[export]
def main() {
    sscan_json("[1,2]", [1, 2])
}
