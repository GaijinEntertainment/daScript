options persistent_heap
options gc

require cbind/cbind_boost
require fio

class OpenGLBind : DasGenBind
    def OpenGLBind ( pfn,pfp:string )
        let args <- [{string "-DGL_GLEXT_PROTOTYPES"}]
        init_args(pfn,pfp,args)
        init_defaults()
    def init_defaults
        known_type <- {{
            "GLboolean"         => "bool";
            "GLboolean *"       => "bool? implicit";
            "const GLbyte *"    => "int8 const? implicit"
        }}
        var blocked_defines <- [{string []
            "true";
            "false"
        }]
        for bd in blocked_defines
            blocked_defines_table[bd] = true
        delete blocked_defines
    def override functionArgumentRules : TypeRules
        return (
            TypeRules char_ptr_is_string |
            TypeRules function_proto_is_void_ptr |
            TypeRules structure_ptr_is_void_ptr |
            TypeRules top_level_pointers_are_implicit
        )
    def override functionResultRules : TypeRules
        return (
            TypeRules char_ptr_is_string |
            TypeRules function_proto_is_void_ptr |
            TypeRules structure_ptr_is_void_ptr
        )
    def override output_dasbind ( function_name:string )
        fprint(func_decl_file, "[extern(opengl,late,name=\"{function_name}\",library=\"Opengl32\")]\n")

[export]
def main
    let output_name = "{get_das_root()}/modules/dasOpenGL/opengl/opengl_func.das"
    fopen(output_name,"wb") <| $( fdf )
        fprint(fdf,"require dasbind public\n\n")
        let output_const_name = "{get_das_root()}/modules/dasOpenGL/opengl/opengl_const.das"
        fopen(output_const_name,"wb") <| $( dcf )
            let pfn = "glcorearb.h"
            let pfp = "{get_das_root()}/modules/dasOpenGL/opengl/OpenGL/"
            var glbind = new OpenGLBind(pfn,pfp)
            glbind->setFiles (dcf,fdf)
            glbind->generate()
            unsafe
                delete glbind
