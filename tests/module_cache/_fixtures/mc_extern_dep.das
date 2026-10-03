options gen2
options indenting = 4

module mc_extern_dep

require mc_extern_lib
require dasbind

def mc_extern_dep_probe() : int {
    return mc_extern_probe() + 1
}
