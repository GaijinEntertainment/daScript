require cbind/cbind_boost
require daslib/fio
require daslib/defer
require daslib/strings_boost

class ClangGen : CppGenBind
    def ClangGen
        let pfn = "clang-c/Index.h"
        let pfp = "{get_das_root()}/../libclang/include/"
        let args <- [{string
            "-xc++-header";
            "-std=c++1z";
            "-I{get_full_file_name(pfp)}";
            "-DWIN32";
            "-DCINDEX_EXPORTS"
        }]
        print("{args}\n")
        init_args(pfn,pfp,args)
        set_default_files()
        init_enum_prefix()
    def init_enum_prefix
        enum_prefix <- {{
            "CXAvailabilityKind" => "CXAvailability";
            "CXDiagnosticSeverity" => "CXDiagnostic";
            "CXLoadDiag_Error" => "CXLoadDiag";
            "CXDiagnosticDisplayOptions" => "CXDiagnostic_Display";
            "CXTypeKind" => "CXType";
            "CXCursorKind" => "CXCursor";
            "CXChildVisitResult" => "CXChildVisit"
        }}
    def override isArgByValue ( name:string )
        return true
    def override skip_file ( fname:string ) : bool
        if fname |> ends_with("CXErrorCode.h")
            return false
        elif fname |> ends_with("CXString.h")
            return false
        return ! fname |> ends_with(PARSE_FILE_NAME)


[export]
def main
    let clangbind_root = "{get_das_root()}/Modules/dasClangBind/src"
    // enum files
    var edi = fopen("{clangbind_root}/dasClangBind.enum.decl.inc","wb")
    if edi==null
        panic("can't open enum decl\n")
    defer <|
        fclose(edi)
    var eci = fopen("{clangbind_root}/dasClangBind.enum.class.inc","wb")
    if eci==null
        panic("can't open enum class\n")
    defer <|
        fclose(eci)
    var eai = fopen("{clangbind_root}/dasClangBind.enum.add.inc","wb")
    if eai==null
        panic("can't open enum add\n")
    defer <|
        fclose(eai)
    // structure files
    var sdi = fopen("{clangbind_root}/dasClangBind.struct.decl.inc","wb")
    if sdi==null
        panic("can't open struct decl\n")
    defer <|
        fclose(sdi)
    var sii = fopen("{clangbind_root}/dasClangBind.struct.impl.inc","wb")
    if sii==null
        panic("can't open struct impl\n")
    defer <|
        fclose(sii)
    var sci = fopen("{clangbind_root}/dasClangBind.struct.class.inc","wb")
    if sci==null
        panic("can't open struct class\n")
    defer <|
        fclose(sci)
    var sai = fopen("{clangbind_root}/dasClangBind.struct.add.inc","wb")
    if sai==null
        panic("can't open struct add\n")
    defer <|
        fclose(sai)
    var spai = fopen("{clangbind_root}/dasClangBind.struct.postadd.inc","wb")
    if spai==null
        panic("can't open struct add\n")
    defer <|
        fclose(spai)
    // alias files
    var aai = fopen("{clangbind_root}/dasClangBind.alias.add.inc","wb")
    if aai==null
        panic("can't open alias add\n")
    defer <|
        fclose(aai)
    var dai = fopen("{clangbind_root}/dasClangBind.dummy.add.inc","wb")
    if dai==null
        panic("can't open dummy add\n")
    defer <|
        fclose(dai)
    // function
    var fai = fopen("{clangbind_root}/dasClangBind.func.add.inc","wb")
    if fai==null
        panic("can't open dummy func add\n")
    defer <|
        fclose(fai)
    // generate
    var cgb = new ClangGen()
    defer <|
        unsafe
            delete cgb
    cgb->setEnumFiles ( edi,eci,eai )
    cgb->setStructFiles ( sdi,sii,sci,sai,spai )
    cgb->setAliasFiles ( aai,dai )
    cgb->setFuncFiles ( fai )
    cgb->generate()
