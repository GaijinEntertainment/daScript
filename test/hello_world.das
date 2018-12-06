def main
    print("hello,world!\n")

def it(a:array<int>):array<int>
    debug(a)
    push(a,length(a)+1)
    debug(a)
    return a

def test
    let(a:array<int>)
        debug(a)
        it(it(it(a)))

