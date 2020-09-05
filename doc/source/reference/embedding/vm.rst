.. _embedding_vm:

========================
daScript Virtual Machine
========================

-----------------------
Tree-based Interpretion
-----------------------

Virtual Machine of daScript consists of a small execution context, which manages stack and heap allocation.
The compiled program itself is a Tree of "Nodes" (called SimNode) which can evaluate virtual function. These Nodes don't check argument types, assuming that all such checks were done by compiler.
Why tree-based? Tree-based interpreters are slow!

Yes, Tree-based interpreters are infamous for their low performance compared to byte-code interpreters.

However, this opinion is typically based on a comparison between AST (abstract syntax tree) interpreter of dynamic-typed language with optimized register or stack based optimized interpreter. 
Due to simplicity in implementation AST-based interpreters are also seen in a lot of "home-brewed" naive interpreters getting tree-based interpreters additional bad fame. 
AST usually contains a lot of useless or unnecessary for interpretation data, and big tree depth and complexity. 

It is also hard to even make AST interpreter of statically typed language which will somehow benefit from static types data - basically each tree node visitor will still return both value and type information in generic form.

In comparison, good byte-code VM interpreter of typical dynamically typed language will feature tight core loop of all or most frequent instructions (probably with computed goto) and additionally can statically (or during execution) infer types and optimize code for it.

**Register**- and **stack-based**- VM will each have their own trade-offs, notably generally fewer generated instructions/fused instructions and less memory moves/indirect memory access for register-based vs. smaller instructions set and easier implementation for stack-based VMs.

The more "core types" the VM have the more instructions will probably be needed in the instruction set and/or instruction cost increases.
Although dynamically typed languages usually don't have many core types, and some can embed all their main types values and type information in just 64bits (using NAN-tagging, for example) - that still usually leaves one of these core types (table/class/object) to be worked with using associative containers lookups (unordered_map/hashmap).
That is not optimal for cache locality/performance, and also makes interop with host (C++) types slow and inefficient. 

Interop with host/C++ functions because of that is usually slow, as it requires complex and slow arguments/return value type conversion, and/or associative table(s) lookup.

So, typically, host functions calls are very "heavy", and programmers usually can't optimize scripts by extracting just some of functionality into C++ function - the have to re-write big chunk/loop.

Increasing amount of core internal types can help (for example - making typical in game development "float3" one of "core" types), but makes instruction set bigger/slower, increases complexity of type conversion, and usually introduce mandatory indirection (due to limited bitsize of value type), which also not good for cache locality. 

However, **daScript** *does not* interpret AST, neither it is dynamically typed language. 

Instead, for run-time program execution it emits different tree (Simulation Tree), which doesn't require type information to be provided for arguments/return type, since it is statically typed, and all types are known.

For daScript ABI 128bit words are used, which are natural to most of modern hardware. 

Chosen representation helps branch prediction, increases cache locality and provides mix of stack and register based code - each 'Node' is utilizing native machine registers to perform. 

Which is also important, amount of "types" and "instructions" doesn't matter much - what matters is amount of different instructions used in a particular program/function.

Type conversion between VM and C++ ABIs is straightforward (and for most of types is as simple as move instruction), so it is very fast and cache-friendly. 

It also makes possible for programmer to optimize particular functionality (in interpretation) by extracting it to C++/host function - basically "fuse" instructions into one. 

Adding new user-types is rather simple and non-painful performance or engineering wise. 

"Value" types have to fit into 128bits and have to be relocatable and zero-initialized (i.e. should be trivially destructible, and allows memcpy and memsetting with zeroes); all other types are "RefTypes" or "Boxed Types", which means they can be operated in script only as references/pointers.

There are no limits on amount of user types, neither there is performance impact caused by using such types (besides obvious cost of indirection for Boxed/Ref Types).

Using generalized node additionally allows seamless mix of interpretation and Ahead of Time compiled run-time - i.e. if some of the functions in script were changed, the unchanged portion will still be running optimized AoT code.

These are the main reasons, why tree-based interpretation (not to be confused with AST-based) was chosen for daScript interpreter, and why its interpreter is faster than most, if not all, byte code based scripts interpreters.



-----------------
Execution Context
-----------------

daScript Execution Context is light-weight. It basically consists of stack allocation and two heap allocators (strings and everything else).
One Context can be used to execute different programs, however, if the program has any global state in a heap, all calls to the program has to be done with a same Context.

It is possible to call stop-the-world garbage collection on a Context (this call better to be done outside the program execution, it's unsafe otherwise).

However, the cost of re-setting context (i.e. deallocate all memory) is extremely low, and (depending on memory usage) can be as low as several instructions,

which allows the simplest and fastest form of memory management for all of the stateless scripts - just reset the context each frame or each call.
This basically turns Context heap management into form of 'bump/stack allocator', significantly simplifying and optimizing memory management.

There are certain ways (including code of policies) to ensure that none of the scripts are not using global variables at all, or at least global variables which require heap memory.

For example, one can split all contexts into several: one context for all stateless script programs, and then one context for each GC'ed (or, additionally, ``unsafe``) script.
The stateless context then will be resetted as often as needed (for example, each 'top' call from C++ or each frame/tick), and on GC-ed contexts one can call garbage collection as soon as needed (using some heurestics of memory usage/performance).

Each context can be used only in one thread simultaneously, i.e. for multi-threading you will need one Context for each simultaneously running thread.

To make data exchange/comminication between different contexts, use 'channels' or some other custom brewed C++ hosted code of that kind.