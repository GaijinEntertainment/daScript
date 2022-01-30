options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module glsl_internal shared public

require ast
require rtti
require strings

require daslib/ast_boost
require daslib/templates_boost
require daslib/strings_boost
require daslib/ast_used

let DEBUG_SHADER_TEXT = false
let DEBUG_SHADER_BINDINGS = false

enum ShaderType
    vertex
    fragment
    compute

bitfield ShaderExportCaps
    output_shader_lines                         // output #line declarations
    write_version                               // output shader version on top
    output_inout                                // output regular variable declarations for inout variables (disable allows BGFX replcament declarations)
    output_inout_stub                           // output $input and $output declarations for the inout varaibles for BGFX
    output_inout_decl                           // output inout variable declarations to the separate DECL variable for BGFX
    replace_hlsl_incompatible_operations        // replace MUL and vector comparisons
    restrict_uniform_types                      // restrict uniform types to BGFX-compatible
    restrict_vertex_attribute_names             // restrict vertex attribute names to BGFX-compatible
    restrict_array_as_result                    // restrict arrays as function results or global variables (due to initializer)
    output_unfirom_macro_declarations           // output BGFX-style image and sampler declarations
    output_compatibility_constructors           // output VEC2, VEC3, etc constructors. also MUL for matrix and vector multiplications
    output_hlsl_style_compute_layout            // output NUM_THREADS(x,y,z) macro in front of main of the compute shader
    output_structure_initializers               // generate Foo_STRUCTURE_INITIALIZER instead of Foo() for HLSL compatibility
    allow_unitialized_local_arrays              // var a: float[9] can be local, and will not be initialized for HLSL (only if restrict_array_as_result)
    resolve_supported_semantics                 // resolve semantics from variable name (a_color0, v_position etc)

class GlslShader : AstFunctionAnnotation
    shaderType = ShaderType vertex
    write_version = true
    output_inout = true
    output_inout_stub = false
    output_inout_decl = false
    output_shader_lines = true
    replace_hlsl_incompatible_operations = false
    restrict_uniform_types = false
    restrict_vertex_attribute_names = false
    restrict_array_as_result = false
    output_unfirom_macro_declarations = false
    output_compatibility_constructors = false
    output_hlsl_style_compute_layout = false
    output_structure_initializers = false
    allow_unitialized_local_arrays = false
    resolve_supported_semantics = false
    def get_glsl_prefix
        return ""
    def generate_bind_uniform_dummy ( var func:FunctionPtr )
        pass
    def generate_bind_uniform(fnMain:FunctionPtr; var fn:FunctionPtr)
        pass
    def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        func.flags |= FunctionFlags exports // note: this is temporary, until we are done with dependency collecting etc
        let argName = find_arg("name", args) ?as tString ?? "{func.name}`shader_text"
        if !add_global_var(compiling_module(),argName, new [[TypeDecl() at=func.at, baseType=Type tString]], func.at, false)
            errors := "can't add global variable {argName}"
            return false
        if output_inout_decl
            let decl_name = "{argName}_DECL"
            if !add_global_var(compiling_module(), decl_name, new [[TypeDecl() at=func.at, baseType=Type tString]], func.at, false)
                errors := "can't add global variable {decl_name}"
                return false
        self->generate_bind_uniform_dummy(func)
        return true
    def override patch ( var func:FunctionPtr; var group:ModuleGroup; args,progArgs:AnnotationArgumentList; var errors : das_string; var astChanged:bool& ) : bool
        for_each_function(compiling_module(), bind_uniform_function_name(func)) <| $ ( bfun )
            if bfun.flags.exports
                bfun.flags &= ~FunctionFlags exports
                self->generate_bind_uniform(func, bfun)
                astChanged = true
        return true
    def buildCaps
        var caps : ShaderExportCaps
        if write_version
            caps |= ShaderExportCaps write_version
        if output_inout
            caps |= ShaderExportCaps output_inout
        if output_inout_stub
            caps |= ShaderExportCaps output_inout_stub
        if output_inout_decl
            caps |= ShaderExportCaps output_inout_decl
        if output_shader_lines
            caps |= ShaderExportCaps output_shader_lines
        if replace_hlsl_incompatible_operations
            caps |= ShaderExportCaps replace_hlsl_incompatible_operations
        if restrict_uniform_types
            caps |= ShaderExportCaps restrict_uniform_types
        if restrict_vertex_attribute_names
            caps |= ShaderExportCaps restrict_vertex_attribute_names
        if restrict_array_as_result
            caps |= ShaderExportCaps restrict_array_as_result
        if output_unfirom_macro_declarations
            caps |= ShaderExportCaps output_unfirom_macro_declarations
        if output_compatibility_constructors
            caps |= ShaderExportCaps output_compatibility_constructors
        if output_hlsl_style_compute_layout
            caps |= ShaderExportCaps output_hlsl_style_compute_layout
        if output_structure_initializers
            caps |= ShaderExportCaps output_structure_initializers
        if allow_unitialized_local_arrays
            caps |= ShaderExportCaps allow_unitialized_local_arrays
        if resolve_supported_semantics
            caps |= ShaderExportCaps resolve_supported_semantics
        return caps
    def override fixup ( var func:FunctionPtr; var group:ModuleGroup; args,progArgs:AnnotationArgumentList; var errors : das_string ) : bool
        let argName = find_arg("name", args) ?as tString ?? "{func.name}`shader_text"
        var version = find_arg("version", args) ?as tInt ?? (shaderType==ShaderType compute ? 430 : 330)
        var compute_local_size : int3
        if shaderType==ShaderType compute
            let arg_local_size_x = find_arg("local_size_x", args)
            if arg_local_size_x is tInt
                compute_local_size.x = arg_local_size_x as tInt
            else
                errors := "missing local_size_x"
                return false
            compute_local_size.y = find_arg("local_size_y", args) ?as tInt ?? 0
            compute_local_size.z = find_arg("local_size_z", args) ?as tInt ?? 0
        // TODO: 'find_global(name)'
        var err : array<string>
        var glob <- find_variable(compiling_module(), argName)
        if glob!=null && glob.init == null
            if glob.name == argName
                var caps = self->buildCaps()
                let gres = generate_glsl(func, err, shaderType, version, compute_local_size, caps)
                let shader_text = "{gres.inout_stub}{self->get_glsl_prefix()}{gres.body}"
                glob.init <- new [[ExprConstString() at=func.at,
                    value := shader_text,
                    _type <- new [[TypeDecl() at=func.at, baseType=Type tString]]
                ]]
                if ! err |> empty()
                    glob.init := null
                else
                    glob._type.flags |= TypeDeclFlags constant
                    if output_inout_decl
                        let decl_name = "{argName}_DECL"
                        var glob_def <- find_variable(compiling_module(), decl_name)
                        if glob_def!=null
                            glob_def.init <- new [[ExprConstString() at=func.at,
                                value := gres.inout_decl,
                                _type <- new [[TypeDecl() at=func.at, baseType=Type tString]]
                            ]]
                            glob_def := null
        glob := null
        if !err |> empty()
            errors := join(err, "\n")
            return false
        return true

let private glsl_restricted_vertex_attribute_names <- {{
    "a_position"    => true;
    "a_normal"      => true;
    "a_tangent"     => true;
    "a_bitangent"   => true;
    "a_color0"      => true;
    "a_color1"      => true;
    "a_color2"      => true;
    "a_color3"      => true;
    "a_indices"     => true;
    "a_weight"      => true;
    "a_texcoord0"   => true;
    "a_texcoord1"   => true;
    "a_texcoord2"   => true;
    "a_texcoord3"   => true;
    "a_texcoord4"   => true;
    "a_texcoord5"   => true;
    "a_texcoord6"   => true;
    "a_texcoord7"   => true
}}

let private glsl_types <- {{
    Type tFloat         => "float";
    Type tFloat2        => "vec2";
    Type tFloat3        => "vec3";
    Type tFloat4        => "vec4";
    Type tInt           => "int";
    Type tInt2          => "ivec2";
    Type tInt3          => "ivec3";
    Type tInt4          => "ivec4";
    Type tBool          => "bool";
    Type tRange         => "ivec2";
    Type tURange        => "uvec2";
    Type tVoid          => "void";
    Type tEnumeration   => "int"
}}

let private glsl_handle_types <- {{
    "float4x4"      => "mat4";
    "float3x3"      => "mat3"
}}

let private glsl_functions <- {{
    "float3x3"  =>  "mat3";
    "float2"    =>  "vec2";
    "float3"    =>  "vec3";
    "float4"    =>  "vec4";
    "int2"    =>  "ivec2";
    "int3"    =>  "ivec3";
    "int4"    =>  "ivec4";
    "uint2"    =>  "uvec2";
    "uint3"    =>  "uvec3";
    "uint4"    =>  "uvec4";
    "floori"    =>  "floor";
    "lerp"      =>  "mix";
    "saturate"  =>  "SATURATE";
    "int_bits_to_float" => "intBitsToFloat";
    "uint_bits_to_float" => "uintBitsToFloat";
    "float_bits_to_int" => "floatBitsToInt";
    "float_bits_to_uint" => "floatBitsToUint";
    "discard"   =>  "_discard";
    "_"         =>  "_"
}}

