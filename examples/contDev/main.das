// options logNodes=true, logStack=true

require fio
require rtti

def process(fname:string)
	print("processing {fname}\n")
    fopen(fname,"rb") <| $(f)
        if f!=null
            let text = fread(f)
            rttiCompile(fname,text) <| $(ok,program,issues)
                if ok
                    rttiProgramForEachStructure(program) <| $(info)
                        print("struct {info.name}\n")
                        for field in info
                            print("\t{field.name} : {field.basicType} ")
                            if field.annotation_arguments != null
                                print("[")
                                var first = true
                                for arg in deref(field.annotation_arguments)
                                    if first
                                        first = false
                                    else
                                        print(",")
                                    print("{arg.name}=")
                                    if arg.basicType==Type tInt
                                        print("{arg.iValue}")
                                    elif arg.basicType==Type tFloat
                                        print("{arg.fValue}")
                                    elif arg.basicType==Type tString
                                        print("\"{arg.sValue}\"")
                                    elif arg.basicType==Type tBool
                                        print("{arg.bValue}")
                                print("] ")
                            print("\n")
                else
                    print("{fname} failed to compile\n{issues}\n")
        else
            print("can't open {fname}\n")


[export]
def main(fn:string)
    let atdir = dirname(fn) + "/src"
    print("das network message generator at {atdir}\n")
	let main_time = stat(fn).mtime
    var ttab : table<string,clock>
	while stat(fn).mtime == main_time
        dir(atdir) <| $ (fname)
            let fileName = atdir + "/" + fname
            var st : FStat
            if stat(fileName,st)
                if st.is_reg
                    let fmt = st.mtime
                    let tmt = ttab[fname]
                    if (tmt!=fmt)
                        ttab[fname] = fmt
                        process(fileName)
            else
                print("can't stat {fileName}\n")
		sleep(1000u)
	print("ok\n")
	return true

