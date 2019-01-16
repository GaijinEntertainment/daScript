// options log=true, printVarAccess=true, printRef=true

require testProfile

let
    RND = 0u

def lcg
    RND = RND*1664525u + 1013904223u
    return RND

def makeRandomSequence(src:array<string>)
    let n = 500000
    let mod = uint(n)
    resize(src,n)
    for i in range(0,n)
        let num = (lcg() % mod) * 271828183u
        src[i] = "{num}"

def dict(tab:table<string,int>; src:array<string>)
    clear(tab)
    let maxOcc = 0
    for s in src
        maxOcc = max(++tab[s],maxOcc)
    return maxOcc

[export]
def test
    let tab : table<string,int>
    let src : array<string>
    makeRandomSequence(src)
	profile(20,"dictionary") <|
        dict(tab,src)
    profile(20,"c++ dictionary") <|
        testProfile::testDict(src)
    return true

