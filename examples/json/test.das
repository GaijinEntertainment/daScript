// options log = true

require RapidJson

// options log=true, logInferPasses=false, optimize=true

struct Foo
    i : int = 5
    f : float = 6.0
    s : string = "hello"
    n : int64 = 100500100500l

struct Bar
    i : int
    j : int[3] = [[int 1;2;3]]
    t : array<float>
    k : table<string,int>
    f : Foo

def test_write
    let t <- Bar()
    t.f = Foo()
    push(t.t,1.2)
    push(t.t,1.3)
    t.k["foo"] = 1
    t.k["bar"] = 2
    writeJson(t) <| $ ( root )
        print("{root}\n")
    return true


[export]
def test
    test_write()
    test_parse()
    return true

def test_parse
    let json_text = "\{ \"hello\": \"world\", \"t\": true , \"f\": false, \"n\": null, \"i\": 123, \"pi\": 3.1416, \"a\": [1, 2, 3, 4] \}"

    readJson(json_text) <| $ ( root, error )
        // general navigation
        print("{root}\n")
        assert(root.is_object)
        let r_n = root.n
        assert(r_n.is_null)
        let r_t = root.t
        assert(r_t.is_bool & r_t.as_bool==true)
        let r_i = root.i
        assert(r_i.is_int & r_i.as_int==123)
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
        // safe navigation
        let safe_i = root.i?.as_int ?? 456
        assert(safe_i==123)
        let safe_j = root?.j?.as_int ?? 456
        assert(safe_j==456)
        let safe_jj = root?.j?.j?.as_float ?? 1.0f
        assert(safe_jj==1.0f)
        // setter
        set(root.i,13)
        assert(root.i.as_int==13)
        set(root.t,false)
        assert(root.t.as_bool==false)
        set(root.hello,"hello")
        assert(root.hello.as_string=="hello")
        set(root.pi, 1.23)
        assert(root.pi.as_float==1.23)
	return true
