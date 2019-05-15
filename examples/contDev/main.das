// options logNodes=true, logStack=true

require fio
require rtti

/*
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

*/

/*
tupleCompatibility << "  template<> const "<< cppType << " &" << "get<" << field << ">() {return "<<fl.name<<";}\n";
                    string flArgName = fl.name +"_";
                    constructorArgs << "    " << (field!=0 ? "," : " ") << " const " << cppType << "& " << flArgName << "\n";
                    constructorBody << "    " << (field!=0 ? "," : " ") << fl.name << "(" << flArgName << ")\n";
*/

def cppBaseTypeName(bt)
	if bt==Type tFloat3
		return "float3"
	else
		return "/*undefined*/"

def cppTypeName(info)
//	debug(info,"info=\n")
	var res = cppBaseTypeName(info.basicType)
/*
	if info.dimSize !=0 
		for index in range(0,info.dimSize)
			let d = rtti_getDim(info,index)
			res += "[{d}]"
*/
	return res

def processStructInfo(info)
	print("class {info.name} : public Event \{\n")
	print("public:\n")
	print("\t{info.name}(")
	var first = true
	for field in info
		if first
			first = false
		else
			print(", ")
		print("{cppTypeName(field)} _{field.name}");
	print(") : Event(ECS_HASH(\"{info.name}\")),\n")
	first = true
	for field in info
		if first
			print("\t\t")
			first = false
		else
			print(",")
		print("{field.name}(_{field.name})");
	print(" \{\};\n");
	for field in info
		print("\t{cppTypeName(field)} {field.name};\n")
	print("\ttemplate<int n, class T> const T& get();\n");
	var count = 0
	for field in info
		print("\ttemplate<> {cppTypeName(field)} & get<{count++}>() \{ return {field.name}; \};\n");
	print("\};\n")

def process(fname:string)
	print("processing {fname}\n")
    fopen(fname,"rb") <| $(f)
        if f!=null
            let text = fread(f)
            rttiCompile(fname,text) <| $(ok,program,issues)
                if ok
                    rttiProgramForEachStructure(program) <| $(info)
						processStructInfo(info)
                else
                    print("{fname} failed to compile\n{issues}\n")
        else
            print("can't open {fname}\n")


[export]
def main(fn:string)
    let atdir = "./src"
    print("{fn}\ndas network message generator at {atdir}\n")
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

