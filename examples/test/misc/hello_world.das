struct Foo
    a : int

[export]
def main
    let tab : table<string; Foo>
    get(tab, "default") <| $(f:Foo)
        print("found\n")

options log
