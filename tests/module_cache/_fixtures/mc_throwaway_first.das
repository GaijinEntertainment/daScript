options gen2
options indenting = 4

require mc_throwaway_shared

[export]
def main {
    var t : tuple<mc_throwaway_helper : int; z : float>
    t.mc_throwaway_helper = 1
    print("MC_THROWAWAY_FIRST {take(t)}\n")
}
