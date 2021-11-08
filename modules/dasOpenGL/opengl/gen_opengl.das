require fio
require daslib/defer
require daslib/json
require daslib/json_boost
require daslib/strings_boost
require daslib/regex
require daslib/regex_boost
require math


var blocked_defines : array<string> <- [{string []
    "true";
    "false"
}]

var blocked_defines_table : table<string;bool>

var is_const_blocked : lambda<(name:string):bool>

var const_type_table : table<string; string>

def genDefineConstants(fname,prefix:string)
    var fnames <- [{ auto[] fname }]
    defer <| { delete fnames; }
    fnames |> genDefineConstants <| prefix

def getGenConstType ( name, default_name:string )
    return const_type_table?[name] ?? default_name

def searchAndGenConst ( var regex_def:Regex; defTName,suffix:string; hex:bool; var ofs:table<int; bool>; data:string; var dup:table<string;bool>; hf:FILE const? )
    regex_foreach(regex_def,data) <| $ ( r )
        if ! ofs |> key_exists(r.x)
            let DEF = regex_group(regex_def,1,data)
            let VAL = regex_group(regex_def,2,data)
            if DEF |> starts_with("__")
                print("WARNING: skipping {DEF} due to name limitations\n")
                return true
            var suf = suffix
            if hex && suffix=="u"
                let v = to_uint64(VAL,true)
                if v > 0xfffffffful
                    print("{DEF} promoted to ul\n")
                    suf = "ul"
            if !blocked_defines_table |> key_exists(DEF)
                if is_const_blocked==null || !invoke(is_const_blocked,DEF)
                    let ctt = getGenConstType(DEF,defTName)
                    if !dup[DEF]
                        fprint(hf,"let {DEF} = {ctt}({VAL}{suf})\n")
                        dup[DEF] = true
        return true

def genDefineConstants(fnames:array<string>; gen_file:string)
    delete blocked_defines_table
    for bd in blocked_defines
        blocked_defines_table[bd] = true
    var reg_def_hex <- %regex~#define\s+(\w+)\s+(0x[0-9A-Fa-f]+)%%
    var reg_def_dec <- %regex~#define\s+(\w+)\s+(\d+)%%
    var reg_def_UINT8 <- %regex~#define\s+(\w+)\s+UINT8_C\s*\((0x[0-9A-Fa-f]+)\)%%
    var reg_def_UINT16 <- %regex~#define\s+(\w+)\s+UINT16_C\s*\((0x[0-9A-Fa-f]+)\)%%
    var reg_def_UINT32 <- %regex~#define\s+(\w+)\s+UINT32_C\s*\((0x[0-9A-Fa-f]+)\)%%
    var reg_def_UINT64 <- %regex~#define\s+(\w+)\s+UINT64_C\s*\((0x[0-9A-Fa-f]+)\)%%
    fopen(gen_file,"wb") <| $(hf)
        if hf==null
            panic("can't write {gen_file}")
        for fname in fnames
            fopen(fname,"rb") <| $( f )
                if f == null
                    panic("can't open {fname}")
                var ofs : table<int; bool>
                let data = fread(f)
                var dup : table<string; bool>
                searchAndGenConst(reg_def_hex, "uint", "u", true, ofs, data, dup, hf)
                searchAndGenConst(reg_def_dec, "int", "", false, ofs, data, dup, hf)
                searchAndGenConst(reg_def_UINT8, "uint8", "u", true, ofs, data, dup, hf)
                searchAndGenConst(reg_def_UINT16, "uint16", "u", true, ofs, data, dup, hf)
                searchAndGenConst(reg_def_UINT32, "uint", "u", true, ofs, data, dup, hf)
                searchAndGenConst(reg_def_UINT64, "uint64", "ul", true, ofs, data, dup, hf)

var reg_const_ptr_type <- %regex~const\s*(\w+)\s*\*%%

var reg_ptr_type <- %regex~(\w+)\s*\*%%

let types <- {{
    "void"          =>  "void";
    "GLboolean"     =>  "bool";
    "GLint"         =>  "int";
    "GLsizei"       =>  "int";
    "GLchar"        =>  "int";
    "GLbyte"        =>  "int8";
    "GLubyte"       =>  "uint8";
    "GLushort"      =>  "uint16";
    "GLshort"       =>  "int16";
    "GLuint"        =>  "uint";
    "GLbitfield"    =>  "uint";
    "GLenum"        =>  "uint";
    "GLintptr"      =>  "int64";
    "GLsizeiptr"    =>  "int64";
    "GLint64"       =>  "int64";
    "GLint64EXT"    =>  "int64";
    "GLuint64"      =>  "uint64";
    "GLuint64EXT"   =>  "uint64";
    "void"          =>  "void";
    "GLsync"        =>  "void?";
    "GLfloat"       =>  "float";
    "GLdouble"      =>  "double"
}}

let unsupported <- {{
    "GLeglImageOES" =>  false;
    "GLVULKANPROCNV"=>  false;
    "GLDEBUGPROC"   =>  false;
    "GLDEBUGPROCARB"    =>  false;
    "struct _cl_context *"  =>  false;
    "struct _cl_event *"    =>  false
}}

