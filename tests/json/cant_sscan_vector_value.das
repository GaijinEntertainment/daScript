options gen2
expect 31200

[export]
def main() {
    sscan_json("[1,2]", float2(1, 2))
}
