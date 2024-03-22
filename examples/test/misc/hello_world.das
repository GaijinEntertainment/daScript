require UnitTest

[export]
def main
    var dummy = makeDummy()
	debug(dummy)
	var tempDummy = makeTempDummy()
	debug(tempDummy)

options log
