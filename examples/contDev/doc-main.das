// options logNodes=true, logStack=true
// options logCpp=true, onlyFastAot=true

require fio
require rtti


def getModuleDoc(modName:string)
	let pMod = rttiGetModule(modName)
	if pMod==null
		print("module {modName} not found\n")
		return
	rttiModuleForEachStructure(pMod) <| $(info)
		print("struct {info.name}\n")
	rttiModuleForEachFunction(pMod) <| $(info)
		print("def {info.name}")
		if (info.flags & FUNCINFO_BUILTIN) == FUNCINFO_BUILTIN
			print(" // {info.cppName=="" ? "WARNING: no CPP name" : info.cppName}")
		print("\n")
	rttiModuleForEachGeneric(pMod) <| $(info)
		print("def {info.name} // generic\n")

[export]
def main(fn:string)
    print("module documentatoin generator\n")
	// getModuleDoc("$")	// builtin
	getModuleDoc("fio")
	print("ok\n")
	let main_time = stat(fn).mtime
	while stat(fn).mtime == main_time
		sleep(1000u)
	return true