let private glsl_compatibility_constructors <- {{
    "float4x4"  =>  "MAT4";
    "float3x3"  =>  "MAT3";
    "float4"  =>  "VEC4";
    "float3"  =>  "VEC3";
    "float2"  =>  "VEC2";
    "int4"  =>  "IVEC4";
    "int3"  =>  "IVEC3";
    "int2"  =>  "IVEC2";
    "uint4"  =>  "UVEC4";
    "uint3"  =>  "UVEC3";
    "uint2"  =>  "UVEC2"
}}

class GlslExport : AstVisitor
    errors : array<string>
    writer : StringBuilderWriter?
    tab : int = 0
    stnames   : table<string;bool>
    depFun    : table<uint64;bool>
    depVar    : table<uint64;bool>
    depStruct : table<uint64;bool>
    depEnum   : table<uint64;bool>
    trackLine : int = 0
    trackFile : string
    shaderType : ShaderType
    version : int = 0
    renames : array<tuple<name:string;repl:string>>
    astVisitorAdapter : smart_ptr<ast::VisitorAdapter>
    compute_local_size : int3
    caps : ShaderExportCaps
    inout_stub : string
    inout_decl : string
    def GlslExport ( var w : StringBuilderWriter; sht:ShaderType; ver:int; cls:int3; cp:ShaderExportCaps )
        unsafe
            writer = addr(w)
        shaderType = sht
        version = ver
        compute_local_size = cls
        caps = cp
    def operator delete
        delete stnames
        delete depFun
        delete depVar
        delete depStruct
        delete depEnum
        delete renames
// TODO: fixme (egyptian braces)
    def newLine(count:int)
        for t in range(count)
            *writer |> write("\n")
        trackLine += count
    def error ( txt:string; at:LineInfo )
        let fname = at.fileInfo!=null ? "{at.fileInfo.name}" : ""
        errors |> push("\tGLSL: {txt} at {fname}:{int(at.line)}:{int(at.column)}")
    def line(at:LineInfo)
        if !caps.output_shader_lines
            return
        if at.fileInfo!=null
            peek(at.fileInfo.name) <| $ ( atname )
                if trackLine!=int(at.line) || trackFile!=atname
                    trackLine = int(at.line)
                    trackFile := atname
                    let trackFileText = trackFile |> replace("\\","/")
                    *writer |> write("#line {trackLine} \"{trackFileText}\"\n")
// GLSL TYPE
    def describe_glsl_type_ex ( t : TypeDeclPtr; write_dim:bool )
        let st = build_string() <| $ ( var tw )
            // if t.flags.ref
            //    tw |> write("& ") // inout?
            if t.baseType==Type tStructure
                tw |> write("{t.structType.name}")
            elif t.baseType==Type tHandle
                peek(t.annotation.name) <| $ ( aname )
                    tw |> write(glsl_handle_types?[aname] ?? "error")
            elif glsl_types |> key_exists(t.baseType)
                tw |> write(glsl_types?[t.baseType] ?? "error")
            else
                self->error ("unsupported type {t.baseType}", t.at)
                tw |> write("error")
            if write_dim
                for d in t.dim
                    tw |> write("[{d}]")
        return st
    def describe_glsl_type ( t : TypeDeclPtr )
        return self->describe_glsl_type_ex(t,true)
    def glsl_function_name ( fun:FunctionPtr )
        var funname = "{fun.name}"
        if fun.flags.builtIn
            pass
        elif fun._module.name=="$"
            pass
        elif fun._module.name=="glsl_common"
            pass
        elif fun._module.name=="math" || fun._module.name=="math_bits"
            pass
        elif fun._module.name!=""
            funname = "{funname}_in_{fun._module.name}"
        while stnames |> key_exists(funname)
            funname = "_{funname}"
        return funname
    def describe_glsl_function ( fun:FunctionPtr )
        let st = build_string() <| $ ( var tw )
            tw |> write <| self->describe_glsl_type(fun.result)
            var funname = self->glsl_function_name(fun)
            tw |> write (" {funname} ( ")
            for arg,i in fun.arguments,range(100500)
                if i!=0
                    tw |> write(", ")
                if arg._type.isRef && !arg._type.flags.constant
                    tw |> write("inout ")
                tw |> write <| self->describe_glsl_type(arg._type)
                tw |> write ( " {arg.name}")
            tw |> write(" )")
        return st
    def produce_zero ( t : TypeDeclPtr )
        if length(t.dim) > 0
            *writer |> write("{self->describe_glsl_type(t)}(")
            var ct <- clone_type(t)
            ct.dim |> pop()
            for d in range(t.dim[length(t.dim)-1])
                if d!=0
                    *writer |> write(",")
                self->produce_zero(ct)
            ct := null
            *writer |> write(")")
        elif t.baseType==Type tStructure
            if caps.output_structure_initializers
                *writer |> write("{t.structType.name}_STRUCTURE_INITIALIZER(")
            else
                *writer |> write("{t.structType.name}(")
            for fld,i in t.structType.fields,range(100500)
                if i!=0
                    *writer |> write(",")
                self->produce_zero(fld._type)
            *writer |> write(")")
        elif t.baseType==Type tHandle
            if t.annotation.name=="float3x3"
                if caps.output_compatibility_constructors
                    *writer |> write("MAT3(VEC3(0),VEC3(0),VEC3(0))")
                else
                    *writer |> write("mat3(vec3(0),vec3(0),vec3(0))")
            elif t.annotation.name=="float4x4"
                if caps.output_compatibility_constructors
                    *writer |> write("MAT4(VEC4(0),VEC4(0),VEC4(0),VEC4(0))")
                else
                    *writer |> write("mat4(vec4(0),vec4(0),vec4(0),vec4(0))")
            else
                self->error("unsupported zero for handled type {describe(t)}", t.at)
        elif t.baseType==Type tInt
            *writer |> write("0")
        elif t.baseType==Type tInt2 || t.baseType==Type tRange
            *writer |> write("ivec2(0,0)")
        elif t.baseType==Type tInt3
            *writer |> write("ivec3(0,0,0)")
        elif t.baseType==Type tInt4
            *writer |> write("ivec4(0,0,0,0)")
        elif t.baseType==Type tUInt || t.baseType==Type tURange
            *writer |> write("0u")
        elif t.baseType==Type tUInt2
            *writer |> write("uvec2(0u,0u)")
        elif t.baseType==Type tUInt3
            *writer |> write("uvec3(0u,0u,0u)")
        elif t.baseType==Type tUInt4
            *writer |> write("uvec4(0u,0u,0u,0u)")
        elif t.baseType==Type tFloat
            *writer |> write("0.0")
        elif t.baseType==Type tFloat2
            *writer |> write("vec2(0.0f,0.0f)")
        elif t.baseType==Type tFloat3
            *writer |> write("vec3(0.0f,0.0f,0.0f)")
        elif t.baseType==Type tFloat4
            *writer |> write("vec4(0.0f,0.0f,0.0f,0.0f)")
        elif t.baseType==Type tBool
            *writer |> write("false")
        elif t.baseType==Type tEnumeration
            *writer |> write("0")
        else
            self->error("unsupported zero for type {describe(t)}", t.at)
// any subexpression to string in current context
    def describe_subexpression(expr:ExpressionPtr)
        let oldWriter = writer
        var st = build_string() <| $ (var newWriter)
            unsafe
                writer = addr(newWriter)
            visit(expr, astVisitorAdapter)
        writer = oldWriter
        return st
// check if main
    def isMainFunction(fun:FunctionPtr)
        var isMain = false
        for ann in fun.annotations
            peek(ann.annotation.name) <| $ ( aname )
                if aname|>find("vertex_program")!=-1 || aname|>find("fragment_program")!=-1 || aname|>find("compute_program")!=-1
                    isMain = true
        return isMain
