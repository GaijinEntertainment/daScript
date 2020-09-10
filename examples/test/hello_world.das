options debugger = true

require ast

options log = true

[export]
def foo ( ctx : __context )
	print("ctx = {ctx}\n")


[export]
def test
	foo()
	var x = 5
	// breakpoint()
	x = 6
	for t in range(0,10)
		print("x={x} t={t}\n")
		x ++
		x += 3
	return true

/*
        TextPrinter tp;
        if ( at.fileInfo ) {
            tp << at.fileInfo->name << ":" << at.line << ":" << at.column << "\n";
            auto textAroundBreak = getLinesAroundCode(
                at.fileInfo->getSource(),
                at.line,
                at.fileInfo->tabSize);
            tp << textAroundBreak;
        }
        tp << getStackWalk(&at, true, true, false, true);
        for ( ;; ) {
            tp << "C - Continue, SPACE - Step\n";
            int ch = _getch();
            switch ( ch ) {
                case ' ':
                    singleStepMode = true;
                    singleStepAt = &at;
                    goto done;
                case 'c':
                case 'C':
                    singleStepMode = false;
                    singleStepAt = nullptr;
                    goto done;
                default:  ; // beep
            }
        }
        done:;
*/
