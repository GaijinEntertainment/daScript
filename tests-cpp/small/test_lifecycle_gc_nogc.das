options gen2

var g_frame = 0

[export]
def update() {
    var junk : array<float>
    junk |> resize(1024)
    junk[0] = float(g_frame)
    g_frame++
}