// program
    def override preVisitProgramBody(prog:ProgramPtr;mod:Module?) : void
        var any = false
        for_each_function(mod, "") <| $ ( fun )
            if depFun |> key_exists(intptr(get_ptr(fun)))
                if !self->isMainFunction(fun)
                    if fun._module.name!="glsl_common"
                        let ss = self->describe_glsl_function(fun)
                        *writer |> write("{ss};")
                        self->newLine(1)
                        any = true
        if any
            self->newLine(1)
    def override preVisitProgram(prog:ProgramPtr) : void
        if caps.write_version
            *writer |> write("#version {version} core\n\n")
        // TODO: only output builtin functions which are necessary
        if version>=400
            *writer |> write("void _discard ( ) \{ discard; \}\n")
        *writer |> write("ivec2 range ( int r ) \{ return ivec2(0,r); \}\n")
        *writer |> write("ivec2 range ( int f, int t ) \{ return ivec2(f,t); \}\n")
        *writer |> write("float SATURATE ( float a ) \{ return clamp(a,0.,1.); \}\n")
        *writer |> write("vec2 SATURATE ( vec2 a ) \{ return clamp(a,vec2(0.,0.),vec2(1.,1.)); \}\n")
        *writer |> write("vec3 SATURATE ( vec3 a ) \{ return clamp(a,vec3(0.,0.,0.),vec3(1.,1.,1.)); \}\n")
        *writer |> write("vec4 SATURATE ( vec4 a ) \{ return clamp(a,vec4(0.,0.,0.,0.),vec4(1.,1.,1.,1.)); \}\n")
        *writer |> write("void sincos ( float a, out float sv, out float cv ) \{ sv=sin(a); cv=cos(a); \}\n")
        *writer |> write("float length_sq ( vec2 t ) \{ return dot(t,t); \}\n")
        *writer |> write("float length_sq ( vec3 t ) \{ return dot(t,t); \}\n")
        *writer |> write("float length_sq ( vec4 t ) \{ return dot(t,t); \}\n")
        *writer |> write("float mad ( float a, float b, float c ) \{ return a*b+c; \}\n")
        *writer |> write("vec2 mad ( vec2 a, float b, vec2 c ) \{ return a*vec2(b,b)+c; \}\n")
        *writer |> write("vec3 mad ( vec3 a, float b, vec3 c ) \{ return a*vec3(b,b,b)+c; \}\n")
        *writer |> write("vec4 mad ( vec4 a, float b, vec4 c ) \{ return a*vec4(b,b,b,b)+c; \}\n")
        *writer |> write("vec2 mad ( vec2 a, vec2 b, vec2 c ) \{ return a*b+c; \}\n")
        *writer |> write("vec3 mad ( vec3 a, vec3 b, vec3 c ) \{ return a*b+c; \}\n")
        *writer |> write("vec4 mad ( vec4 a, vec4 b, vec4 c ) \{ return a*b+c; \}\n")
        *writer |> write("float distance_sq ( vec3 a, vec3 b ) \{ return length_sq(a-b); \}\n")
        *writer |> write("float inv_distance_sq ( vec3 a, vec3 b ) \{ return 1. / distance_sq(a,b); \}\n")
        if caps.output_compatibility_constructors
            /*
            *writer |> write("#define VEC2 vec2\n")
            *writer |> write("#define VEC3 vec3\n")
            *writer |> write("#define VEC4 vec4\n")
            *writer |> write("#define IVEC2 ivec2\n")
            *writer |> write("#define IVEC3 ivec3\n")
            *writer |> write("#define IVEC4 ivec4\n")
            *writer |> write("#define UVEC2 uvec2\n")
            *writer |> write("#define UVEC3 uvec3\n")
            *writer |> write("#define UVEC4 uvec4\n")
            *writer |> write("#define MAT3 mat3\n")
            *writer |> write("#define MAT4 mat4\n")
            */
            // vec2
            *writer |> write("vec2 VEC2(float c) \{ return vec2(c,c); \}\n")
            *writer |> write("vec2 VEC2(float x, float y) \{ return vec2(x,y); \}\n")
            *writer |> write("vec2 VEC2(ivec2 c) \{ return vec2(float(c.x),float(c.y)); \}\n")
            *writer |> write("vec2 VEC2(uvec2 c) \{ return vec2(float(c.x),float(c.y)); \}\n")
            // vec3
            *writer |> write("vec3 VEC3(float c) \{ return vec3(c,c,c); \}\n")
            *writer |> write("vec3 VEC3(float x, float y, float z) \{ return vec3(x,y,z); \}\n")
            *writer |> write("vec3 VEC3(vec2 xy, float z) \{ return vec3(xy.x,xy.y,z); \}\n")
            *writer |> write("vec3 VEC3(float x, vec2 yz) \{ return vec3(x,yz.x,yz.y); \}\n")
            *writer |> write("vec3 VEC3(ivec3 c) \{ return vec3(float(c.x),float(c.y),float(c.z)); \}\n")
            // vec4
            *writer |> write("vec4 VEC4(float c) \{ return vec4(c,c,c,c); \}\n")
            *writer |> write("vec4 VEC4(vec2 xy, float z, float w) \{ return vec4(xy.x,xy.y,z,w); \}\n")
            *writer |> write("vec4 VEC4(vec2 xy, vec2 zw) \{ return vec4(xy.x,xy.y,zw.x,zw.y); \}\n")
            *writer |> write("vec4 VEC4(float x, float y, vec2 zw) \{ return vec4(x,y,zw.x,zw.y); \}\n")
            *writer |> write("vec4 VEC4(float x, vec2 yz, float w) \{ return vec4(x,yz.x,yz.y,w); \}\n")
            *writer |> write("vec4 VEC4(vec3 xyz, float w) \{ return vec4(xyz.x,xyz.y,xyz.z,w); \}\n")
            *writer |> write("vec4 VEC4(float x, vec3 yzw) \{ return vec4(x,yzw.x,yzw.y,yzw.z); \}\n")
            *writer |> write("vec4 VEC4(float x, float y, float z, float w) \{ return vec4(x,y,z,w); \}\n")
            *writer |> write("vec4 VEC4(ivec4 c) \{ return vec4(float(c.x),float(c.y),float(c.z),float(c.w)); \}\n")

            // ivec2
            *writer |> write("ivec2 IVEC2(int c) \{ return ivec2(c,c); \}\n")
            *writer |> write("ivec2 IVEC2(uvec2 c) \{ return ivec2(int(c.x),int(c.y)); \}\n")
            *writer |> write("ivec2 IVEC2(int x, int y) \{ return ivec2(x,y); \}\n")
            // ivec3
            *writer |> write("ivec3 IVEC3(int c) \{ return ivec3(c,c,c); \}\n")
            *writer |> write("ivec3 IVEC3(int x, int y, int z) \{ return ivec3(x,y,z); \}\n")
            *writer |> write("ivec3 IVEC3(ivec2 xy, int z) \{ return ivec3(xy.x,xy.y,z); \}\n")
            *writer |> write("ivec3 IVEC3(int x, ivec2 yz) \{ return ivec3(x,yz.x,yz.y); \}\n")
            // ivec4
            *writer |> write("ivec4 IVEC4(int c) \{ return ivec4(c,c,c,c); \}\n")
            *writer |> write("ivec4 IVEC4(ivec2 xy, int z, int w) \{ return ivec4(xy.x,xy.y,z,w); \}\n")
            *writer |> write("ivec4 IVEC4(ivec2 xy, ivec2 zw) \{ return ivec4(xy.x,xy.y,zw.x,zw.y); \}\n")
            *writer |> write("ivec4 IVEC4(int x, int y, ivec2 zw) \{ return ivec4(x,y,zw.x,zw.y); \}\n")
            *writer |> write("ivec4 IVEC4(int x, ivec2 yz, int w) \{ return ivec4(x,yz.x,yz.y,w); \}\n")
            *writer |> write("ivec4 IVEC4(ivec3 xyz, int w) \{ return ivec4(xyz.x,xyz.y,xyz.z,w); \}\n")
            *writer |> write("ivec4 IVEC4(int x, ivec3 yzw) \{ return ivec4(x,yzw.x,yzw.y,yzw.z); \}\n")
            *writer |> write("ivec4 IVEC4(int x, int y, int z, int w) \{ return ivec4(x,y,z,w); \}\n")
            // uvec2
            *writer |> write("uvec2 UVEC2(uint c) \{ return uvec2(c,c); \}\n")
            *writer |> write("uvec2 UVEC2(ivec2 c) \{ return uvec2(uint(c.x),uint(c.y)); \}\n")
            *writer |> write("uvec2 UVEC2(uint x, uint y) \{ return uvec2(x,y); \}\n")
            // uvec3
            *writer |> write("uvec3 UVEC3(uint c) \{ return uvec3(c,c,c); \}\n")
            *writer |> write("uvec3 UVEC3(uint x, uint y, uint z) \{ return uvec3(x,y,z); \}\n")
            *writer |> write("uvec3 UVEC3(uvec2 xy, uint z) \{ return uvec3(xy.x,xy.y,z); \}\n")
            *writer |> write("uvec3 UVEC3(uint x, uvec2 yz) \{ return uvec3(x,yz.x,yz.y); \}\n")
            // uvec4
            *writer |> write("uvec4 UVEC4(uint c) \{ return uvec4(c,c,c,c); \}\n")
            *writer |> write("uvec4 UVEC4(uvec2 xy, uint z, uint w) \{ return uvec4(xy.x,xy.y,z,w); \}\n")
            *writer |> write("uvec4 UVEC4(uvec2 xy, uvec2 zw) \{ return uvec4(xy.x,xy.y,zw.x,zw.y); \}\n")
            *writer |> write("uvec4 UVEC4(uint x, uint y, uvec2 zw) \{ return uvec4(x,y,zw.x,zw.y); \}\n")
            *writer |> write("uvec4 UVEC4(uint x, uvec2 yz, uint w) \{ return uvec4(x,yz.x,yz.y,w); \}\n")
            *writer |> write("uvec4 UVEC4(uvec3 xyz, uint w) \{ return uvec4(xyz.x,xyz.y,xyz.z,w); \}\n")
            *writer |> write("uvec4 UVEC4(uint x, uvec3 yzw) \{ return uvec4(x,yzw.x,yzw.y,yzw.z); \}\n")
            *writer |> write("uvec4 UVEC4(uint x, uint y, uint z, uint w) \{ return uvec4(x,y,z,w); \}\n")
            // mat3
            *writer |> write("mat3 MAT3(vec3 a, vec3 b, vec3 c) \{ mat3 res; res[0]=a; res[1]=b; res[2]=c; return res; \}\n")
            *writer |> write("mat3 MAT3(mat4 m) \{ mat3 res; res[0]=m[0].xyz; res[1]=m[1].xyz; res[2]=m[2].xyz; return res; \}\n")
            // mat4
            *writer |> write("mat4 MAT4(vec4 a, vec4 b, vec4 c, vec4 d) \{ mat4 res; res[0]=a; res[1]=b; res[2]=c; res[3]=d; return res; \}\n")
            // mul mat vec
            *writer |> write("vec3 MUL(mat3 a, vec3 b) \{ return a[0]*b.x + a[1]*b.y + a[2]*b.z; \}\n")
            *writer |> write("mat3 MUL(mat3 a, mat3 b) \{
                mat3 res;
                res[0] = a[0]*b[0].x + a[1]*b[0].y + a[2]*b[0].z;
                res[1] = a[0]*b[1].x + a[1]*b[1].y + a[2]*b[1].z;
                res[2] = a[0]*b[2].x + a[1]*b[2].y + a[2]*b[2].z;
                return res; \}\n")
            *writer |> write("vec4 MUL(mat4 a, vec4 b) \{ return a[0]*b.x + a[1]*b.y + a[2]*b.z + a[3]*b.w; \}\n")
            *writer |> write("mat4 MUL(mat4 a, mat4 b) \{
                mat4 res;
                res[0] = a[0]*b[0].x + a[1]*b[0].y + a[2]*b[0].z + a[3]*b[0].w;
                res[1] = a[0]*b[1].x + a[1]*b[1].y + a[2]*b[1].z + a[3]*b[1].w;
                res[2] = a[0]*b[2].x + a[1]*b[2].y + a[2]*b[2].z + a[3]*b[2].w;
                res[3] = a[0]*b[3].x + a[1]*b[3].y + a[2]*b[3].z + a[3]*b[3].w;
                return res; \}\n")
            // and done
            *writer |> write("\n")
        if shaderType==ShaderType compute
            if !caps.output_hlsl_style_compute_layout
                if compute_local_size.z != 0
                    *writer |> write("layout (local_size_x={compute_local_size.x}, local_size_y={compute_local_size.y}, local_size_z={compute_local_size.z}) in;\n")
                elif compute_local_size.y != 0
                    *writer |> write("layout (local_size_x={compute_local_size.x}, local_size_y={compute_local_size.y}) in;\n")
                else
                    *writer |> write("layout (local_size_x={compute_local_size.x}) in;\n")
