options log = true

require rtti


[export]
def test()
	rtti_builtin_program_for_each_registered_module() <| $ ( mod )
		rtti_builtin_module_for_each_global(mod) <| $ ( glob )
			if glob.basicType == Type tInt
				print("{glob.name} = {glob.iValue}\n")
			elif glob.basicType == Type tUInt
				print("{glob.name} = {glob.uValue}\n")
			elif glob.basicType == Type tInt64
				print("{glob.name} = {glob.i64Value}\n")
			elif glob.basicType == Type tUInt64
				print("{glob.name} = {glob.u64Value}\n")
			elif glob.basicType == Type tFloat
				print("{glob.name} = {glob.fValue}\n")
			elif glob.basicType == Type tDouble
				print("{glob.name} = {glob.dfValue}\n")
			elif glob.basicType == Type tBool
				print("{glob.name} = {glob.bValue}\n")
			elif glob.basicType == Type tString
				print("{glob.name} = {glob.sValue}\n")
			else
				print("glob = {glob}\n")
	return true
