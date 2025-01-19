template struct Foo  {
    a_t : $t(TT) = 2;

    def add(b : $t(TT)) {
        a_t += b;
    }

    s : int;

    def static sum ( a,b : int ) : int {
        return a + b;
    }


    def init() {
        s = sum(1,2);
    }

}

[export]
def main() {
    print("Hello, World!\n");
    var f = Foo();
    debug(f);
    let t = @@Foo`sum;
    debug(t);
}

options log;
