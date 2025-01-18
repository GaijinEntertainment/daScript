options gen2

template struct Foo  {
    a : $t(TT) = 2

    def add(b : $t(TT)) {
        a += b
    }
}

[export]
def main() {
    print("Hello, World!\n")
}

options log_infer_passes;
