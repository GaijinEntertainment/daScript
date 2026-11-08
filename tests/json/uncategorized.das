options gen2

require dastest/testing_boost public
require daslib/json_boost
require daslib/stringify
require strings

let broken1 = %stringify~
{
    "mood" : "🙏",
    "search" : "Pater Noster translation",
    "message" : "Sure, I can help you out with that. Pater Noster is Latin for 'Our Father'. Here's an English translation of the prayer:" + "Our Father, who art in heaven," + "hallowed be thy name;" + "thy kingdom come;" + "thy will be done on earth as it is in heaven." + "Give us this day our daily bread;" + "and forgive us our trespasses" + "as we forgive those who trespass against us;" + "and lead us not into temptation," + "but deliver us from evil. Amen." + "Hope that helps, kid.",
    "order" : null,
    "raw" : null,
    "result" : "search",
    "question" : null,
    "summary" : null,
    "draw" : null
}
%%

let broken2 = %stringify~
{
    "mood" : "😒💬",
    "search" : null,
    "message" : "You come to me, on this day, and ask me of firearms? Fine, I'll bite. The M1 Garand and M1A1 are both semi-automatic rifles, but there are some important differences between them. The M1 Garand was the standard service rifle of the US Army from 1936 until 1957, while the M1A1 is a modern civilian version of the M1 Garand, updated with a few different features. The M1A1 has a short barrel with a flash hider, a synthetic stock, and a top-mounted Picatinny rail for mounting optics, while the M1 Garand is more of a traditional rifle with a longer barrel and a wooden stock. The M1 Garand also has a distinctive "ping" sound when it ejects its empty clip, while the M1A1 has been updated to use detachable magazines.",
    "order" : null,
    "raw" : null,
    "result" : "question",
    "summary" : "difference between M1 Garand rifle and M1A1 rifle",
    "question" : "Could you explain me what's the difference between M1 Garand rifle and M1A1 rifle?",
    "draw" : null
}
%%

let broken3 = %stringify~
{
    "mood" : "😠",
    "message" : "You want me to draw you a picture of my mood? How about I draw you a picture of a horse's head instead? It might be more useful for you. Don't waste my time with nonsense.",
    "result" : "draw",
    "draw" : "A very detailed drawing of a horse's head.",
    "summary" : "User is asking me to draw a picture of my mood.",
}
%%

let broken4 = %stringify~
    {
    "result" : "nothing",
    "summary" : "User asked for the difference between the words: limit, border, and frontier",
    "message" : "Ah, Greg, you come to me on the day of my daughter's wedding and ask me about the difference between limit, border, and frontier. I will give you the definitions of each word so that you can understand the differences: n- A limit is a point beyond which something does not or may not extend or pass. n- A border is the outer part or edge of something. It can also mean the line or frontier area separating political or geographical regions. n- A frontier is a line or zone of contact between two adjoining countries, political systems, or groups, where either one or both are expanding or could expand."
    "draw" : null,
    "mood" : "🤨",
    "code" : null,
    }
%%

let broken5 = %stringify~ {\u004857"code" : "",\u004857"message" : "I'm doing well, thank you for asking. How can I help you?",\u004857"result" : "nothing",\u004857"draw" : ""}\u004858 %%

def test_broken(t : T?; text : string) {
    var error : string
    var j = read_json(text, error)
    t |> equal(null, j)
    let fixed = try_fixing_broken_json(text)
    j = read_json(fixed, error)
    t |> success(j != null)
    unsafe {
        delete j
    }
}

[test(description = "try_fixing_broken_json repairs concatenated strings, stray quotes, extra and missing commas and bad escapes")]
def broken_json(t : T?) {
    t |> run("string concatination") @(t : T?) {
        test_broken(t, broken1)
    }
    t |> run("quotes") @(t : T?) {
        test_broken(t, broken2)
    }
    t |> run("extra ,") @(t : T?) {
        test_broken(t, broken3)
    }
    t |> run("missing ,") @(t : T?) {
        test_broken(t, broken4)
    }
    t |> run("extra \u004857") @(t : T?) {
        test_broken(t, broken5)
    }
}

[test(description = "the safe operators on objects, arrays and null values give values, null for missing parts and defaults")]
def safe_operators(t : T?) {
    t |> run("safe object") @(t) {
        let data = "\{ \"a\": 1, \"b\": true, \"c\": null, \"d\": \"str\" \}"
        var err = ""
        var js = read_json(data, err)
        t |> equal(err, "")

        t |> equal((js?["a"] ?as _longint) ?? -1l, 1l)
        t |> equal(js?.a ?? -1, 1)
        t |> equal(js?.a ?? -1l, 1l) // JSON int (_longint) → int64 via ??
        t |> equal(js?.b ?? false, true)
        t |> equal(js?.c is _null, true)
        t |> equal(js?.d ?? "", "str")
        t |> equal(js?.xx is _null, true) // invalid value
        t |> equal(js?.xx?.xy is _null, true) // invalid value
    }

    t |> run("safe array") @(t) {
        let data = "[ 1, true, null, \"str\" ]"
        var err = ""
        var js = read_json(data, err)
        t |> equal(err, "")

        t |> equal(js?[0] ?? -1, 1)
        t |> equal(js?[1] ?? false, true)
        t |> equal(js?[2] is _null, true)
        t |> equal(js?[3] ?? "", "str")
        t |> equal(js?[42] is _null, true) // invalid value
        t |> equal(js?[42]?[0] is _null, true) // invalid value
    }

    t |> run("null object") @(t) {
        var js : JsonValue?
        t |> equal(js?.a ?? -1, -1)
        t |> equal(js?[0] ?? -1, -1)
        js = JVNull()
        t |> equal(js?.a ?? -1, -1)
        t |> equal(js?[0] ?? -1, -1)
    }
}

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

[test(description = "JV and from_JV round-trip a struct holding arrays of structs holding arrays")]
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
