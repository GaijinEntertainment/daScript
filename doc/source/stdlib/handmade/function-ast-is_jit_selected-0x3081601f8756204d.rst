True when ``program``'s JIT selected ``function`` - the DLL the program runs holds it, so a
call to it from jitted code goes into the DLL rather than through an interpreter slot. The
selection is the program's, not the function's: a shared module's function is selected by
one program and not by another in the same process. ``[jit]`` on the function is the
author's request (``func.moreFlags.requestJit``), which the plan reads and the selection
never writes. A null ``program`` throws.
