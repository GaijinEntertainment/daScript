struct L3
    a:int

struct L2
    l3:L3?

struct L1
    l2:L2?

def main
    let(b:L3[10])
        b.a = 0

def test:bool
    main()
    return true

