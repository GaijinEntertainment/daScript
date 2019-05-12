// options log = true, logCpp=true
// options logInferPasses=true

require fio

[export]
def test
    var st : FStat
    fopen("test.txt","w") <| $(f:FILE?)
        if f!=null
            fprint(f,"hello,world!\n")
            if fstat(f,st)
                print("fstat size = {st.size}, mtime = {st.mtime}\n")
        else
            print("can't open test.txt")
    if stat("test.txt",st)
        print("stat size = {st.size}, mtime = {st.mtime}\n")
    else
        print("can't get stats\n")
    let path = "."
    dir(path) <| $(filename:string)
        let fst = stat(path+"/"+filename)
        if fst.is_dir
            print("{filename} <DIR>\n")
        else
            print("{filename} {fst.size} {fst.atime}\n")
    if mkdir("gen")
        print("GEN\n")
    return true
