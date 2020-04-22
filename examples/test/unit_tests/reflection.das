require rtti

// note: this is var because we want "sample" global variable to show up
var
	sample = "
require math
require UnitTest

let
	one_two = 12

enum TestEnum
	foo
	bar

[checkRange (foo=5)]
struct TestStruct
	a : int = 5
	b : string = \"something\"
	c : float[3][4]
	d : array<int>
	e : table<float; string>
	f : TestStruct?
	g : tuple<int; float; string>
	h : variant<i : int; f : float>

[export]
def main
	print(\"hello world!\n\")
	return true
"

def describeType(info)
	var res : string 
	if info.basicType==Type tHandle
		res = string(info.annotation_or_name.name)
	elif info.basicType==Type tStructure
		res = info.structType?.name ?? "structure"
	elif info.basicType==Type tEnumeration | info.basicType==Type tEnumeration8 | info.basicType==Type tEnumeration16
		res = info.enumType?.name ?? "enumeration"
	elif info.basicType==Type tPointer
		if info.firstType!=null
			res = "{describeType(*(info.firstType))}?"
		else
			res = "void?"
	elif info.basicType==Type tArray
		if info.firstType!=null
			res = "array<{describeType(*(info.firstType))}>"
		else
			res = "array"
	elif info.basicType==Type tTable
		if info.firstType!=null & info.secondType!=null
			res = "table<{describeType(deref(info.firstType))};{describeType(deref(info.secondType))}>"
		else
			res = "table"
	elif info.basicType==Type tTuple
		if info.argTypes!=null
			res = "tuple<"
			var first = true
			for argtype in arg_types(info)
				if first
					first = false
				else
					res += ";"
				res += "{describeType(argtype)}"
			res += ">"
		else
			res += "tuple"
	elif info.basicType==Type tVariant
		if info.argTypes!=null & info.argNames!=null
			res = "variant<"
			var first = true
			for argname,argtype in arg_names(info),arg_types(info)
				if first
					first = false
				else
					res += ";"
				res += "{argname}:{describeType(argtype)}"
			res += ">"
		else
			res += "variant"
	else
		res = "{get_das_type_name(info.basicType)}"	// TODO: FIXME. this is crashing without "{}"
	if info.dimSize !=0u
		for index in range(0,int(info.dimSize))
			let d = get_dim(info,index)
			res += d!=-1 ? "[{d}]" : "[]"
	if info.isConst
		res += " const"
	if info.isRef
		res += " &"
	return res

def describeFunction(finfo)
	print("def {finfo.name} ")
	if finfo.count != 0u
		print("( ")
		var first = true
		for arg in finfo
			if first
				first = false
			else
				print("; ")
			print("{arg.name}:{describeType(arg)}")
		print(" ) ")
	if finfo.result != null
		print ( ": {describeType(*(finfo.result))}")
	print("\n")

def describeVariable(glob)
	print("{glob.name} : {describeType(glob)}")
	let value = get_variable_value(glob)
	if value is nothing
		pass
	elif value is tString
		print(" = \"{value}\"")
	else
		print(" = {value}")
	print("\n")

def describeStructure(sinfo)
	var anyAnn = false
	structure_for_each_annotation(sinfo) <| $(ann; annArgs)
		anyAnn = true
		print("[{ann.name}")
		var first = true
		for arg in annArgs
			if first
				print("(")
				first = false
			else
				print(",")
			let value = get_annotation_argument_value(arg)
			print("{arg.name}")
			if value is nothing
				pass
			elif value is tString
				print("=\"{value}\"")
			else
				print("={value}")
		if !first
			print(")")
		print("]")
	if anyAnn
		print("\n")
	print("struct {sinfo.name}\n")
	for sfield in sinfo
		print("    ")
		describeVariable(sfield)

def describeEnumeration(einfo)
	print("enum {einfo.name}\n")
	for eitem in einfo
		print("    {eitem.name} = {eitem.value}\n")

def describeModule(mod)
	var name = string(mod.name)
	if name=="$"
		print("// modulde BUILTIN\n")
	elif name==""
		print("// modulde THIS\n")
	else
		print("// modulde {name}\n")
	module_for_each_enumeration(mod) <| $ ( einfo )
		describeEnumeration(einfo)
	module_for_each_structure(mod) <| $ ( sinfo )
		describeStructure(sinfo)
	module_for_each_function(mod) <| $ ( finfo )
		describeFunction(finfo)
	module_for_each_generic(mod) <| $ ( finfo )
		describeFunction(finfo)
	module_for_each_global(mod) <| $ ( glob )
		describeVariable(glob)
	
[export]
def main
	// global modules
	program_for_each_registered_module() <| $ ( mod )
		describeModule(mod)
	describeModule(get_module("$"))
	// sampel program
	compile("sample",sample) <| $ ( ok; prog; errors )
		if ok
			print("compiled\n")
			let thisMod = get_this_module(prog)
			program_for_each_module(prog) <| $ ( mod )
				if mod==thisMod
					describeModule(mod)
		else
			print("failed to compile:\n{errors}\n")
			assert(false)
	// current context
	for fni in range(0,get_total_functions())
		let finfo & = get_function_info(fni)
		describeFunction(finfo)
	for vi in range(0,get_total_variables())
		let vinfo & = get_variable_info(vi)
		describeVariable(vinfo)
	return true

[export]
def test
	return true
