options log = true

require macros

[export]
def test
    var hints <- [[HintLookup
        tab <- {{
            hash("one") => "one";
            hash("two") => "two"
        }}
    ]]
    let h1 = get_hint(hints, "one", "no hint")
    let h2 = get_hint(hints, "three", "no hint")
    print ( "h1 = {h1}\nh2 = {h2}\n")
    return true
