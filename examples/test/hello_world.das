
options log

require extra_ann

[export,test_extra]
def foo
    print("bar")

[export]
def main
    var a = "blah, blah"
    a += ", blah"
    print("a = {a}\n")




