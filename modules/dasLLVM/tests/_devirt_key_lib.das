options gen2
module _devirt_key_lib public

class Plain {
    def read : int => 1
}

def use_plain(var p : Plain?) : int => p->read()
