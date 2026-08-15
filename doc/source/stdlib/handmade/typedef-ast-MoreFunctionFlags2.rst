Overflow word for function flags (moreFlags is full at 32 bits).
Function is a @@{} local function body - generated, but the block is verbatim user code.
Result is always a fresh string allocation (or null), never a passthrough of an input, never retained by the callee.
Executing the body may hit a temp-string queue site (a builder or a wrappable call, transitively) - a caller must not hold a parked temp across a call to this function.
Function may reach a heap collect point (transitively over direct calls) - its caller keeps a real stack frame, so carriers are denied fastcall.
