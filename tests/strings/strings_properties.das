require dastest/testing_boost
require daslib/strings_boost
require daslib/faker
require daslib/fuzzer
require daslib/regex

[test]
def test_properties ( t:T? )
    t |> run("length") <| @@ ( t : T? )
        var fake <- Faker()
        let limit = re_gen_get_rep_limit()
        fuzz <|
            let str = fake |> any_string
            let len = str |> length
            t |> success(len>=0 && len<int(limit))
        fuzz(100) <|
            let str = fake |> long_string
            let len = str |> length
            t |> success(len>=0 && len<int(fake.max_long_string))

    t |> run("empty") <| @@ ( t : T? )
        var fake <- Faker()
        let limit = re_gen_get_rep_limit()
        fuzz <|
            let str = fake |> any_string
            let e = str |> empty
            t |> success(e==false || e==true)

    t |> run("starts_with") <| @@ ( t : T? )
        var fake <- Faker()
        let limit = re_gen_get_rep_limit()
        fuzz <|
            let str = fake |> any_string
            let cmp = fake |> any_string
            let sw = starts_with(str,cmp)
            t |> success(sw==true || sw==false)

    t |> run("ends_with") <| @@ ( t : T? )
        var fake <- Faker()
        let limit = re_gen_get_rep_limit()
        fuzz <|
            let str = fake |> any_string
            let cmp = fake |> any_string
            let sw = ends_with(str,cmp)
            t |> success(sw==true || sw==false)

