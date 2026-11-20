//! under `options no_aliasing` a construction stored through a
//! pointer of unknown origin is an alias error when it may read
//! memory: a constructor, or a field value that is not a constant
expect 30151:5
options gen2
options no_aliasing

var g_seed = 3

def seed() : int {
    return g_seed
}

struct GlobalInit {
    a : int = g_seed
}

struct CallInit {
    a : int = seed()
}

struct UserCtor {
    a : int
}

def UserCtor() {
    return UserCtor(uninitialized a = 1)
}

struct Plain {
    a : int
}

[export]
def global_init(var p : GlobalInit?) {
    *p = default<GlobalInit>
}

[export]
def call_init(var p : CallInit?) {
    *p = CallInit()
}

[export]
def user_ctor(var p : UserCtor?) {
    *p = default<UserCtor>
}

[export]
def from_argument(var p : Plain?; x : int) {
    *p = Plain(a = x)
}

[export]
def tuple_from_argument(var p : tuple<int; int>?; x : int) {
    *p = (1, x)
}