let arg_names <- {{
    "type"      =>  "_type";
    "string"    =>  "_string";
    "array"     =>  "_array";
    "range"     =>  "_range";
    "label"     =>  "_label"
}}

let string_types <- {{
    "const GLchar *"        => "string";
    "const GLchar *const*"  => "string const?";
    "GLchar *"              => "string const"
}}

def glTypeToDasType ( s : string )
    if string_types |> key_exists(s)
        return string_types?[s] ?? ""
    if regex_match(reg_const_ptr_type,s) != -1
        let ct = regex_group(reg_const_ptr_type,1,s)
        let pt = _glTypeToDasType(ct)
        if pt == "*"
            print("UNMATCHED CONST TYPE {ct} of {s}\n")
            return "*"
        elif pt == ""
            print("MISSING CONST TYPE {ct} of {s}\n")
            return ""
        else
            return "{pt} const?"
    elif regex_match(reg_ptr_type,s) != -1
        let ct = regex_group(reg_ptr_type,1,s)
        let pt = _glTypeToDasType(ct)
        if pt == "*"
            print("UNMATCHED CONST TYPE {ct} of {s}\n")
            return "*"
        elif pt == ""
            print("MISSING CONST TYPE {ct} of {s}\n")
            return ""
        else
            return "{pt}?"
    return _glTypeToDasType(s)

def _glTypeToDasType ( s : string )
    if types |> key_exists(s)
        return types?[s] ?? ""
    if unsupported |> key_exists(s)
        return ""
    print("UNMATCHED ARGUMENT TYPE: {s}\n")
    return "*"

def genFunctionPrototypes(fnames:array<string>; gen_file:string)
    var reg_func_header <- %regex~GLAPI\s+(\w+\s*\w*\s*\*?)\s*APIENTRY\s*(\w+)%%
    var reg_func_args <- %regex~.+\((.*)\)%%
    fopen(gen_file,"wb") <| $(hf)
        if hf==null
            panic("can't write {gen_file}")
        fprint(hf,"require dasbind public\n\n")
        var maxargs = 0
        for fname in fnames
            fopen(fname,"rb") <| $( f )
                if f == null
                    panic("can't open {fname}")
                while !feof(f)
                    let data = fgets(f)
                    if !empty(data)
                        if data |> starts_with("GLAPI")
                            if regex_match(reg_func_header, data) != -1
                                let FUNC_TYPE = regex_group(reg_func_header,1,data)
                                let FUNC_NAME = regex_group(reg_func_header,2,data)
                                let das_func_type = glTypeToDasType(FUNC_TYPE)
                                if das_func_type==""
                                    print("skipping {FUNC_NAME} due to unsupported return address {FUNC_TYPE}\n")
                                    continue
                                var FUNC_ARGS : array<tuple<name:string;dastype:string>>
                                if regex_match(reg_func_args, data) != -1
                                    let ARGS = regex_group(reg_func_args,1,data)
                                    let args <- split(ARGS,",")
                                    maxargs = max(length(args),maxargs)
                                    for arg in args
                                        if arg=="void"
                                            pass
                                        else
                                            let llen = length(arg)
                                            var len = llen-1
                                            while len>=0
                                                var ch : int
                                                unsafe
                                                    ch = character_at(arg,len)
                                                if ch=='_' || is_alpha(ch) || is_number(ch)
                                                    len --
                                                else
                                                    len ++
                                                    break
                                            let ARG_TYPE = strip(slice(arg,0,len))
                                            var ARG_NAME = strip(slice(arg,len,llen))
                                            ARG_NAME = arg_names?[ARG_NAME] ?? ARG_NAME
                                            let atype = glTypeToDasType(ARG_TYPE)
                                            if atype==""
                                                print("skipping {FUNC_NAME} due to unsupported argument type {ARG_TYPE}\n")
                                                continue
                                            if atype=="*"
                                                print("{data}{args}\n{ARG_TYPE} | {ARG_NAME}\n")
                                                panic("data did not match\n")
                                            FUNC_ARGS |> push([[auto ARG_NAME, atype]])
                                else
                                    print("unmatched arguments\n{data}")
                                fprint(hf,"[extern(opengl,late,name=\"{FUNC_NAME}\",library=\"Opengl32\")]\n")
                                fprint(hf,"def {FUNC_NAME} ( ")
                                for arg,i in FUNC_ARGS,range(100500)
                                    if i!=0
                                        fprint(hf,"; ")
                                    fprint(hf,"{arg.name}:{arg.dastype}")
                                    if find(arg.dastype,"?") != -1
                                        fprint(hf," implicit")
                                fprint(hf," ) : {das_func_type}\n")
                                if das_func_type=="void"
                                    fprint(hf,"\tpass\n")
                                else
                                    fprint(hf,"\treturn [[{das_func_type}]]\n")
                                fprint(hf,"\n")
                            else
                                print("UNMATCHED: {data}")
        print("maximum {maxargs} arguments\n")

[export]
def main
    genDefineConstants([{string "OPENGL/glcorearb.h"}],"opengl_const.das")
    genFunctionPrototypes([{string "OPENGL/glcorearb.h"}],"opengl_func.das")


