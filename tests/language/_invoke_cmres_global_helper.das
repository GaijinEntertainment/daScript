options gen2

module _invoke_cmres_global_helper public

var g_pair : tuple<int; int>

def set_pair(fn : function<() : tuple<int; int>>) {
    g_pair = invoke(fn)
}
