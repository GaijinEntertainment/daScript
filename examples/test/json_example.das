// options debugger=true

require fio
require json
require json_enh

[export]
def test
    for jidx in range(1,5)
        let fname = "C:/Users/Boris/Work/yzg/examples/test/json/{jidx}.json"
        fopen(fname, "r") <| $ ( f )
            fmap(f) <| $ ( data )
                print ( "\n{fname}\n" )
                var error = ""
                var json = read_json(data, error)
                if json == null
                    print("failed to parse, {error}\n")
                else
                    // macro test
                    if json is _object
                        print("object\n")
                        (json as _object)["foo"] = new JsonValue("bar")
                    print("{write_json(json)}\n")

    return true
