options gen2

module _init_seed_helper public

var private g_seed_set_by_init = 0

[init]
def private seed_at_init {
    g_seed_set_by_init = 7
}

def public seed_set_by_init() : int => g_seed_set_by_init
