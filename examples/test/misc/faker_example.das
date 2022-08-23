require daslib/faker

[export]
def main
    fake_positive_int(10) <| $ ( str )
        print("positive int {str}\n")
    fake_int(10) <| $ ( str )
        print("int {str}\n")
