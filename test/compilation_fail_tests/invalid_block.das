expect 30801:2  // invalid_block
expect 30304    // function_not_found

def take_any(b:block)
    invoke(b)

def take_int(b:block<int>):int
    return invoke(b)

def test:bool
    let(a:int;b:float)
        take_any({ a++; })
        take_any() <|
            return true     // captured block can't return outside the block
        while true
            take_any() <|
                break       // captured block can't break outside the block
        while true
            take_any() <|
                while true
                    break   // this one ok
        take_int({ a++; })
        take_int({ b++; })  // no mtaching function ( block<float> )
    return true
