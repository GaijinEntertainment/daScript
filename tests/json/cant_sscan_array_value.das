//! sscan_json into a temporary array value must not compile: the
//! destination has to be an addressable variable.
options gen2
options rtti
expect 30107

[export]
def main() {
    sscan_json("[]", default<array<int>>)
}
