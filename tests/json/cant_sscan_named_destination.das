options gen2
expect 30341

[export]
def main() {
    let value = 0.0
    sscan_json([json = "1.5", value = value])
}
