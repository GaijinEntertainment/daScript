//! sscan_json into a default table value must not compile: a
//! temporary table cannot be a destination.
options gen2
options rtti
expect 30107

[export]
def main() {
    sscan_json("\{\}", default<table<string; int>>)
}
