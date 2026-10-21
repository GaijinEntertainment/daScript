options gen2
expect 31200

[export]
def main() {
    let value = 0.0
    sscan_json("1.5", value)
}
