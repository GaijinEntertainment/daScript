options gen2
options solid_context

var g_cells : int[16]
var g_count = 0
let shared g_table : int[4] = fixed_array(1, 2, 3, 4)

[export]
def fill_cells : int {
    for (i in range(16)) {
        g_cells[i] = i * 3
    }
    g_count = 16
    var total = 0
    for (c in g_cells) {
        total += c
    }
    return total + g_count + g_table[3]
}
