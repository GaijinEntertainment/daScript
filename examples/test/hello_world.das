options logCpp=true, logNodes=false

require math

def makeRandomSequence(src:array<string>)
    let n = 500000
    let mod = uint(n)
    resize(src,n)
    for i in range(0,n)
        let num = (271828183u ^ uint(i*119))%mod
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
    return true







