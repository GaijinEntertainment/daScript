// options debugger=true

require fio
require daslib/json
require daslib/json_enh

[export]
def test
    for jidx in range(1,8)
        let fname = "C:/Users/Boris/Work/yzg/examples/test/json/{jidx}.json"
        fopen(fname, "r") <| $ ( f )
            fmap(f) <| $ ( data )
                print ( "\n{fname}\n" )
                var error = ""
                var json = read_json(data, error)
                if json == null
                    panic("failed to parse, {error}\n")
                else
                    // macro test
                    if json is _object
                        print("object\n")
                        (json as _object)["foo"] = JV("bar")
                    print("{write_json(json)}\n")

    return true
