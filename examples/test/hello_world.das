options log = true

options log_nodes = true

var
    FMAX = FLT_MAX

def logit(t)
    print("t = {t}\n")

[export]
def test
    let t = FLT_MAX
    logit(t)
    return true