// type
    ET : bool = false
    def override preVisitTypeDecl(typ:TypeDeclPtr)
        ET = typ.isExprType
        if ET
            *writer |> write("/*[")
    def override visitTypeDecl(typ:TypeDeclPtr)
        if ET
            *writer |> write("]*/")
        return typ
// enumeration
    def override canVisitEnumeration(enu:Enumeration?)
        return false
// structure
    def override canVisitStructure(str:Structure?)
        if !(depStruct |> key_exists(intptr(str)))
            return false
        if str._module.name=="glsl_common"
            return false
        return true
    def override preVisitStructure(str:StructurePtr)
        if str.flags.isClass
            self->error("classes are not supported", str.at)
        self->line(str.at)
        *writer |> write("struct {str.name}")
        self->newLine(1)
        *writer |> write("\{")
        self->newLine(1)
    def override preVisitStructureField(str:StructurePtr;decl:FieldDeclaration;last:bool)
        self->line(decl.at)
        *writer |> write("\t")
        //if decl.annotation.length != 0
        //    *writer |> write("[{describe(decl.annotation)}] ")
        *writer |> write("{self->describe_glsl_type_ex(decl._type,false)} {decl.name}")
        if decl.init != null
            self->error("structure initialization is not supported", decl.init.at)
    def override visitStructureField(str:StructurePtr;decl:FieldDeclaration;last:bool)
        for d in decl._type.dim
            *writer |> write("[{d}]")
        *writer |> write(";")
        self->newLine(1)
    def override visitStructure(str:StructurePtr) : StructurePtr
        *writer |> write("};")
        self->newLine(2)
        if caps.output_structure_initializers
            *writer |> write("{str.name} {str.name}_STRUCTURE_INITIALIZER ( ")
            var first = true
            for fl in str.fields
                if first
                    first = false
                else
                    *writer |> write(", ")
                *writer |> write(self->describe_glsl_type(fl._type))
                *writer |> write(" _{fl.name}")
            *writer |> write(" )\n")
            *writer |> write("\{\n")
            *writer |> write("\t{str.name} RESULT;\n")
            for fl in str.fields
                *writer |> write("\tRESULT.{fl.name} = _{fl.name};\n")
            *writer |> write("\treturn RESULT;\n")
            *writer |> write("\}\n\n")
        return str
// function
    def override canVisitFunction(arg:Function?):bool
        if !depFun |> key_exists(intptr(arg))
            return false
        if arg._module.name=="glsl_common"
            return false
        if arg._module.name=="math" || arg._module.name=="math_bits"
            return false
        return true
    def override preVisitFunction(fun:FunctionPtr)
        if caps.restrict_array_as_result
            if length(fun.result.dim)>0
                self->error("returning arrays is prohibited in this shader model", fun.at)
        let isMain = self->isMainFunction(fun)
        if isMain && shaderType==ShaderType compute
            if caps.output_hlsl_style_compute_layout
                if compute_local_size.z != 0
                    *writer |> write("NUM_THREADS({compute_local_size.x},{compute_local_size.y},{compute_local_size.z})\n")
                elif compute_local_size.y != 0
                    *writer |> write("NUM_THREADS({compute_local_size.x},{compute_local_size.y},1)\n")
                else
                    *writer |> write("NUM_THREADS({compute_local_size.x},1,1)\n")
        self->line(fun.at)
        *writer |> write("{self->describe_glsl_type(fun.result)} ")
        var fnName = isMain ? "main" : self->glsl_function_name(fun)
        *writer |> write("{fnName}(")
    def override preVisitFunctionBody(fun:FunctionPtr)
        *writer |> write(")")
        for ann in fun.annotations
            if ann.annotation.name=="pixel_shader" // TODO: other ann
                let argo = find_arg("output",ann.arguments)
                if argo is tString
                    *writer |> write(" : {argo as tString}")
        self->newLine(1)
    def override visitFunction(fun:FunctionPtr) : FunctionPtr
        self->newLine(1)
        return fun
    def override preVisitFunctionArgument(fun:FunctionPtr;arg:VariablePtr;last:bool)
        if arg._type.isRef && !arg._type.flags.constant
            *writer |> write("inout ")
        *writer |> write("{self->describe_glsl_type(arg._type)} {arg.name} ")
    def override visitFunctionArgument(fun:FunctionPtr;arg:VariablePtr;last:bool)
        if !last
            *writer |> write(", ")
        return arg
    def override canVisitFunctionArgumentInit (fun:Function?; arg:VariablePtr;value:ExpressionPtr) : bool
        return false
// block
    def override preVisitExprBlock(blk:smart_ptr<ExprBlock>)
        if blk.blockFlags.isClosure
            self->error ("closure is not supported", blk.at)
        *writer |> write("{repeat("\t",tab)}\{")
        self->newLine(1)
        tab ++
    def override visitExprBlock(blk:smart_ptr<ExprBlock>) : ExpressionPtr
        tab --
        *writer |> write("{repeat("\t",tab)}\}")
        self->newLine(1)
        return blk
    def override preVisitExprBlockExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr)
        self->line(expr.at)
        *writer |> write("{repeat("\t",tab)}")
    def override visitExprBlockExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr)
        *writer |> write(";")
        self->newLine(1)
        return expr
    def override visitExprBlockFinal(blk:smart_ptr<ExprBlock>)
        self->error ("finally is not supported", blk.at)
// let
    def override preVisitExprLetVariable(expr:smart_ptr<ExprLet>;arg:VariablePtr;lastArg:bool)
        if arg._type.flags.ref
            self->error("local references are not supported by GLSL", arg.at)
        self->line(arg.at)
        *writer |> write("{self->describe_glsl_type_ex(arg._type,false)} {arg.name}")
        for d in arg._type.dim
            *writer |> write("[{d}]")
    def override visitExprLetVariable(expr:smart_ptr<ExprLet>;arg:VariablePtr;lastArg:bool)
        if arg.init == null
            var needZero = true
            if caps.restrict_array_as_result
                if caps.allow_unitialized_local_arrays
                    if length(arg._type.dim) > 0
                        needZero = false
            if needZero
                *writer |> write(" = ")
                self->produce_zero(arg._type)
        if !lastArg
            *writer |> write(";")
        return arg
    def override preVisitExprLetVariableInit(blk:smart_ptr<ExprLet>;arg:VariablePtr;expr:ExpressionPtr)
        // if arg.flags.init_via_move
        // elif arg.flags.init_via_clone
        *writer |> write(" = ")
