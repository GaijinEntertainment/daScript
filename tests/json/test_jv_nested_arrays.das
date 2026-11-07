options gen2
require dastest/testing_boost public
require daslib/json_boost
require strings

struct Sample {
    name : string
    values : array<double>
}

struct Group {
    name : string
    samples : array<Sample>
}

struct Record {
    program : string
    groups : array<Group>
}

[test]
def test_jv_of_three_struct_array_levels(t : T?) {
    var r = Record(program = "p")
    var g = Group(name = "g")
    g.samples |> emplace(Sample(name = "s", values <- [0.5lf, 0.6lf]))
    r.groups |> emplace(g)
    let text = write_json(JV(r))
    t |> success(find(text, "\"values\"") >= 0, "the inner array is in the JSON: {text}")
    var err : string
    let js = read_json(text, err)
    t |> equal(err, "")
    let back = from_JV(js, type<Record>)
    t |> equal(back.groups[0].samples[0].values[1], 0.6lf)
}
