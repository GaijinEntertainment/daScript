//! sscan_json into a const scalar must not compile: the destination
//! is written, so it cannot be const.
options gen2
expect 30341

[export]
def main() {
    let value = 0.0
    sscan_json("1.5", value)
}