// global let
    def override canVisitGlobalVariable(arg:Variable?):bool
        if !depVar |> key_exists(intptr(arg))
            return false
        if "{arg._module.name}" |> find("glsl_") != -1
            return false
        return true
    def checkUniformType ( typ:TypeDeclPtr )
        if !caps.restrict_uniform_types
            return
        if typ.baseType==Type tFloat4
            return
        if typ.baseType==Type tHandle
            if typ.annotation.name=="float3x3" || typ.annotation.name=="float4x4"
                return
        if typ.baseType==Type tStructure
            if "{typ.structType.name}" |> find("sampler")!=-1
                return
            if "{typ.structType.name}" |> find("image")!=-1
                return
        self->error("unsupported uniform type {describe(typ)}", typ.at)
    def getSamplerType(typ:TypeDeclPtr) : string
        if typ.baseType==Type tStructure
            if "{typ.structType.name}" |> find("sampler")!=-1
                return string(typ.structType.name)
        return ""
    def getImageType(typ:TypeDeclPtr) : string
        if typ.baseType==Type tStructure
            if "{typ.structType.name}" |> find("image")!=-1
                return string(typ.structType.name)
        return ""
    def outputSamplerMacro(arg:VariablePtr) : bool
        if !caps.output_unfirom_macro_declarations
            return false
        let st = self->getSamplerType(arg._type)
        if st==""
            return false
        let stage = (find_arg("stage",arg.annotation) ?as tInt) ?? 0
        let sfunc = to_upper(st)
        *writer |> write(sfunc)
        *writer |> write("({arg.name},{stage})")
        return true
    def outputImageMacro(arg:VariablePtr) : bool
        if !caps.output_unfirom_macro_declarations
            return false
        let st = self->getImageType(arg._type)
        if st==""
            return false
        let format =  (find_arg("format",arg.annotation) ?as tString) ?? "rgba8"
        let binding = (find_arg("stage",arg.annotation) ?as tInt) ?? 0
        var suffix = "_RW"
        if (find_arg("writeonly",arg.annotation) ?as tBool) ?? false
            suffix = "_WR"
        elif (find_arg("readonly",arg.annotation) ?as tBool) ?? false
            suffix = "_RO"
        let sfunc = to_upper(st)
        *writer |> write(sfunc)
        *writer |> write("{suffix}({arg.name},{format},{binding})")
        return true
    def override preVisitGlobalLetVariable(arg:VariablePtr;lastArg:bool)
        self->line(arg.at)
        if caps.restrict_array_as_result
            if length(arg._type.dim)>0
                let need_init = find_arg("need_init",arg.annotation) ?as tBool ?? true
                let group_shared = find_arg("groupshared",arg.annotation) ?as tBool ?? false
                if need_init && !group_shared
                    self->error("returning arrays is prohibited in this shader model",arg.at)
        if !caps.output_inout
            if ( !(find_arg("inout", arg.annotation) is nothing) ||
                 !(find_arg("in",  arg.annotation) is nothing) ||
                 !(find_arg("out", arg.annotation) is nothing) )
                *writer |> write("// ")
        if find_arg("ssbo", arg.annotation) is tBool
            var binding = find_arg("stage", arg.annotation) ?as tInt ?? 0
            *writer |> write("layout(std430, binding={binding}) buffer {arg.name}_ssbo")
            self->newLine(1)
            *writer |> write("\{")
            self->newLine(1)
            *writer |> write("    {self->describe_glsl_type(arg._type)} {arg.name};")
            self->newLine(1)
            *writer |> write("\}")
        else
            var needInit = find_arg("need_init", arg.annotation) ?as tBool ?? true
            if find_arg("groupshared", arg.annotation) ?as tBool ?? false
                needInit = false
            if shaderType==ShaderType vertex
                if find_arg("uniform", arg.annotation) is tBool
                    self->checkUniformType(arg._type)
                    if self->outputSamplerMacro(arg)
                        return
                    *writer |> write("uniform "); needInit = false
                let argLoc = find_arg("location", arg.annotation)
                if argLoc is tInt
                    *writer |> write("layout(location={argLoc as tInt}) "); needInit = false
                if find_arg("in", arg.annotation) is tBool
                    *writer |> write("in "); needInit = false
                if find_arg("inout", arg.annotation) is tBool
                    *writer |> write("out "); needInit = false
            elif shaderType==ShaderType fragment
                if find_arg("uniform", arg.annotation) is tBool
                    self->checkUniformType(arg._type)
                    if self->outputSamplerMacro(arg)
                        return
                    *writer |> write("uniform "); needInit = false
                if find_arg("in", arg.annotation) is tBool
                    *writer |> write("in "); needInit = false
                if find_arg("out", arg.annotation) is tBool
                    *writer |> write("out "); needInit = false
                if find_arg("inout", arg.annotation) is tBool
                    *writer |> write("in "); needInit = false
            elif shaderType==ShaderType compute
                if find_arg("groupshared", arg.annotation) is tBool
                    if caps.output_unfirom_macro_declarations
                        *writer |> write("SHARED ")
                    else
                        *writer |> write("shared ")
                if find_arg("uniform", arg.annotation) is tBool
                    self->checkUniformType(arg._type)
                    if self->outputSamplerMacro(arg)
                        return
                    if self->outputImageMacro(arg)
                        return
                    *writer |> write("uniform "); needInit = false
                if find_arg("in", arg.annotation) is tBool
                    *writer |> write("in "); needInit = false
                if find_arg("out", arg.annotation) is tBool
                    *writer |> write("out "); needInit = false
                if find_arg("inout", arg.annotation) is tBool
                    *writer |> write("in "); needInit = false
                if is_glsl_structure(arg,"image2D")
                    var layouts : array<string>
                    let i_writeonly = find_arg("writeonly", arg.annotation)
                    if i_writeonly is tBool
                        *writer |> write("writeonly "); needInit = false
                    let i_readonly = find_arg("readonly", arg.annotation)
                    if i_readonly is tBool
                        *writer |> write("readonly "); needInit = false
                    let s_format = find_arg("format", arg.annotation)
                    if s_format is tString
                        layouts |> push(s_format as tString)
                    let i_binding = find_arg("stage", arg.annotation)
                    if i_binding is tInt
                        layouts |> push("binding={i_binding as tInt}")
                    if !empty(layouts)
                        let lstr = join(layouts,",")
                        *writer |> write("layout({lstr}) "); needInit = false
            *writer |> write("{self->describe_glsl_type_ex(arg._type,false)} {arg.name}")
            for d in arg._type.dim
                *writer |> write("[{d}]")
            if arg.init==null && needInit
                *writer |> write(" = ")
                self->produce_zero(arg._type)
    def override visitGlobalLetVariable(arg:VariablePtr;lastArg:bool)
        let reg = find_arg("register", arg.annotation)
        if reg is tString
            *writer |> write(": register({reg as tString})")
        *writer |> write(";")
        self->newLine(2)
        var is_inout = false
        if caps.output_inout_stub
            is_inout = true
            if !(find_arg("inout", arg.annotation) is nothing)
                if shaderType==ShaderType vertex
                    inout_stub += "$output {arg.name}\n"
                elif shaderType==ShaderType compute || shaderType==ShaderType fragment
                    inout_stub += "$input {arg.name}\n"
            elif !(find_arg("in", arg.annotation) is nothing)
                inout_stub += "$input {arg.name}\n"
                if caps.restrict_vertex_attribute_names
                    if !glsl_restricted_vertex_attribute_names |> key_exists("{arg.name}")
                        var names <- [{for n in keys(glsl_restricted_vertex_attribute_names); n}]
                        let allowed_names = join(names,",")
                        self->error("invalid vertex attribute name {arg.name}, supported names are {allowed_names}", arg.at)
                        delete names
            elif !(find_arg("out", arg.annotation) is nothing)
                inout_stub += "$output {arg.name}\n"
            else
                is_inout = false
        if is_inout && caps.output_inout_decl
            let sema = find_arg("semantics", arg.annotation)
            var suffix = ""
            if sema is tString
                suffix = " : {sema as tString}"
            if caps.resolve_supported_semantics
                if suffix == ""
                    let aname = to_upper(string(arg.name))
                    if glsl_supported_semantics |> key_exists(aname)
                        suffix = " : {aname}"
                    else
                        let atail = slice(aname,2)
                        if glsl_supported_semantics |> key_exists(atail)
                            suffix = " : {atail}"
                if suffix == ""
                    self->error("missing semantics for {arg.name}", arg.at)
            inout_decl +="{self->describe_glsl_type(arg._type)} {arg.name}{suffix};\n"
        return arg
    def override preVisitGlobalLetVariableInit(arg:VariablePtr;expr:ExpressionPtr)
        // if arg.flags.init_via_move
        // elif arg.flags.init_via_clone
        *writer |> write(" = ")
// string builder
    def override preVisitExprStringBuilder(expr:smart_ptr<ExprStringBuilder>)
        self->error ("string builder is not supported", expr.at)
// new
    def override preVisitExprNew(expr:smart_ptr<ExprNew>)
        self->error ("new is not supported", expr.at)
// named call
    def override preVisitExprNamedCall(expr:smart_ptr<ExprNamedCall>)
        self->error ("named call is not supported", expr.at)
// looks like call (debug,assert,verify,erase,find,key_exists,keys,values,invoke,memzero etc)
    def isInvokeMethod(expr:smart_ptr<ExprLooksLikeCall>)
        return (expr ?as ExprInvoke)?.isInvokeMethod ?? false
    def override preVisitExprLooksLikeCall(expr:smart_ptr<ExprLooksLikeCall>)
        if self->isInvokeMethod(expr)
            pass
        else
            *writer |> write("{expr.name}(")
    def override visitExprLooksLikeCall(expr:smart_ptr<ExprLooksLikeCall>) : ExpressionPtr
        *writer |> write(")")
        return expr
    def override preVisitExprLooksLikeCallArgument(expr:smart_ptr<ExprLooksLikeCall>;arg:ExpressionPtr;last:bool)
        let isInvoke = self->isInvokeMethod(expr)
        if isInvoke && arg==expr.arguments[1]
            *writer |> write("/*")
    def override visitExprLooksLikeCallArgument(expr:smart_ptr<ExprLooksLikeCall>;arg:ExpressionPtr;last:bool)
        let isInvoke = self->isInvokeMethod(expr)
        if isInvoke && arg==expr.arguments[0]
            *writer |> write("(")
        elif isInvoke && arg==expr.arguments[1]
            *writer |> write("*/")
        elif !last
            *writer |> write(",")
        return arg
