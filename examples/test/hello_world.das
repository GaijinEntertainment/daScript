// options log = true

require rtti

// note: this is var because we want "sample" global variable to show up
var
    sample = "
require math
require UnitTest

let
    one_two = 12

enum TestEnum
    foo
    bar

[checkRange (foo=5)]
struct TestStruct
    a : int = 5
    b : string = \"something\"
    c : float[3][4]
    d : array<int>
    e : table<float; string>
    f : TestStruct?
    g : tuple<int; float; string>
    h : variant<i : int; f : float>
    fn : function<(a:int;b:float):string>
    lm : lambda<(a:int;b:float):string>

[export]
def bar ( blk : block<(a:int):float> )
    pass

[export]
def main
    print(\"hello world!\n\")
    return true
"

[export,unsafe]
def test
    // can specify .das_project here, or empty string for default file access
    // var access <- make_file_access("")

    // default access. this one won't open any files, other than introduced
    var access <- new FileAccess

    set_file_source(access, "sample.das", sample)

    // can pass null here, default file access will be created
    //compile_file("sample.das",[[FileAccessPtr]]) <| $ ( ok; prog; errors )

    compile_file("sample.das",access) <| $ ( ok; prog; errors )
        if ok
            print("compiled\n")
        else
            print("failed to compile:\n{errors}\n")
            assert(false)
    delete access
    return true
