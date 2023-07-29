require emoji

[export]
def main
    let l1 = "U0001F596U0001F48E"
    let l2 = fix_broken_utf32(l1)
    print("l1={l1} l2={l2}\n")

    // utf32 sad face u1f622
    let l3 = "smily u1f600 and sad u1f622"
    let l4 = fix_broken_utf32(l3)
    print("l3={l3} l4={l4}\n")
