// options log = true
// options log_aot = true

require daslib/regex
require fio

let
    test_and_log = false

[export,unsafe]
def test
    static_if test_and_log
        test_regex()

    var r1:Regex // email
    if !regex_compile(r1,"[\\w\\.+-]+@[\\w\\.-]+\\.[\\w\\.-]+")
        panic("can't compile")
    static_if test_and_log
        regex_debug(r1)

    let test = "first.last@learnxinyminutes.com"
    let res = regex_match(r1,test)
    assert(res!=-1)

    let test2 = "first"
    let res2 = regex_match(r1,test2)
    assert(res2==-1)

    // regex_debug(r1)
    let root = get_das_root()
    let input = root + "/../REGEX/input-text.txt"
    fopen(input,"rb") <| $( f )
        if f != null
            var dict : table<string; int>
            fmap(f) <| $(data)
                var text = clone_string(data)
                profile(5,"regex email") <|
                    regex_foreach(r1,text) <| $ ( r )
                        static_if test_and_log
                            print("match {slice(text,r.x,r.y)}\n")
                        return true
                delete_string(text)
        else
            print("can't open ")
    return true

[unsafe]
def test_re ( reg:string; matches:array<string>; nonmatches:array<string> )
    print("testing {reg}\n")
    var regex : Regex
    if !regex_compile(regex, reg)
        print("failed to compile\n")
        assert(false)
    print("re is:\n")
    regex_debug(regex)
    var failed = false
    for m in matches
        let ma = regex_match(regex, m)
        print("matching `{m}` -> {ma} {ma==-1 ? "failed" : "ok"}\n")
        failed |= ma==-1
    for m in nonmatches
        let ma = regex_match(regex, m)
        print("^matching `{m}` -> {ma} {ma!=-1 ? "failed" : "ok"}\n")
        failed |= ma!=-1
    print("{failed ? "failed" : "ok"}\n\n")
    assert(!failed)
    delete regex

[unsafe]
def test_group ( reg:string; match:string; groups:array<string>)
    print("testing groups is {reg}\n")
    var regex : Regex
    if !regex_compile(regex, reg)
        print("failed to compile\n")
        assert(false)
    print("re is:\n")
    regex_debug(regex)
    if length(regex.groups)==0
        print("has no groups\n")
        assert(false)
    let ma = regex_match(regex, match)
    assert(ma!=-1,"failed to match\n")
    let tot = length(regex.groups) - 1
    assert(tot==length(groups),"group count mismatch")
    var failed = false
    for x in range(0,tot)
        let grp = regex_group(regex,x+1,match)
        let cgrp = groups[x]
        if grp != cgrp
            print("failed, expecting {x}={cgrp}, got {grp}")
            failed = true
    assert(!failed)
    delete regex


[unsafe]
def test_regex

    // character
    test_re("a", [{string[] "a"}], [{string ""; "b"; "ba"}])
    // union
    test_re("cat|dog", [{string "cat";"dog";"cats"}], [{string "at";" cat";"doog"}])
    test_re("cat|dog|bat", [{string "cat";"dog";"cats";"bats"}], [{string "at";" cat";"doog";"at"}])
    // set & negatvie set
    test_re("[0-9a-zA-Z_]",[{string "0";"3";"9";"a";"e";"z";"A";"Q";"Z";"_"}], [{string "#";"*"}])
    test_re("[\\w]",[{string "0";"3";"9";"a";"e";"z";"A";"Q";"Z";"_"}], [{string "#";"*"}])
    test_re("\\w",[{string "0";"3";"9";"a";"e";"z";"A";"Q";"Z";"_"}], [{string "#";"*"}])
    test_re("[^0-9a-zA-Z_]",[{string "#";"*"}],[{string "0";"3";"9";"a";"e";"z";"A";"Q";"Z";"_"}])
    // any, concat
    test_re("cat.", [{string "catt";"cats"}], [{string "cat";" cat"}])
    test_re("(cat)(dog)(bat)", [{string[] "catdogbat"}], [{string "cat";" cat"; "catdog"; "dogbat"}])
    // eos
    test_re("cat$", [{string[] "cat"}], [{string "cattt";" cat"}])
    // +
    test_re("ab+", [{string "ab";"abb";"abbbc"}], [{string "a";" ab"; "bbb"}])
    test_re("(cat)+",[{string "cat";"catcat"}], [{string "dog";"caat"}])
    // *
    test_re("ab*", [{string "a";"ab";"abb";"abbbc"}], [{string " ab"; "bbb"}])
    test_re("a(cat)*",[{string "a";"acat";"acatcat"}], [{string "dog";"caat";"cat"}])
    test_re("a*(cat)",[{string "cat";"acat";"aaacat"}], [{string "dog";"caat"}])
    // ?
    test_re("ab?", [{string "a";"ab";"abb"}], [{string "b";" ab"; "bbb"}])
    test_re("(cat)?x",[{string "catx";"x"}], [{string "dog";"caat"}])
    // match2
    test_re("[a-z.]+.com", [{string[] "abra.com"}], [{string "1"; "2"}])

    // gropus
    test_group ( "i have a (cat|dog)", "i have a cat.", [{string[] "cat"}])
    test_group ( "(this|that) is a (book|table|car)", "that is a table", [{string "that";"table"}])

