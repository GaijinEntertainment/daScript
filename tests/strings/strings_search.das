require dastest/testing_boost
require daslib/strings_boost
require daslib/faker
require daslib/fuzzer
require daslib/regex

[test]
def test_search ( t:T? )

    t |> run("fuzz find") <| @@ ( t : T? )
        t |> equal ( find("123","23"), 1 )
        t |> equal ( find("123","22"), -1 )
        var fake <- Faker()
        fuzz <|
            let str = fake |> any_string
            let substr = fake |> any_string
            t |> accept(find(str,substr))

    t |> run("fuzz find") <| @@ ( t : T? )
        t |> equal ( find("123",'2'), 1 )
        t |> equal ( find("123",'4'), -1 )
        var fake <- Faker()
        fuzz <|
            let str = fake |> any_string
            let substr = fake |> random_int % 256
            t |> accept(find(str,substr))

    t |> run("fuzz find1") <| @@ ( t : T? )
        t |> equal ( find("123","23",1), 1 )
        t |> equal ( find("123","23",2), -1 )
        let limit = int(re_gen_get_rep_limit())*2
        var fake <- Faker()
        fuzz <|
            let str = fake |> any_string
            let substr = fake |> any_string
            let start = fake |> random_int % limit
            t |> accept(find(str,substr,start))

    t |> run("fuzz find1") <| @@ ( t : T? )
        t |> equal ( find("123",'2',1), 1 )
        t |> equal ( find("123",'2',2), -1 )
        let limit = int(re_gen_get_rep_limit())*2
        var fake <- Faker()
        fuzz <|
            let str = fake |> any_string
            let substr = fake |> random_int % 255
            let start = fake |> random_int % limit
            t |> accept(find(str,substr,start))
