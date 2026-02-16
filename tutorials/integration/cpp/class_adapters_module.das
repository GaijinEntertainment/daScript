options gen2
options remove_unused_symbols = false

// Abstract base class — daScript side.
// C++ will include this via compileBuiltinModule and the XDD .das.inc file.
// daScript classes can derive from this and override the virtual methods.

class TutorialBaseClass {
    def abstract update(dt : float) : void
    def abstract get_position : float3
}
