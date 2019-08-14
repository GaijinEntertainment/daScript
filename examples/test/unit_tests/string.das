// options log=true,optimize=true,logOptimizationPasses=false

// options logCpp=true

[export]
def test:bool
    let s:string = "hello, world!"
    assert(toupper(s) == "HELLO, WORLD!")
    assert(tolower(toupper(s)) == s)
    assert(reverse(s) == "!dlrow ,olleh")
    assert(length(s) == 13)
    assert(find(s, "world!") == 7)
    assert(find(s, "world!", 4) == 7)
    assert(find(s, "o") == 4)
    assert(find(s, "o", 5) == 8)
    assert(find(s, "o", 9) == -1)
    assert(find(s, "??") == -1)
    assert(endswith(s, "world!"))
    assert(startswith(s, "hello"))
    assert(startswith(s, ""))
    assert(endswith(s, ""))
    assert(startswith(s, "a") == false)
    assert(endswith(s, "b") == false)
    let s2:string = "   hello, world!   "
    assert(strip(s2) == s)
    assert(strip_right(strip_left(s2)) == s)
    assert(strip_left(s2) == "hello, world!   ")
    assert(strip_right(s2) == "   hello, world!")
    assert(slice(s, 7) == "world!")
    assert(slice(s, -6) == "world!")
    assert(slice(s, 6, -6) == " ")
    assert(slice(s, 6, 7) == " ")
    assert(int("11") == 11)
    assert(int("-12") == -12)
    assert(uint("11") == 11U)
    assert(float("-12") == -12.)
    assert(float("1e1") == 10.)
    try
        debug(int("11abc"), "=")
        assert(false, "can't convert non int string without exception")
    recover {}
    assert(to_int("11abc") == 11)
    assert(to_float("11.1abc") == 11.1)
    let a_slash_slash = "ab\\xy"
    assert(length(a_slash_slash)==5)
    let mls = "this is
multi line string"
    assert(mls=="this is\nmulti line string")
    assert(format("%02x",26)=="1a")
    assert(format("%02x",27u)=="1b")
    assert(format("%02x",28l)=="1c")
    assert(format("%02x",29ul)=="1d")
    assert(format("%.1f",1.23)=="1.2")
    assert(format("%.2f",1.234)=="1.23")
    // string builder
    let st = build_string() <| $ (var writer : StringBuilderWriter)
        write(writer,"255 hex is ")
        format(writer,"%2x",255)
    assert(st=="255 hex is ff")
    // string iterator
    var count = 0
    var index = 0
    for ch in st
        if ch=='5'
            count ++
        let chAt = characterat(st,index)
        assert(ch==chAt)
        index ++
    assert(count==2)
    return true