// call
    def override preVisitExprCall(expr:smart_ptr<ExprCall>)
        var fnName : string
        unsafe
            fnName = self->glsl_function_name(reinterpret<FunctionPtr> expr.func)
        if caps.output_compatibility_constructors
            glsl_compatibility_constructors |> find_if_exists(fnName) <| $(newName:string?)
                if newName != null
                    fnName = *newName
        glsl_functions |> find_if_exists(fnName) <| $(newName:string?)
            if newName != null
                fnName = *newName
        *writer |> write("{fnName}(")
    def override visitExprCall(expr:smart_ptr<ExprCall>) : ExpressionPtr
        *writer |> write(")")
        return expr
    def override visitExprCallArgument(expr:smart_ptr<ExprCall>;arg:ExpressionPtr;last:bool)
        if !last
            *writer |> write(",")
        return arg
// null coaelescing
    def override preVisitExprNullCoalescing(expr:smart_ptr<ExprNullCoalescing>): void
        self->error ("null coaelescing is not supported", expr.at)
// at
    def override visitExprAt(expr:smart_ptr<ExprAt>) : ExpressionPtr
        *writer |> write("]")
        return expr
    def override preVisitExprAtIndex(expr:smart_ptr<ExprAt>;index:ExpressionPtr)
        *writer |> write("[")
// safe at
    def override preVisitExprSafeAt(expr:smart_ptr<ExprSafeAt>): void
        self->error ("safe index is not supported", expr.at)
// is
    def override preVisitExprIsType(expr:smart_ptr<ExprAt>;typeDecl:TypeDeclPtr)
        self->error ("is type is not supported", expr.at)
// op2
    def getReplaceOp2(expr:smart_ptr<ExprOp2>) : tuple<fun:string;replOp:bool>
        if expr.op=="%" && !expr._type.isInteger
            return [[auto "mod",true]]
        if caps.replace_hlsl_incompatible_operations
            if expr.op=="*"
                if is_any_matrix(expr.left._type) || is_any_matrix(expr.right._type)
                    return [[auto "MUL",true]]
            if expr.op=="==" || expr.op=="!="
                if is_any_matrix(expr.left._type) || expr.left._type.isVectorType || is_any_matrix(expr.right._type) || expr.right._type.isVectorType
                    return [[auto "all", false]]
        return [[auto "",false]]
    def override preVisitExprOp2(expr:smart_ptr<ExprOp2>)
        let repl = self->getReplaceOp2(expr)
        *writer |> write("{repl.fun}(")
    def override visitExprOp2(expr:smart_ptr<ExprOp2>) : ExpressionPtr
        *writer |> write(")")
        return expr
    def override preVisitExprOp2Right(expr:smart_ptr<ExprOp2>;right:ExpressionPtr)
        let repl = self->getReplaceOp2(expr)
        if repl.replOp
            *writer |> write(", ")
        else
            *writer |> write(" {expr.op} ")
// op3
    def override preVisitExprOp3(expr:smart_ptr<ExprOp3>)
        *writer |> write("(")
    def override visitExprOp3(expr:smart_ptr<ExprOp3>) : ExpressionPtr
        *writer |> write(")")
        return expr
    def override preVisitExprOp3Left(expr:smart_ptr<ExprOp3>;left:ExpressionPtr)
        *writer |> write(" ? ")
    def override preVisitExprOp3Right(expr:smart_ptr<ExprOp3>;right:ExpressionPtr)
        *writer |> write(" : ")
// copy
    def override preVisitExprCopyRight(expr:smart_ptr<ExprCopy>;right:ExpressionPtr)
        *writer |> write(" = ")
// move
    def override preVisitExprMoveRight(expr:smart_ptr<ExprMove>;right:ExpressionPtr)
        self->error ("move is not supported", expr.at)
// clone
    def override preVisitExprCloneRight(expr:smart_ptr<ExprClone>;right:ExpressionPtr)
        self->error ("clone is not supported", expr.at)
// with
    def override preVisitExprWith(expr:smart_ptr<ExprWith>)
        self->error ("with is not supported", expr.at)
// while
    def override preVisitExprWhile(expr:smart_ptr<ExprWhile>)
        *writer |> write("while ( ")
    def override preVisitExprWhileBody(expr:smart_ptr<ExprWhile>;right:ExpressionPtr)
        *writer |> write(") ")
        self->newLine(1)
// try-catch
    def override preVisitExprTryCatch(expr:smart_ptr<ExprTryCatch>)
        self->error ("try-recover is not supported", expr.at)
// if-then-else
    def override preVisitExprIfThenElse(expr:smart_ptr<ExprIfThenElse>)
        *writer |> write("if (")
    def override preVisitExprIfThenElseIfBlock(expr:smart_ptr<ExprIfThenElse>;ifBlock:ExpressionPtr)
        *writer |> write(")")
        self->newLine(1)
    def override preVisitExprIfThenElseElseBlock(expr:smart_ptr<ExprIfThenElse>;elseBlock:ExpressionPtr)
        *writer |> write("{repeat("\t",tab)}")
        if elseBlock.__rtti=="ExprIfThenElse"
            *writer |> write("else ")
        else
            *writer |> write("else")
            self->newLine(1)
// for
    def isDimFor(svar:ExpressionPtr)
        if length(svar._type.dim)>0
            return true
        return false
    def isRangeFor(svar:ExpressionPtr)
        let bt = svar._type.baseType
        if bt==Type tRange || bt==Type tURange
            return true
        return false
    def override preVisitExprFor(expr:smart_ptr<ExprFor>)
        for source in expr.sources
            if !self->isRangeFor(source) && !self->isDimFor(source)
                self->error("only range for or dim for are supported for now {describe(source._type)}", source.at)
                return
        *writer |> write("\{")
    def override preVisitExprForSource(expr:smart_ptr<ExprFor>;source:ExpressionPtr;last:bool) : void
        self->newLine(1)
        let index = get_for_source_index(expr, source)
        if self->isDimFor(source)
            *writer |> write("{repeat("\t",tab)}int {expr.iteratorVariables[index].name} = 0; //")
        elif self->isRangeFor(source)
            let svtype = "{self->describe_glsl_type(source._type)}"
            *writer |> write("{repeat("\t",tab)}{svtype} _for_range_{expr.iteratorVariables[index].name} = ")
    def override visitExprForSource(expr:smart_ptr<ExprFor>;source:ExpressionPtr;last:bool) : ExpressionPtr
        var index = get_for_source_index(expr,source)
        if self->isDimFor(source)
            pass
        elif self->isRangeFor(source)
            *writer |> write(";")
            self->newLine(1)
            let sname = "{expr.iteratorVariables[index].name}"
            let stype = "{self->describe_glsl_type(expr.iteratorVariables[index]._type)}"
            *writer |> write("{repeat("\t",tab)}{stype} {sname} = _for_range_{sname}.x;")
        return source
    def override preVisitExprForBody(expr:smart_ptr<ExprFor>)
        self->newLine(1)
        *writer |> write("{repeat("\t",tab)}while ( ")
        for source,svar,iter in expr.sources,expr.iteratorVariables,expr.iterators
            if iter != expr.iterators[0]
                *writer |> write(" && ")
            if self->isDimFor(source)
                let sdim = source._type.dim[length(source._type.dim)-1]
                *writer |> write("{svar.name}!={sdim}")
                let newsrc = "{self->describe_subexpression(source)}[{svar.name}]"
                renames |> push([[auto "{iter}", newsrc]])
            elif self->isRangeFor(source)
                *writer |> write("{svar.name}!=_for_range_{svar.name}.y")
        *writer |> write(" )")
        self->newLine(1)
        *writer |> write("{repeat("\t",tab)}\{")
        self->newLine(1)
        *writer |> write("{repeat("\t",tab-1)}")
    def override visitExprFor(expr:smart_ptr<ExprFor>) : ExpressionPtr
        for svar,source in expr.iteratorVariables,expr.sources
            *writer |> write("{repeat("\t",tab)}")
            if self->isDimFor(source)
                renames |> pop()
                *writer |> write("{svar.name}++;")
            elif self->isRangeFor(source)
                *writer |> write("{svar.name}++;")
            self->newLine(1)
        *writer |> write("{repeat("\t",tab)}\}\}")
        return expr
// make variant
    def override preVisitExprMakeVariant(expr:smart_ptr<ExprMakeVariant>)
        self->error ("variant is not supported", expr.at)
// make structure
    def outputMakeStruct(expr:smart_ptr<ExprMakeStruct>;index:int)
        var prev = -1
        for st,j in *(expr.structs[index]),range(100500)
            for sf,i in expr._type.structType.fields,range(100500)
                if sf.name == st.name
                    if prev > i
                        self->error("out of order initialization is not supported by GLSL yet", st.at)
                    if !st.value.flags.noSideEffects
                        prev = i
        for sf,i in expr._type.structType.fields,range(100500)
            if i!=0
                *writer |> write(",")
            var has = false
            for st,j in *(expr.structs[index]),range(100500)
                if sf.name == st.name
                    has = true
                    visit(st.value, astVisitorAdapter)
            if !has
                self->produce_zero(sf._type)
    def override preVisitExprMakeStruct(expr:smart_ptr<ExprMakeStruct>)
        if expr._block != null
            self->error ("structure initialization with the block is not supported", expr.at)
        *writer |> write("{expr.makeType.structType.name}")
        if caps.output_structure_initializers
            *writer |> write("_STRUCTURE_INITIALIZER")
        let nStructs = length(expr.structs)
        if nStructs!=1
            *writer |> write("[{nStructs}]")
        *writer |> write("(")
        if nStructs!=1
            for n in range(nStructs)
                if n!=0
                    *writer |> write(",")
                *writer |> write("{expr.makeType.structType.name}(")
                self->outputMakeStruct(expr,n)
                *writer |> write(")")
        else
            self->outputMakeStruct(expr,0)
        *writer |> write(")")
    def override canVisitMakeStructBody(expr:smart_ptr<ExprMakeStruct>) : bool
        return false
