require debugapi

require rtti public

class DapiDebugAgent
    def abstract onInstall ( agent:DebugAgent? ) : void
    def abstract onUninstall ( agent:DebugAgent? ) : void
    def abstract onCreateContext ( var ctx:Context ) : void
    def abstract onDestroyContext ( var ctx:Context ) : void
    def abstract onSingleStep ( var ctx:Context; at:LineInfo ) : void
    def abstract onBreakpoint ( var ctx:Context; at:LineInfo ) : void
    def abstract onTick : void

def install_new_debug_agent ( agentPtr )
    static_if typeinfo(is_class *agentPtr)
        let agentInfo = class_info(*agentPtr)
        var agentAdapter <- make_debug_agent(agentPtr,agentInfo)
        install_debug_agent(agentAdapter)
        unsafe
            delete agentAdapter
    else
        concept_assert(false,"can't make debug agent of non-class")
