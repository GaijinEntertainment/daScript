options gen2
options indenting = 4

require test_dasbind_cache_reg

[export]
def probe : int {
    return c_abs(-7)
}
