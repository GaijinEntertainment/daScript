options debugger = true

require rtti
require debugapi

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

[export]
def test
    fork_debug_agent_context(@@debug_agent)
	var x = 5
	breakpoint()
	x = 6
	for t in range(0,10)
		print("x={x} t={t}\n")
		x ++
		x += 3
	return true
