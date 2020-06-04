// options debugger=true
// options log = true

// options string_heap_page=0
// options heap_page=0

require fio
require daslib/json
require daslib/json_enh

[export,unsafe]
def test
    for jidx in range(1,8)
        let fname = "C:/Users/Boris/Work/yzg/examples/test/json/{jidx}.json"
        // let fname = "/Users/borisbatkin/Work/yzg/examples/test/json/{jidx}.json"
        fopen(fname, "r") <| $ ( f )
            fmap(f) <| $ ( data )
                print ( "\n{fname}\n" )
                // print("before read {int(heap_bytes_allocated())} {int(string_heap_bytes_allocated())}\n")
                var error = ""
                var json = read_json(data, error)
                if json == null
                    panic("failed to parse, {error}\n")
                else
                    // macro test
                    if json is _object
                        print("object {int(heap_bytes_allocated())}\n")
                        (json as _object)["foo"] = JV("bar")
                    print("{write_json(json)}\n")
                    print("before delete {int(heap_bytes_allocated())} {int(string_heap_bytes_allocated())}\n")
                    delete json
                    print("before collect  {int(heap_bytes_allocated())} {int(string_heap_bytes_allocated())}\n")
                    heap_report()
                    string_heap_collect()
                    print("after collect {int(heap_bytes_allocated())} {int(string_heap_bytes_allocated())}\n")
    print("allocated {int(heap_bytes_allocated())} {int(string_heap_bytes_allocated())}\n")
    string_heap_collect()
    print("after collect {int(heap_bytes_allocated())} {int(string_heap_bytes_allocated())}\n")
    return true
