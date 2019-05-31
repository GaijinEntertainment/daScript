// options logInferPasses = true
// options logCpp=true


[export,unsafe]
def test:bool
    let t : tuple < int; float; string >
    debug(t,"\nt=")
    print("t={t}\n")
    print("sizeof t = {typeinfo(sizeof type tuple <int;float;string>)}\n")
    let q = t
    print("q={q}\n")
    return true

