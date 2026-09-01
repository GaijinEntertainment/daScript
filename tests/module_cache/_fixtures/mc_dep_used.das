options gen2
options indenting = 4

module mc_dep_used shared private

require mc_dep_mod

// the [dep_probe] carrier must live in a REQUIRED module: only required modules land in the
// module cache, so only here does the registered dependency reach a cached record
[dep_probe]
def public probed : int {
    return 42
}
