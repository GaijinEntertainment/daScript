require fio

[export]
def main
    unsafe
        popen("dir") <| $ ( f )
            if f != null
                while !feof(f)
                    let st = fgets(f)
                    print("{st}")
