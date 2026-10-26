options gen2
options rtti
expect 30107

[export]
def main() {
    sscan_json("[1,2]", [1, 2])
}
