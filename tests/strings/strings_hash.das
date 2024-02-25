require dastest/testing_boost
require daslib/faker
require daslib/fuzzer

/*

[test]
def test_hash_builder ( t:T? )
    var fake <- Faker()
    fuzz <|
        let str1 = fake |> any_string
        if str1 != ""
            let strHash = hash(str1)
            let hashBuilder = build_hash() <| $(bldr)
                bldr |> write(str1)
            t |> equal(strHash, hashBuilder)


[test]
def test_hash_builder4 ( t:T? )
    var fake <- Faker()
    fuzz <|
        let str1 = fake |> any_string
        let str2 = fake |> any_string
        let str3 = fake |> any_string
        let str4 = fake |> any_string
        if str1 != "" && str2 != "" && str3 != "" && str4 != ""
            let strHash = hash("{str1}{str2}{str3}{str4}")
            let hashBuilder = build_hash() <| $(bldr)
                bldr |> write(str1)
                bldr |> write(str2)
                bldr |> write(str3)
                bldr |> write(str4)
            t |> equal(strHash, hashBuilder)

*/
