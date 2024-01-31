require rtti

require test_reader

let foo = %suffix~
def test_me
    debug(1,"here")
    print("haha\n")
    debug(2,"there")
%%

[export]
def main
    test_me()

