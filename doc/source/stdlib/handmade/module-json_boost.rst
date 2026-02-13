The JSON_BOOST module extends JSON support with operator overloads for convenient
field access (``?[]``), null-coalescing (``??``), and automatic struct-to-JSON
conversion macros (``from_JsValue``, ``to_JsValue``).

All functions and symbols are in "json_boost" module, use require to get access to it. ::

    require daslib/json_boost

Example: ::

    require daslib/json_boost

        [export]
        def main() {
            let data = "\{ \"name\": \"Alice\", \"age\": 30 \}"
            var error = ""
            var js <- read_json(data, error)
            if (error == "") {
                let name = js?.name ?? "?"
                print("name = {name}\n")
                let age = js?.age ?? -1
                print("age = {age}\n")
            }
            unsafe {
                delete js
            }
        }
        // output:
        // name = Alice
        // age = 30
