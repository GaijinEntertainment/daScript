options gen2
options gc
options persistent_heap

// a program that collects its own heaps - the context must carry
// both flags the collector checks, and a debug info per global
// that can hold heap data, for the collector to walk

var g_collects = 0
var g_names : array<string>

[export]
def collect_now : int {
    unsafe {
        heap_collect(true, false)
    }
    g_collects++
    g_names |> push("collect {g_collects}")
    return length(g_names)
}
