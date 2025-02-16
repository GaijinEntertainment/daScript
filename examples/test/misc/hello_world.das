require t_counter

[export]
def main()
    unsafe
        var a : $counter(type<int>)
        debug(a)

options log_infer_passes


