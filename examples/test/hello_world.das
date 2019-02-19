options log = true // , logInferPasses=true // , optimize = false, printVarAccess=true, printRef=true //

let
	x = [[int 1; 
			2; 
			3; 4; 
			5; 6]]

[export]
def test
	print("x={x}\n")
    return true



