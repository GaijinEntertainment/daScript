expect 31101:2, 31102:9, 31100:1, 30102:3, 30108:2, 30103, 30304:1

let
    a:auto                      // 31101, can't infer, need initializer

let
    b = a                       // 31102, can't infer, auto to auto

def test2(a:auto)               // 31100, genertics are not supported yet
    assert(false)

def blk(b:block<(a:int):int>) : int
    return invoke(b,3)

def test3
    let v1 = b                          // 31102, can't infer, auto to auto
    let v2 : auto& = 13                 // 31102, can't infer, auto& to int (needs to be ref)
    let x = 14  // x:int
    let v3 : auto& = x                  // 30108, local varialbe can't be reference
    let arr : int[2][3]
    let arr2 : auto[2][3] = arr
    let v4 : auto[3][2] = arr           // 31102, can't infer, array dimension mismatch
    let v5 : auto? = x                  // 31102, can't infer, expecting pointer
    let pT : int?
    let ppT : int? ?
    let ppA : auto? ? = ppT
    let v6 : auto? ? = pT               // 31102, can't infer, expecting pointer to pointer
    blk() <| $(a:auto):int              // 31101, block argument can't be infered
        return a                        // 30102, incompatible return type
    blk() <| $(a:int):auto?
        return a                        // 31102, block argument can't be infered,  30102, incompatible return type
    blk() <| $(a:auto?=3):int           // 31101, block argument can't be infered,  30103, block argument default value type mismatch
        return a                        // 30102, incompatible return type
    let tab : table<string,int>
    let tabI : table<auto,float> = tab  // 31102: local variable initialization type can't be infered
