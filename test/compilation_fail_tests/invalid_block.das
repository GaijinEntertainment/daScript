expect 30801  // invalid_block

def take_any(b:block)
    invoke(b)

def take_int(b:block<int>):int
    return invoke(b)

def test:bool
    let(a:int;b:float)
        take_any({ a++; })
        while true
            take_any() <|
                break       // captured block can't break outside the block
        while true
            take_any() <|
                while true
                    break   // this one ok
        take_int({ a++; })
        take_int({ b++; })  // no matching function ( block<float> )
    return true