// make array
    def override preVisitExprMakeArray(expr:smart_ptr<ExprMakeArray>)
        let atype = self->describe_glsl_type(expr._type)
        *writer |> write("({atype}(")
    def override visitExprMakeArray(expr:smart_ptr<ExprMakeArray>) : ExpressionPtr
        *writer |> write("))")
        return expr
    def override preVisitExprMakeArrayIndex(expr:smart_ptr<ExprMakeArray>;index:int;init:ExpressionPtr;last:bool) : void
        *writer |> write("(")
    def override visitExprMakeArrayIndex(expr:smart_ptr<ExprMakeArray>;index:int;init:ExpressionPtr;last:bool) : ExpressionPtr
        *writer |> write(")")
        if !last
            *writer |> write(",")
        return init

// make tuple
    def override preVisitExprMakeTuple(expr:smart_ptr<ExprMakeTuple>)
        // TODO: support nameless structure
        self->error ("tuple is not supported YET", expr.at)
// array comprehension
    def override preVisitExprArrayComprehension(expr:smart_ptr<ExprArrayComprehension>)
        self->error ("array comprehension is not supported", expr.at)
// type info
    def override preVisitExprTypeInfo(expr:smart_ptr<ExprTypeInfo>)
        self->error ("typeinfo is not supported", expr.at)
// ptr to ref
    def override preVisitExprPtr2Ref(expr:smart_ptr<ExprPtr2Ref>)
        self->error ("deref is not supported", expr.at)
// label
    def override preVisitExprLabel(expr:smart_ptr<ExprLabel>)
        self->error ("label is not supported", expr.at)
// goto
    def override preVisitExprGoto(expr:smart_ptr<ExprGoto>)
        self->error ("goto is not supported", expr.at)
// ref 2 ptr
    def override preVisitExprRef2Ptr(expr:smart_ptr<ExprRef2Ptr>)
        self->error ("addr is not supported", expr.at)
// ref to value
    def override preVisitExprRef2Value(expr:smart_ptr<ExprRef2Value>)
        *writer |> write("/*r2v*/ (")
    def override visitExprRef2Value(expr:smart_ptr<ExprRef2Value>) : ExpressionPtr
        *writer |> write(")")
        return expr
// @@
    def override preVisitExprAddr(expr:smart_ptr<ExprAddr>)
        self->error ("function addr is not supported", expr.at)
// assert / verify
    def override preVisitExprAssert(expr:smart_ptr<ExprAssert>) : void
        self->error ("assert is not supported", expr.at)
// static_assert
    def override preVisitExprStaticAssert(expr:smart_ptr<ExprStaticAssert>) : void
        self->error ("static_assert is not supported", expr.at)
// quote
    def override preVisitExprQuote(expr:smart_ptr<ExprQuote>) : void
        self->error ("quote is not supported", expr.at)
// debug
    def override preVisitExprDebug(expr:smart_ptr<ExprDebug>) : void
        self->error ("debug is not supported", expr.at)
// invoke
    def override preVisitExprInvoke(expr:smart_ptr<ExprInvoke>) : void
        self->error ("invoke is not supported", expr.at)
// erase
    def override preVisitExprErase(expr:smart_ptr<ExprErase>) : void
        self->error ("erase is not supported", expr.at)
// find
    def override preVisitExprFind(expr:smart_ptr<ExprFind>) : void
        self->error ("find is not supported", expr.at)
// key exists
    def override preVisitExprKeyExists(expr:smart_ptr<ExprKeyExists>) : void
        self->error ("keyexists is not supported", expr.at)
// ascend
    def override preVisitExprAscend(expr:smart_ptr<ExprAscend>)
        self->error ("new is not supported", expr.at)
// cast
    def override preVisitExprCast(expr:smart_ptr<ExprCast>)
        self->error ("cast is not supported", expr.at)
// delete
    def override preVisitExprDelete(expr:smart_ptr<ExprDelete>)
        self->error ("delete is not supported", expr.at)
// var
    def override preVisitExprVar(expr:smart_ptr<ExprVar>)
        let l = length(renames)
        let en = "{expr.name}"
        for i in range(l)
            let ri = l - i - 1
            if renames[ri].name == en
                *writer |> write(renames[ri].repl)
                return
        *writer |> write("{expr.name}")
// field
    def override visitExprField(expr:smart_ptr<ExprField>) : ExpressionPtr
        *writer |> write(".{expr.name}")
        return expr
// safe field
    def override preVisitExprSafeField(expr:smart_ptr<ExprSafeField>) : void
        self->error ("'?.' is not supported", expr.at)
// swizzle
    def override visitExprSwizzle(expr:smart_ptr<ExprSwizzle>) : ExpressionPtr
        *writer |> write(".")
        let f2name = [[string "x";"y";"z";"w"]]
        for fch in expr.fields
            let f = int(fch)
            *writer |> write("{f>=0 && f<=3 ? f2name[f] : "?"}")
        return expr
// is variant
    def override preVisitExprIsVariant(expr:smart_ptr<ExprIsVariant>) : void
        self->error ("'is' is not supported", expr.at)
// as variant
    def override preVisitExprAsVariant(expr:smart_ptr<ExprAsVariant>) : void
        self->error ("'as' is not supported", expr.at)
// safe as variant
    def override preVisitExprSafeAsVariant(expr:smart_ptr<ExprSafeAsVariant>) : void
        self->error ("'?as' is not supported", expr.at)
// op1
    def override preVisitExprOp1(expr:smart_ptr<ExprOp1>) : void
        let op = "{expr.op}"
        if op!="+++" && op!="---"
            *writer |> write(op)
        *writer |> write("(")
    def override visitExprOp1(expr:smart_ptr<ExprOp1>) : ExpressionPtr
        let op = "{expr.op}"
        *writer |> write(")")
        if op=="+++" || op=="---"
            *writer |> write("{slice(op,0,-1)}")
        return expr
// return
    def override preVisitExprReturn(expr:smart_ptr<ExprReturn>) : void
        *writer |> write("return ")
// yield
    def override preVisitExprYield(expr:smart_ptr<ExprYield>) : void
        self->error ("yield is not supported", expr.at)
// break
    def override preVisitExprBreak(expr:smart_ptr<ExprBreak>) : void
        *writer |> write("break")
// continue
    def override preVisitExprContinue(expr:smart_ptr<ExprContinue>) : void
        *writer |> write("continue")
// make block
    def override preVisitExprMakeBlock(expr:smart_ptr<ExprMakeBlock>) : void
        self->error ("'make block' is not supported", expr.at)
// make generator
    def override preVisitExprMakeGenerator(expr:smart_ptr<ExprMakeGenerator>) : void
        self->error ("'make generator' is not supported", expr.at)
// memzero
    def override preVisitExprMemZero(expr:smart_ptr<ExprMemZero>) : void
        self->error ("memzero is not supported", expr.at)
// const ptr
    def override preVisitExprConstPtr(expr:smart_ptr<ExprConstPtr>) : void
        self->error ("pointer is not supported", expr.at)
// const enumeraiton
    def override preVisitExprConstEnumeration(expr:smart_ptr<ExprConstEnumeration>) : void
        let val = find_enum_value(expr.enumType, "{expr.value}")
        *writer |> write("{int(val)}")
// const bitfield
    def override preVisitExprConstBitfield(expr:smart_ptr<ExprConstBitfield>) : void
        *writer |> write("{expr.value}")
// const int 8
    def override preVisitExprConstInt8(expr:smart_ptr<ExprConstInt8>) : void
        *writer |> write("{expr.value}")
// const int 16
    def override preVisitExprConstInt16(expr:smart_ptr<ExprConstInt16>) : void
        *writer |> write("{expr.value}")
// const int 64
    def override preVisitExprConstInt64(expr:smart_ptr<ExprConstInt64>) : void
        *writer |> write("{expr.value}")
// const int
    def override preVisitExprConstInt(expr:smart_ptr<ExprConstInt>) : void
        *writer |> write("{expr.value}")
// const int2
    def override preVisitExprConstInt2(expr:smart_ptr<ExprConstInt2>) : void
        if caps.output_compatibility_constructors
            *writer |> write("IVEC2({expr.value})")
        else
            *writer |> write("ivec2({expr.value})")
// const int3
    def override preVisitExprConstInt3(expr:smart_ptr<ExprConstInt3>) : void
        if caps.output_compatibility_constructors
            *writer |> write("IVEC3({expr.value})")
        else
            *writer |> write("ivec3({expr.value})")
// const int4
    def override preVisitExprConstInt4(expr:smart_ptr<ExprConstInt4>) : void
        if caps.output_compatibility_constructors
            *writer |> write("IVEC4({expr.value})")
        else
            *writer |> write("ivec4({expr.value})")
