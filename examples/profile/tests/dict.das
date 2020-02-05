// options log=true, print_var_access=true, print_ref=true

require math

// enable for test purposes only. this is slower
// options intern_strings = true

// 8Mb heap
options string_heap=8388608

require testProfile

def makeRandomSequence(var src:array<string>)
    let n = 500000
    let mod = uint(n)
    resize(src,n)
    for i in range(0,n)
        let num = (271828183u ^ uint(i*119))%mod
        src[i] = "{num}"

def dict(var tab:table<string;int>; src:array<string>)
    clear(tab)
    var maxOcc = 0
    for s in src
        maxOcc = max(++tab[s],maxOcc)
    return maxOcc

[export]
def test
    var tab : table<string;int>
    var src : array<string>
    makeRandomSequence(src)
	profile(20,"dictionary") <|
        dict(tab,src)
    profile(20,"c++ dictionary") <|
        testProfile::testDict(src)
	// print("string heap depth is {string_heap_depth()}\n")
    verify(string_heap_depth()==1)
    return true

