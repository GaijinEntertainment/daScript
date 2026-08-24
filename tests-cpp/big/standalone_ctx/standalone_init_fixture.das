options gen2

require standalone_init_dep

var g_stamp = 0

def next_stamp : int {
    g_stamp ++
    return g_stamp
}

def read_forward : int {
    return g_later
}

var g_first = next_stamp() + g_dep
var g_second = next_stamp()
var g_reads_forward = read_forward() + 100
var g_later = 7
var g_init_fn_stamp = 0

[init]
def record_init {
    g_init_fn_stamp = next_stamp()
}

[export]
def get_first : int {
    return g_first
}

[export]
def get_second : int {
    return g_second
}

[export]
def get_init_fn_stamp : int {
    return g_init_fn_stamp
}

[export]
def get_reads_forward : int {
    return g_reads_forward
}

[export]
def get_later : int {
    return g_later
}