// const uint 8
    def override preVisitExprConstUInt8(expr:smart_ptr<ExprConstUInt8>) : void
        *writer |> write("{expr.value}")
// const uint 16
    def override preVisitExprConstUInt16(expr:smart_ptr<ExprConstUInt16>) : void
        *writer |> write("{expr.value}")
// const uint 64
    def override preVisitExprConstUInt64(expr:smart_ptr<ExprConstUInt64>) : void
        *writer |> write("{expr.value}")
// const uint
    def override preVisitExprConstUInt(expr:smart_ptr<ExprConstUInt>) : void
        *writer |> write("{expr.value}u")
// const uint2
    def override preVisitExprConstUInt2(expr:smart_ptr<ExprConstUInt2>) : void
        if caps.output_compatibility_constructors
            *writer |> write("UVEC2({expr.value.x}u,{expr.value.y}u)")
        else
            *writer |> write("uvec2({expr.value})")
// const uint3
    def override preVisitExprConstUInt3(expr:smart_ptr<ExprConstUInt3>) : void
        if caps.output_compatibility_constructors
            *writer |> write("UVEC3({expr.value.x}u, {expr.value.y}u, {expr.value.z}u)")
        else
            *writer |> write("uvec3({expr.value})")
// const uint4
    def override preVisitExprConstUInt4(expr:smart_ptr<ExprConstUInt4>) : void
        if caps.output_compatibility_constructors
            *writer |> write("UVEC4({expr.value.x}u,{expr.value.y}u,{expr.value.z}u,{expr.value.w}u)")
        else
            *writer |> write("uvec4({expr.value})")
// const range
    def override preVisitExprConstRange(expr:smart_ptr<ExprConstRange>) : void
        if caps.output_compatibility_constructors
            *writer |> write("IVEC2({expr.value})")
        else
            *writer |> write("ivec2({expr.value})")
// const urnage
    def override preVisitExprConstURange(expr:smart_ptr<ExprConstURange>) : void
        if caps.output_compatibility_constructors
            *writer |> write("UVEC2({expr.value.x}u,{expr.value.y}u)")
        else
            *writer |> write("uvec2({expr.value})")
// const bool
    def override preVisitExprConstBool(expr:smart_ptr<ExprConstBool>) : void
        *writer |> write("{expr.value}")
// const float
    def override preVisitExprConstFloat(expr:smart_ptr<ExprConstFloat>) : void
        *writer |> format("%g",expr.value)
// const float2
    def override preVisitExprConstFloat2(expr:smart_ptr<ExprConstFloat2>) : void
        if caps.output_compatibility_constructors
            *writer |> write("VEC2(")
        else
            *writer |> write("vec2(")
        *writer |> format("%g", expr.value.x)
        *writer |> write(",")
        *writer |> format("%g", expr.value.y)
        *writer |> write(")")
// const float3
    def override preVisitExprConstFloat3(expr:smart_ptr<ExprConstFloat3>) : void
        if caps.output_compatibility_constructors
            *writer |> write("VEC3(")
        else
            *writer |> write("vec3(")
        *writer |> format("%g", expr.value.x)
        *writer |> write(",")
        *writer |> format("%g", expr.value.y)
        *writer |> write(",")
        *writer |> format("%g", expr.value.z)
        *writer |> write(")")
// const float4
    def override preVisitExprConstFloat4(expr:smart_ptr<ExprConstFloat4>) : void
        if caps.output_compatibility_constructors
            *writer |> write("VEC4(")
        else
            *writer |> write("vec4(")
        *writer |> format("%g", expr.value.x)
        *writer |> write(",")
        *writer |> format("%g", expr.value.y)
        *writer |> write(",")
        *writer |> format("%g", expr.value.z)
        *writer |> write(",")
        *writer |> format("%g", expr.value.w)
        *writer |> write(")")
// string
    def override preVisitExprConstString(expr:smart_ptr<ExprConstString>) : void
        self->error ("string constant is not supported", expr.at)
// const double
    def override preVisitExprConstDouble(expr:smart_ptr<ExprConstDouble>) : void
        *writer |> write("{expr.value}")
// fake context
    def override preVisitExprFakeContext(expr:smart_ptr<ExprFakeContext>) : void
        self->error ("__context__ is not supported", expr.at)
// fake line info
    def override preVisitExprFakeLineInfo(expr:smart_ptr<ExprFakeLineInfo>) : void
        self->error ("__lineinfo__ is not supported", expr.at)
// reader
    def override preVisitExprReader(expr:smart_ptr<ExprReader>) : void
        self->error ("call macro is not supported", expr.at)
// call macro
    def override preVisitExprCallMacro(expr:smart_ptr<ExprCallMacro>) : void
        self->error ("call macro is not supported", expr.at)

[macro_function]
def generate_glsl(fnMain:FunctionPtr; var errors:array<string>; shaderType:ShaderType; version:int; comp_loc_size:int3; caps:ShaderExportCaps ) : tuple<body:string;inout_stub:string;inout_decl:string>
    var inout_stub = ""
    var inout_decl = ""
    var st = build_string() <| $ (var writer)
        var astVisitor = new GlslExport(writer,shaderType,version,comp_loc_size, caps)
        unsafe
            astVisitor.astVisitorAdapter <- make_visitor(*astVisitor)
        collect_dependencies(fnMain) <| $ ( vfun, vvar )
            for f in vfun
                astVisitor.depFun[intptr(f)] = true
            for v in vvar
                astVisitor.depVar[intptr(v)] = true
            collect_used_types(vfun, vvar) <| $ ( onlyUsed  ) : void
                for st in keys(onlyUsed.st)
                    astVisitor.depStruct[intptr(st)] = true
                    astVisitor.stnames["{st.name}"] = true
                for en in keys(onlyUsed.en)
                    astVisitor.depStruct[intptr(en)] = true
                visit_modules(compiling_program(), astVisitor.astVisitorAdapter)
        errors <- astVisitor.errors
        inout_stub = astVisitor.inout_stub
        inout_decl = astVisitor.inout_decl
        astVisitor.astVisitorAdapter := null
        unsafe
            delete astVisitor
    if DEBUG_SHADER_TEXT
        print("{st}\n\n")
    return [[auto st, inout_stub, inout_decl]]

[macro_function]
def bind_uniform_function_name(fnMain:FunctionPtr)
    return "{fnMain.name}_bind_uniform"

[macro_function]
def is_glsl_structure(arg; name:string)
    if arg._type.baseType==Type tStructure && length(arg._type.dim)==0
        if arg._type.structType._module.name=="glsl_common" && arg._type.structType.name==name
            return true
    return false

[macro_function]
def is_any_matrix(typ:TypeDeclPtr)
    if typ.baseType != Type tHandle
        return false
    return typ.annotation.name=="float3x3" || typ.annotation.name=="float3x4" || typ.annotation.name=="float4x4"

struct public FieldDeclInfo
    sizei : int
    typet : Type
    normalized : bool
    as_int : bool

[macro_function]
def get_decl_type_from_name ( name:string )
    if name==""
        return Type none
    elif name=="float" || name=="FLOAT" || name=="GL_FLOAT"
        return Type tFloat
    elif name=="uint8" || name=="UINT8" || name=="GL_BYTE"
        return Type tUInt8
    elif name=="int16" || name=="INT16" || name=="GL_SHORT"
        return Type tInt16
    elif name=="double" || name=="DOUBLE" || name=="GL_DOUBLE"
        return Type tDouble
    return Type none

[macro_function]
def get_decl_info ( typ:TypeDeclPtr; arguments:AnnotationArgumentList ) : tuple<ok:bool; info:FieldDeclInfo>
    if length(typ.dim)>0 || !(typ.isNumeric || typ.isVectorType)
        return [[auto false, [[FieldDeclInfo]]]]
    var fdi = [[FieldDeclInfo
        sizei = find_arg("size", arguments) ?as tInt ?? -1,
        typet = get_decl_type_from_name(find_arg("type", arguments) ?as tString ?? ""),
        normalized = (find_arg("normalized", arguments) ?as tBool) ?? false,
        as_int = (find_arg("as_int", arguments) ?as tBool) ?? false
    ]]
    if fdi.sizei==-1
        fdi.sizei = typ.isVectorType ? typ.vectorDim : 1
    if fdi.typet == Type none
        if typ.baseType==Type tFloat || typ.baseType==Type tFloat2 || typ.baseType==Type tFloat3 || typ.baseType==Type tFloat4
            fdi.typet = Type tFloat
        elif typ.baseType==Type tUInt8 || typ.baseType==Type tInt16 || typ.baseType==Type tDouble
            fdi.typet = typ.baseType
    return fdi.typet!=Type none ? [[auto true, fdi]] : [[auto false, [[FieldDeclInfo]]]]

var public glsl_supported_semantics <- {{
	"POSITION" => true;
	"NORMAL" => true;
	"TANGENT" => true;
	"BITANGENT" => true;
	"COLOR0" => true;
	"COLOR1" => true;
	"COLOR2" => true;
	"COLOR3" => true;
	"INDICES" => true;
	"WEIGHT" => true;
	"TEXCOORD0" => true;
	"TEXCOORD1" => true;
	"TEXCOORD2" => true;
	"TEXCOORD3" => true;
	"TEXCOORD4" => true;
	"TEXCOORD5" => true;
	"TEXCOORD6" => true;
	"TEXCOORD7" => true
}}
