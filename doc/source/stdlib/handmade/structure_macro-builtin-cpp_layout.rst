Specifies that structure uses C++ memory layout rules, as oppose to native Daslang memory layout rules::

    [cpp_layout (pod=false)]        // pod=false means that the structure is not plain old data (and different padding rules apply)
    struct CppS1
        vtable : void?              // we are simulating C++ class
        b : int64 = 2l
        c : int = 3

    [cpp_layout (pod=false)]
    struct CppS2 : CppS1            // d will be aligned on the class bounds
        d : int = 4

