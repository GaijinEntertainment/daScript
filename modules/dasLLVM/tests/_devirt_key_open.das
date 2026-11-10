options gen2

require _devirt_key_lib

class Sub : Plain {
    def override read : int => 2
}

[export]
def main : int => use_plain(new Sub())
