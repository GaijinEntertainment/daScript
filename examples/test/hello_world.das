options debugger = true

require rtti
require ast

class SampleDebugAgent : DapiDebugAgent
    def override onBreakpoint ( ctx:__context; at:LineInfo ) : void
        print("{ctx} breakpoint at line {int(at.line)}\n")
        set_single_step(true,ctx)
    def override onSingleStep ( ctx:__context; at:LineInfo ) : void
        print("{ctx} step at {int(at.line)}\n")
        stackwalk(ctx,at)

[export]
def debug_agent ( ctx : __context )
    print("debug agent {ctx}\n")
    var agentPtr = new SampleDebugAgent()
    let agentInfo = class_info(*agentPtr)
    var agentAdapter <- make_debug_agent(agentPtr,agentInfo)
    install_debug_agent(agentAdapter)

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
