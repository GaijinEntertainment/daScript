require fio

let MAX_ARGUMENTS = 16

[export]
def main
    fopen("{get_das_root()}/src/builtin/x86_64_wrapper.inc","wb") <| $ ( fout )
        for nargs in range(MAX_ARGUMENTS)
            for res in range(2)
                for perm in range(16)
                    let call_result = res!=0 ? "vec4f" : "int64_t"
                    let args_prefix = nargs!=0 ? "args" : ""
                    fprint(fout,"vec4f fastcall64_{nargs}_{res}_{perm} ( void * fn, vec4f * {args_prefix} ) \{\n")
                    fprint(fout,"    using call_kind = {call_result} ( * ) ( ")
                    for arg in range(nargs)
                        if arg!=0
                            fprint(fout,",")
                        if arg>=4
                            fprint(fout,"int64_t")
                        else
                            if (perm & (1<<arg))!=0
                                fprint(fout,"double")
                            else
                                fprint(fout,"int64_t")
                    fprint(fout," );\n")
                    fprint(fout,"    return ")
                    if res==0
                        fprint(fout,"Rx ( ")
                    fprint(fout,"call_kind(fn) ( ")
                    for arg in range(nargs)
                        if arg!=0
                            fprint(fout,",")
                        if arg>=4
                            fprint(fout,"AX({arg})")
                        else
                            if (perm & (1<<arg))!=0
                                fprint(fout,"AD({arg})")
                            else
                                fprint(fout,"AX({arg})")
                    if res==0
                        fprint(fout," )")
                    fprint(fout," );\n")
                    fprint(fout,"\}\n\n")
        fprint(fout, "#define MAX_WRAPPER_ARGUMENTS  {MAX_ARGUMENTS}\n\n")
        fprint(fout, "FastCallWrapper fastcall64_table [] = \{\n")
        for nargs in range(MAX_ARGUMENTS)
            for res in range(2)
                fprint(fout,"    ")
                for perm in range(16)
                    fprint(fout,"&fastcall64_{nargs}_{res}_{perm},")
                fprint(fout,"\n")
        fprint(fout, "};\n\n")


