require debugapi

require rtti public

class DapiDebugAgent
    def abstract onInstall ( agent:DebugAgent? ) : void
    def abstract onUninstall ( agent:DebugAgent? ) : void
    def abstract onCreateContext ( ctx:__context ) : void
    def abstract onDestroyContext ( ctx:__context ) : void
    def abstract onSingleStep ( ctx:__context; at:LineInfo ) : void
    def abstract onBreakpoint ( ctx:__context; at:LineInfo ) : void
    def abstract onTick : void

