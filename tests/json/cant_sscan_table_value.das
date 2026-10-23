options gen2
options rtti
expect 31200

[export]
def main() {
    sscan_json("\{\}", default<table<string; int>>)
}
