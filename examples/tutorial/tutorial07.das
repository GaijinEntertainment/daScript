options gen2
options log

require tutorial_07

[export]
def take_2(a : int; b : float) {
    print("take_2 a = {a} b = {b}\n")
}

[export]
def test_cmres(a : int, b : float) {
    print("test_cmres a = {a} b = {b}\n")
    return FooBar(foo = a, bar = b)
}

def callback(a : int, b : float) : string {
    print("callback a = {a} b = {b}\n")
    return "{a};{b}"
}

[export]
def test {
    print("this tutorial implements C interfaces\n")
    das_c_func(13)
    das_c_str_func("hello from das")
    var result = das_callback_func(@@callback, 42, 3.14)
    print("result from callback: {result}\n")

    var captured = 100
    var lmb <- @(a : int, b : float) : string {
        print("lambda callback a = {a} b = {b}\n")
        captured += a
        return "{a};{b};{captured}"
    }
    print("result from lambda: {das_lambda_func(lmb, 10, 2.71)}\n")
    print("result from lambda: {das_lambda_func(lmb, 20, 1.41)}\n")

    var blk <- $(a : int, b : float) {
        print("block callback a = {a} b = {b}\n")
        captured += a
        return "{a};{b};{captured}"
    }
    print("result from block: {das_block_func(blk, 5, 0.577)}\n")
    print("result from block: {das_block_func(blk, 15, 1.618)}\n")

    var a : intarray
    for (x in range(3)) {
        push(a, x)
    }
    print("a = {a}\n")

    var fb : FooBar
    fb.foo = 1
    fb.bar = 2.0
    print("fb = {fb}\n")
    var fb2 : FooBar
    fb2 <- fb
    print("fb2 = {fb2} fb = {fb}\n")

    let t = OneTwo.one
    print("t = {t} {OneTwo.two}\n")

    ////////////////
    // model example
    ////////////////

    let pos <- [ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 ]
    var mdl = make_model(pos)
    if (mdl == null) {
        print("model is null\n")
    } else {
        mdl.set_smooth_shading(true)
        debug(mdl, "model after set_smooth_shading")
    }
}


