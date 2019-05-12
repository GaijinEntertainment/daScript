// options log = true, logCpp=true
// options logInferPasses=true

require fio

[export]
def test
    fopen("test.txt","w") <| $(f:FILE?)
        if f!=null
            fprint(f,"hello,world!\n")
        else
            print("can't open test.txt")
    return true
