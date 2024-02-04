options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

module colors shared

def public bold(str: string)
    return "\x1B[1m{str}\x1B[0m"

def public red_str(str: string; color)
    return color ? "\x1B[31m{str}\x1B[0m" : str

def public green_str(str: string; color)
    return color ? "\x1B[32m{str}\x1B[0m" : str

def public yellow_str(str: string; color)
    return color ? "\x1B[33m{str}\x1B[0m" : str

def public blue_str(str: string; color)
    return color ? "\x1B[34m{str}\x1B[0m" : str
