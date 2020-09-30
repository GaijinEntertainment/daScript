options debugger = true

require rtti
require debugapi

// sample of debug agent

class SampleDataWalker : DapiDataWalker
    def override Float ( var value:float & ) : void
        print("[float] {value}\n")
    def override Int ( var value:int & ) : void
        print("[int] {value}\n")

class SampleDebugAgent : DapiDebugAgent
    def override onBreakpoint ( var ctx:Context; at:LineInfo ) : void
        print("{ctx} breakpoint at line {int(at.line)}\n")
        set_single_step(ctx,true)
    def override onSingleStep ( var ctx:Context; at:LineInfo ) : void
        print("{ctx} step at {int(at.line)}\n")
        stackwalk(ctx,at)

[export]
def debug_agent ( ctx : Context )
    install_new_debug_agent(new SampleDebugAgent())

// sample of data walker

struct Foo
    a : int
    b : float

def example_walk
    var pf = new [[Foo a=1,b=3.14f]]
    var walker <- make_data_walker(new SampleDataWalker())
    var pfoo : void?
    unsafe
        pfoo = reinterpret<void?>(pf)
    walk_data(walker,pfoo,typeinfo(rtti_typeinfo type<Foo>))
    unsafe
        delete walker


[export]
def test
    example_walk()
    fork_debug_agent_context(@@debug_agent)
	var x = 5
	breakpoint()
	x = 6
	for t in range(0,10)
		print("x={x} t={t}\n")
		x ++
		x += 3

	return true
