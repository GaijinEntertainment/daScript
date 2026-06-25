options gen2
require cbind/cbind_boost
require daslib/fio
require daslib/defer
require daslib/strings_boost

class GlfwGen : CppGenBind {
    override func_to_stdout = false
    override generate_constant_decl = true
    glfw_skip_func : table<string; bool>
    glfw_emscripten_skip : table<string>
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
        // GLFW functions emscripten's library_glfw.js (a GLFW 3.3.8 subset) does not
        // provide: the 3.4 additions plus a few 3.3 funcs left unimplemented. All are
        // unused across the codebase + consumers (audited), so skip-on-web is fail-closed.
        glfw_emscripten_skip <- {
            "glfwGetError", "glfwGetGamepadName", "glfwGetGamepadState",
            "glfwGetMonitorUserPointer", "glfwGetPlatform", "glfwGetWindowTitle",
            "glfwInitAllocator", "glfwInitHint", "glfwPlatformSupported",
            "glfwSetMonitorUserPointer", "glfwUpdateGamepadMappings"
        }
    }
    def override emscripten_skip_function(function_name : string) : bool {
        return key_exists(glfw_emscripten_skip, function_name)
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
