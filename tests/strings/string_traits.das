require dastest/testing_boost
require daslib/strings_boost
require daslib/faker
require daslib/fuzzer

[test]
def test_traits ( t:T? )
    t |> run("traits") <| @@ ( t : T? )
        t |> success(is_alpha('A'))
        t |> success(!is_alpha('9'))
        t |> success(is_new_line('\n'))
        t |> success(!is_new_line(' '))
        t |> success(is_white_space('\n'))
        t |> success(!is_white_space('#'))
        t |> success(is_number('3'))
        t |> success(!is_number('A'))
    t |> run("traits fuzz") <| @@ ( t : T? )
        var fake <- Faker()
        fuzz <|
            let Ch = fake |> any_char
            let ia = is_alpha(Ch)
            t |> success(ia==true || ia==false)
            let inl = is_new_line(Ch)
            t |> success(inl==true || inl==false)
            let iws = is_white_space(Ch)
            t |> success(iws==true || iws==false)
            let inn = is_number(Ch)
            t |> success(inn==true || inn==false)
