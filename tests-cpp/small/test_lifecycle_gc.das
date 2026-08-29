options gen2
options gc
options persistent_heap = true

var g_frame = 0
var g_keep : array<float>

[export]
def init() {
    g_keep |> resize(1024)
}

[export]
def update() {
    var junk : array<float>
    junk |> resize(16384)
    junk[0] = float(g_frame)
    let s = "frame {g_frame} " + "pad" + string(g_frame * 7)
    g_keep[g_frame % 1024] = junk[0] + float(length(s))
    g_frame++
}
