options gen2
options indenting = 4

require mc_throwaway_shared
require mc_throwaway_helper

[export]
def main {
    var t : tuple<mc_throwaway_helper : int; z : float>
    t.mc_throwaway_helper = 2
    print("MC_THROWAWAY_SECOND {take(t) + one()}\n")
}
