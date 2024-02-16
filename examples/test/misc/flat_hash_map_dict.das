require math

require daslib/flat_hash_map

%spoof_instance~TFlatHashMap(FlatHashMap_string_to_int,string,int)%%;

def makeRandomSequence(var src:array<string>)
    let n = 500000
    let mod = uint(n)
    resize(src,n)
    for i in range(n)
        let num = (271828183u ^ uint(i*119))%mod
        src[i] = "{num}"

def dict(var tab:FlatHashMap_string_to_int; src:array<string>)
    clear(tab)
    var maxOcc = 0
    for s in src
        maxOcc = max(++tab[s],maxOcc)
    return maxOcc

[export]
def main
    var tab <- FlatHashMap_string_to_int(64)
    var src : array<string>
    makeRandomSequence(src)
	profile(20,"dictionary") <|
        dict(tab,src)

