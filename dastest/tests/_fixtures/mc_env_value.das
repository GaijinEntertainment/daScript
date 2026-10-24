options gen2
options indenting = 4

module mc_env_value

require mc_env_mod

let COMPILED = cache_env()

def compiled_value() : string => COMPILED
