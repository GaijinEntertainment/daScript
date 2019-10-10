// options log_infer_passes=true

require UnitTest

struct S1
    a : int64 = 1l
    b : int64 = 2l
    c : int = 3

struct S2 : S1
    d : int = 4

[cpp_alignment]
struct CppS1
    a : int64 = 1l
    b : int64 = 2l
    c : int = 3

[cpp_alignment]
struct CppS2 : CppS1
    d : int = 4

[export]
def test()
    print("
        sizeof(S1)={typeinfo(sizeof type S1)}
        sizeof(S2)={typeinfo(sizeof type S2)}
        offsetof(S2.a)={typeinfo(offsetof<a> type S2)}
        offsetof(S2.b)={typeinfo(offsetof<b> type S2)}
        offsetof(S2.c)={typeinfo(offsetof<c> type S2)}
        offsetof(S2.d)={typeinfo(offsetof<d> type S2)}

        sizeof(CppS1)={typeinfo(sizeof type CppS1)}
        sizeof(CppS2)={typeinfo(sizeof type CppS2)}
        offsetof(CppS2.a)={typeinfo(offsetof<a> type CppS2)}
        offsetof(CppS2.b)={typeinfo(offsetof<b> type CppS2)}
        offsetof(CppS2.c)={typeinfo(offsetof<c> type CppS2)}
        offsetof(CppS2.d)={typeinfo(offsetof<d> type CppS2)}

        C++ sizeof(CppS1)={CppS1Size()}
        C++ sizeof(CppS2)={CppS2Size()}
        C++ offsetof(CppS2,d)={CppS2DOffset()}
    ")
    return true
