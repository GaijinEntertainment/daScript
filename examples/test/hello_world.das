[export]
def test
	let args <- get_command_line_arguments()
	debug(args,"\nCommand line arguments = \n")
	return true
