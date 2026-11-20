//! under `options no_aliasing` a construction stored through a
//! pointer of unknown origin is an alias error when it runs an
//! initializer or a constructor, which may read memory
expect 30151:3
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
