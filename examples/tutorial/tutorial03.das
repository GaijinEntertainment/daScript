require tutorial_03
require dump_fields

//[dump_fields]
class ColorExt
    color: tutorial_03::Color
    lumCached: float

[export]
def test
    unsafe
        //print("this tutorial binds custom type with fields and properties, as well as function which takes and returns this type\n")
        let c = [[ColorExt]]
        print("{typeinfo(is_struct c.color)}\n")
    //let c = [[ColorExt g=uint8(0x40), a=uint8(0xff)]]
    //debug(c, "c=")
    //print("c = {c}, luminance is {c.color}\n")
    //let cg = make_gray(c)
    //print("cg = {cg}\n")
    
