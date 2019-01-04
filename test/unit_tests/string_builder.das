// options log=true,optimize=true,logOptimizationPasses=false

def test_arg(a:string):string
    return "hello,{a}!"

def test:bool
    let q:string = "blah" + "blah"
    let s:string = "hello,world!"
    let s2:string = "S = {s}"
    let s3:string = "S2 = \{{s2}}";
    let s4:string = "\"say\" 2 + 2 = {2+2}";
    assert(q=="blahblah")
    assert(s2=="S = hello,world!")
    assert(s3=="S2 = \{S = hello,world!}")
    assert(s4=="\"say\" 2 + 2 = 4")
    // print("{s}\n{s2}\n{s3}\n{s4}\n")
    // print("do u like that? {test_arg("string builder")} - cool,huh\n");
    return true
