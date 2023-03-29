require dastest/testing_boost public
require daslib/json_boost
require daslib/stringify

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

def test_broken ( t: T?; text:string )
    var error : string
    var j = read_json(text,error)
    t |> equal(null, j)
    let fixed = try_fixing_broken_json(text)
    j = read_json(fixed,error)
    t |> success(j!=null)
    unsafe
        delete j

[test]
def broken_json ( t: T? )
    t |> run("string concatination") <| @ ( t : T? )
        test_broken(t,broken1)
    t |> run("quotes") <| @ ( t : T? )
        test_broken(t,broken2)
