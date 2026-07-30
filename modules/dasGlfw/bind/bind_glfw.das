options gen2
require cbind/cbind_boost
require daslib/fio
require daslib/defer
require daslib/strings_boost

class GlfwGen : CppGenBind {
    override func_to_stdout = false
    override generate_constant_decl = true
    glfw_skip_func : table<string; bool>
    def GlfwGen {
        bind_root = "{get_das_root()}/modules/dasGlfw/src"
        bind_module = "dasGLFW"
        bind_das_module = "glfw"
        let pfn = "GLFW/glfw3.h"
        let pfp = "{get_das_root()}/modules/dasGlfw/glfw/include/"
        let args <- array<string>(
            "-xc++-header",
            "-std=c++1z",
            "-I{get_full_file_name(pfp)}",
            "-D_ALLOW_COMPILER_AND_STL_VERSION_MISMATCH=1"
        )
        func_per_chunk = 20
        super(pfn, pfp, args)
        init_skip_func()
        openAllFiles()
    }
    def init_skip_func {
        glfw_skip_func <- {
            "glfwDestroyWindow" => true
        }
        // No emscripten_skip_function override: the 11 GLFW functions emcc's JS GLFW
        // (library_glfw.js) does not provide are implemented as honest C stubs in
        // src/glfw_emscripten_stubs.c, so the full 3.4 surface registers on web too.
        // Skipping registration is NOT fail-closed there — a host-cross-compiled card
        // resolves referenced externs by mangled name at startup, and a das-visible
        // function missing from the wasm-side module is a startup fatal.
    }
    def override skip_const(name : string) : bool {
        return name |> starts_with("GL_")
    }
    def override isArgByValue(name : string) {
        return true
    }
    def override skip_function(var c : CXCursor) {
        if (AnyGenBind`skip_function(self, c)) {
            return true
        }
        let function_name = string(clang_getCursorSpelling(c))
        if (glfw_skip_func |> key_exists(function_name)) {
            to_log(LOG_INFO, "skipping GLFW blocked function {function_name} at {clang_getCursorLocationDescription(c)}")
            return true
        }
        if (function_name |> starts_with("glfw")) {
            return false
        } else {
            to_log(LOG_INFO, "skipping non GLFW function {function_name} at {clang_getCursorLocationDescription(c)}")
            return true
        }
    }
    def override skip_file(fname : string) : bool {
        if (fname |> ends_with("glfw3.h")) {
            return false
        }
        return !fname |> ends_with(PARSE_FILE_NAME)
    }
    def override generateModuleHPrefix {
        module_h_file |> fwrite("#include \"cb_dasGLFW.h\"\n")
    }
    def override generateModuleHSuffix {
        module_h_file |> fwrite("\tpublic:\n")
        module_h_file |> fwrite("\t\tvirtual ~Module_{bind_module}();\n")
        module_h_file |> fwrite("\tpublic:\n")
        module_h_file |> fwrite("\t\tstatic thread_local das_map<void *, GlswCallbacks> g_Callbacks;\n")
    }
    def override skip_alias(var c : CXCursor) {
        return false
    }
}

[export]
def main {
    // generate
    var cgb = new GlfwGen()
    defer() {
        unsafe {
            delete cgb
        }
    }
    cgb->generate()
    cgb->genCMakeDecl("DAS_GLFW_BIND")
}
