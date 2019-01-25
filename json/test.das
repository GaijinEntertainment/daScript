options log = true

require RapidJson

[export]
def test
    let json_text = "\{ \"hello\": \"world\", \"t\": true , \"f\": false, \"n\": null, \"i\": 123, \"pi\": 3.1416, \"a\": [1, 2, 3, 4] \}"

    let root = readJson(json_text)

    debug(root)
    assert(root.is_object)

    let r_t = root.t
    assert(r_t.is_bool & r_t.as_bool==true)

    let r_f = root.f
    assert(r_f.is_bool & r_f.as_bool==false)

    let r_hello = root->["hello"]   // same as root.hello
    assert(r_hello.is_string & r_hello.as_string=="world")

    let r_pi = root.pi
    assert(r_pi.is_float & r_pi.as_float==3.1416)

    let r_a = root.a
    assert(r_a.is_array & r_a.size==4)

    for t in range(0,r_a.size)
        let val = r_a->[t]
        assert(val.is_int & val.as_int==(t+1))

	return true

