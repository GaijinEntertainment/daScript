options persistent_heap
options gc

require daslib/openai
require daslib/stringify
require daslib/strings_boost

// note:
//  code-davinci-002 is always overloaded. it does better job though
//  i wonder if trying one and then the other is the right thing to do

// let unit_test_model = "code-davinci-002"
let unit_test_model = "code-cushman-001"

let unit_test_prompt = %stringify~
/*
daScript is a strongly typed programming language, similar to Python, Ruby and Cotlin.
daScript has built in test framework, which includes faker.

faker has the following functions:

def public long_string ( var faker:Faker ) : string         //! Generates a long string of random characters. The string is anywhere between 0 and faker.max_long_string characters long.
def public any_string ( var faker:Faker ) : string          //! Generates a string of random characters. The string is anywhere between 0 and regex::re_gen_get_rep_limit() characters long.
def public any_file_name ( var faker:Faker ) : string       //! Generates random file name.
def public random_int ( var faker:Faker ) : int             //! Generates random integer.
def public random_uint ( var faker:Faker ) : uint           //! Generates random unsigned integer.
def public random_int8 ( var faker:Faker ) : int8           //! Generates random int8.
def public random_uint8 ( var faker:Faker ) : uint8         //! Generates random uint8.
def public random_int16 ( var faker:Faker ) : int16         //! Generates random int16.
def public random_uint16 ( var faker:Faker ) : uint16       //! Generates random uint16.
def public random_float ( var faker:Faker ) : float         //! Generates random float.
def public random_int2 ( var faker:Faker ) : int2           //! Generates random int2.
def public random_range ( var faker:Faker ) : range         //! Generates random range.
def public random_int3 ( var faker:Faker ) : int3           //! Generates random int3.
def public random_int4 ( var faker:Faker ) : int4           //! Generates random int4.
def public random_uint2 ( var faker:Faker ) : uint2         //! Generates random uint2.
def public random_urange ( var faker:Faker ) : urange       //! Generates random urange.
def public random_uint3 ( var faker:Faker ) : uint3         //! Generates random uint3.
def public random_uint4 ( var faker:Faker ) : uint4         //! Generates random uint4.
def public random_float2 ( var faker:Faker ) : float2       //! Generates random float2.
def public random_float3 ( var faker:Faker ) : float3       //! Generates random float3.
def public random_float4 ( var faker:Faker ) : float4       //! Generates random float4.
def public random_float3x3 ( var faker:Faker ) : float3x3   //! Generates random float3x3.
def public random_float3x4 ( var faker:Faker ) : float3x4   //! Generates random float3x4.
def public random_float4x4 ( var faker:Faker ) : float4x4   //! Generates random float4x4.
def public random_int64 ( var faker:Faker ) : int64         //! Generates random int64
def public random_uint64 ( var faker:Faker ) : uint64       //! Generates random uint64
def public random_double ( var faker:Faker ) : double       //! Generates random double.
def public any_set ( var faker:Faker ) : uint[8]            //! Generates random set (uint[8])
def public any_char ( var faker:Faker ) : int               //! Generates random char. (1 to 255 range)
def public number ( var faker:Faker ) : string              //! Generates random number string.
def public positive_int ( var faker:Faker ) : string        //! Generates random positive integer string.
def public any_int ( var faker:Faker ) : string             //! Generates random integer string.
def public any_hex ( var faker:Faker ) : string             //! Generates random integer hex string.
def public any_float ( var faker:Faker ) : string           //! Generates random float string.
def public any_uint ( var faker:Faker ) : string            //! Generates random unsigned integer string.

here is example of the unit test for the functions add and sub:
*/

def add(a,b)
    return a + b

def sub(a,b)
    return a - b

[test] // use random_int, random_float in add. use random_int in sub
def test_add_and_sub ( t:T? )
    t |> run("testing add") <| @@ ( t : T? )
        t |> equal(3, add(1,2))
        t |> equal(3., add(2.,1.))
        var fake <- Faker()
        fuzz <|
            let a = fake |> random_int
            let b = fake |> random_int
            t |> equal(a+b, add(a,b))
            let c = fake |> random_float
            let c = fake |> random_float
            t |> equal(c+d, add(c,d))
    t |> run("testing sub") <| @@ ( t : T? )
        t |> equal(1, sub(3,2))
        var fake <- Faker()
        fuzz <|
            let a = fake |> random_int
            let b = fake |> random_int
            t |> equal(a-b, sub(a,b))

// -----------------------------------

%%

def generate_test ( func, description : string ) : string
    var prompt = build_string <| $ ( writer )
        writer |> write(unit_test_prompt)
        writer |> write(func)
        writer |> write("\n[test] // ")
        writer |> write(description)
        writer |> write("\ndef test_")
    var ccr <- openai_create_completion([[Completion()
        model = unit_test_model,
        prompt = prompt,
        max_tokens = 200,
        top_p = 1.0,
        temperature = 0.,
        frequency_penalty = 0.52,
        presence_penalty = 0.5,
        best_of = 1,
        stop <- [{string "// --"; "\}"}]
    ]])
    if ccr |> is_valid
        var choice = ccr.choices[0].text |> replace("\\r","")
        choice = safe_unescape(choice)
        return build_string <| $(writer)
            writer |> write("[test] // ")
            writer |> write(description)
            writer |> write("\ndef test_")
            writer |> write(choice)
    else
        to_log(LOG_ERROR, "error: {openai_get_last_error()}\n")
    return ""

// -----------------------------------

let example_function = %stringify~
def strcat(a,b)
    return a + b
%%

let example_description = "use any_string and any_file_name for strcat"

[export]
def main
    if !openai_key_is_set()
        print("OpenAI key is not set. Please set OPENAI_API_KEY environment variable.\n")
        return
    // test retrive
    var model <- openai_retrive_model(unit_test_model)
    if !model |> is_valid
        print("{unit_test_model} model not found\n")
        print("error: {openai_get_last_error()}\n")
        return
    let test = generate_test(example_function, example_description)
    print("{test}\n")

