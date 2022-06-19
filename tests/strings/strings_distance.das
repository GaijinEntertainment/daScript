require dastest/testing_boost
require daslib/strings_boost


[test]
def test_levenshtein_distance ( t:T? )
    t |> run("zero distance") <| @@ ( t : T? )
        t |> equal(levenshtein_distance("kitten", "kitten"), 0)
        t |> equal(levenshtein_distance_fast("kitten", "kitten"), 0)

    t |> run("kitten vs sitting") <| @@ ( t : T? )
        t |> equal(levenshtein_distance("kitten", "sitting"), 3)
        t |> equal(levenshtein_distance_fast("kitten", "sitting"), 3)
        
    t |> run("kitten vs sitting") <| @@ ( t : T? )
        t |> equal(levenshtein_distance("GUMBO", "GAMBOL"), 2)
        t |> equal(levenshtein_distance_fast("GUMBO", "GAMBOL"), 2)

    t |> run("Saturday vs Sunday") <| @@ ( t : T? )
        t |> equal(levenshtein_distance("Saturday", "Sunday"), 3)
        t |> equal(levenshtein_distance_fast("Saturday", "Sunday"), 3)
