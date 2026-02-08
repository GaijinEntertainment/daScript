options gen2
options log

require tutorial_07

[export]
def take_2(a : int; b : float) {
    print("take_2 a = {a} b = {b}\n")
}

[export]
def test {
    print("this tutorial implements C interfaces\n")
    das_c_func(13)
    das_c_str_func("hello from das")

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


