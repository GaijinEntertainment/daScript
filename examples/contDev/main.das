// options logNodes=true, logStack=true

require fio
require rtti

[unsafe]
def findArg(field,argName,argType)  : AnnotationArgument const? const
	if field.annotation_arguments != null
		for arg in deref(field.annotation_arguments)
			if string(arg.name)==argName
				if arg.basicType==argType
					return addr(arg)
				else
					print("error: {field.name} {argName} argument must be {argType}\n")
	return null


def cppTypeAttr(field) : string
	let arg = findArg(field,"cppType",Type tString)
	return arg!=null ? string(arg.sValue) : ""

def cppBaseTypeName(bt)
	if bt==Type tInt
		return "int32_t"
	elif bt==Type tInt2
		return "int2"
	elif bt==Type tInt3
		return "int3"
	elif bt==Type tInt4
		return "int4"
	elif bt==Type tUInt
		return "uint32_t"
	elif bt==Type tFloat
		return "float"
	elif bt==Type tFloat2
		return "float2"
	elif bt==Type tFloat3
		return "float3"
	elif bt==Type tFloat4
		return "float4"
	elif bt==Type tString
		return "string"
	else
		return "/*undefined*/"

def cppTypeDim(info)
	var res = ""
	if info.dimSize !=0u
		for index in range(0,int(info.dimSize))
			let d = getDim(info,index)
  			res += "[{d}]"
	return res

def cppTypeName(info)
	var res = cppTypeAttr(info)
	if res == ""
		res = info.isConst ? "const " : ""
		if info.basicType==Type tHandle
			res += string(info.annotation_or_name.name)
		elif info.basicType==Type tStructure
			res += info.structType.name
		else
			res += cppBaseTypeName(info.basicType)
	return res

def generateSquirrelBinding(file,info)
	fprint(file,"\{\n")
    fprint(file,"\tSqrat::DerivedClass<{info.name}, ecs::Event> sqEvent(vm, \"{info.name}\");\n")
    fprint(file,"\tsqEvent\n")
    fprint(file,"\t\t.Ctor<Sqrat::Object>()\n")
    fprint(file,"\t\t.Ctor<Sqrat::Object, Sqrat::Object>()\n")
	for field in info
		fprint(file,"\t\t.Var(\"{field.name}\", &{info.name}::{field.name})\n")
    // fprint(file,"\t\t.Var(\"data\", &ecs::SQEvent::data)\n")
    fprint(file,"\t;\n")
    fprint(file,"\teventsBinding.emplace_back(\"{info.name}\", Sqrat::Object(sqEvent.GetObject(), vm));\n")
	fprint(file,"}\n\n")


def generateCppEvent(file,info)
	fprint(file,"class {info.name} : public Event \{\n")
	fprint(file,"public:\n")
	fprint(file,"\t{info.name}(")
	var first = true
	for field in info
		if first
			first = false
		else
			fprint(file,", ")
		fprint(file,"{cppTypeName(field)} _{field.name}");
	fprint(file,")\n\t\t: Event(ECS_HASH(\"{info.name}\")),\n")
	first = true
	for field in info
		if first
			fprint(file,"\t\t\t")
			first = false
		else
			fprint(file,",")
		fprint(file,"{field.name}(_{field.name})");
	fprint(file," \{\};\n");
	for field in info
		fprint(file,"\t{cppTypeName(field)} {field.name}{cppTypeDim(field)};\n")
	fprint(file,"\ttemplate<int n, class T> const T& get();\n");
	var count = 0
	for field in info
		fprint(file,"\ttemplate<> {cppTypeName(field)}{cppTypeDim(field)} & get<{count++}>() \{ return {field.name}; \};\n");
	fprint(file,"\};\n\n")

def process(fname:string)
	print("processing {fname}\n")
    fopen(fname,"rb") <| $(f)
        if f!=null
            let text = fread(f)
            rttiCompile(fname,text) <| $(ok,program,issues)
                if ok
                    rttiProgramForEachStructure(program) <| $(info)
						generateCppEvent(fstdout(),info)
						generateSquirrelBinding(fstdout(),info)
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

