options gen2

require _standalone_dep_call

[export]
def call_foreign(x : int) : int {
    var probe = new DepProbe()
    let result = dep_probe(x) + probe->method(x)
    unsafe {
        delete probe
    }
    return result
}
