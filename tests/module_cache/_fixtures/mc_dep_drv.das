options gen2
options indenting = 4

require mc_dep_used

// plain main-line driver (NOT a dastest child): the module cache serves the main compile's
// require chain, and only there does mc_dep_used land in a cached record
[export]
def main {
    print("PROBED={probed()}\n")
}
