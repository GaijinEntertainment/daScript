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

