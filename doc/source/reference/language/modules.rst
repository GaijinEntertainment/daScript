.. _modules:

========================
Modules and C++ bindings
========================

Modules provide infrastructure for the code reuse,
as well as mechanism to expose C++ functionality to daScript.
A module is a collection of types, constants, and functions.
Modules can be native to daScript, as well as built-in.

To request a module, use ``require`` keyword::

    require math
    require ast public
    require daslib/ast_boost

``public`` modifier indicates that included model is visible to everything including current module.

Module name may contain ``/`` and ``.`` symbols.
Project is responsible for resolving module names into file names (see :ref:`Project <modules_project>`).

--------------
Native modules
--------------

Native module is separate daScript file, with an optional ``module`` name::

    module custom       // specifies module name
    ...
    def foo             // defines function in module
    ...

If not specified, module name is defaulted to that of a file name.

``[private]`` function annotation specifies that function will not be visible outside of module::

    [private]
    def implementation_detail(a:string) // local to current module
        ...

--------------------------
Module function visibility
--------------------------

When calling function, name of the module can be specified explicitly or implicitly::

    let s1 = sin(0.0)           // implicit, assumed math::sin
    let s2 = math::sin(0.0)     // explicit, always math::sin

If function does not exist in the module, compilation error will occur.
If function is private or not directly visible, compilation error will occur.
If multiple functions match implicit function, compilation error will occur.

Module names ``_`` and ``__`` are reserved to specify `current module` and `current module only` accordingly.
Its particularly important for generic functions, which are always instanced as private functions in the current module::

    module b

    [generic]
    def from_b_get_fun_4()
        return  _::fun_4()      //  call `fun_4', as if it was implicitly called from b

    [generic]
    def from_b_get_fun_5()
        return  __::fun_5()     // always b::fun_5

Specifying empty prefix is the same as specifying no prefix.

Without ``_`` or ``__`` module prefixes overwritten functions will not be visible from the generics.
That is why ``:=`` and ``delete`` operators are always replaced with ``_::clone`` or ``_::finalize`` calls.

---------------
Builtin modules
---------------

Builtin modules are the way to expose C++ functionality to daScript.

Lets look at ``FIO`` module as an example.
To create builtin module application needs to do the following.

Make a C++ file where module resides. Additionally make a header for AOT to include.

Derive from Module class and provide custom module name to the constructor::

    class Module_FIO : public Module {
    public:
        Module_FIO() : Module("fio") {              // this module name is ``fio``
            DAS_PROFILE_SECTION("Module_FIO");      // the profile section is there to profile module initialization time
            ModuleLibrary lib;                      // module needs library to register types and functions
            lib.addModule(this);                    // add current module to the library
            lib.addBuiltInModule();                 // make builtin functions visible to the library

Specify AOT type and provide prefix with C++ includes (see :ref:`AOT <aot>`)::

    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"daScript/simulate/aot_builtin_fio.h\"\n";
        return ModuleAotType::cpp;
    }

Register module at the bottom of C++ file using ``REGISTER_MODULE`` or ``REGISTER_MODULE_IN_NAMESPACE`` macro::

    REGISTER_MODULE_IN_NAMESPACE(Module_FIO,das);

Use ``NEED_MODULE`` macro during application initialization before daScript compiler is invoked::

    NEED_MODULE(Module_FIO);

Its possible to have additional daScript files to accompany builtin module,
and have them compiled at initialization time via ``compileBuiltinModule`` function::

    Module_FIO() : Module("fio") {
        ...
        // add builtin module
        compileBuiltinModule("fio.das",fio_das, sizeof(fio_das));

What happens here is that fio.das is embedded into the executable (via XXD utility) as a string constant.

Once everything is registered in the module class constructor,
its a good idea to very that module is ready for AOT via ``verifyAotReady`` function.
It's also good idea to verify that builtin names are following correct naming conventions
and do not collide with keywords via ``verifyBuiltinNames`` function::

    Module_FIO() : Module("fio") {
        ...
        // lets verify all names
        uint32_t verifyFlags = uint32_t(VerifyBuiltinFlags::verifyAll);
        verifyFlags &= ~VerifyBuiltinFlags::verifyHandleTypes;  // we skip annotatins due to FILE and FStat
        verifyBuiltinNames(verifyFlags);
        // and now its aot ready
        verifyAotReady();
    }

------------------------
Builtin module constants
------------------------

Constants can be exposed via ``addConstant`` function::

    addConstant(*this,"PI",(float)M_PI);

Constant type is automatically inferred, assuming type ``cast`` infrastructure is in place (see :ref:`cast <cast>`).

---------------------------
Builtin module enumerations
---------------------------

Enumerations can be exposed via ``addEnumeration`` function::

    addEnumeration(make_smart<EnumerationGooEnum>());
    addEnumeration(make_smart<EnumerationGooEnum98>());

For this to work enumeration adapter has to be defined via ``DAS_BASE_BIND_ENUM`` or ``DAS_BASE_BIND_ENUM_98`` C++ preprocessor macros::

    namespace Goo {
        enum class GooEnum {
            regular
        ,   hazardous
        };

        enum GooEnum98 {
            soft
        ,   hard
        };
    }

    DAS_BASE_BIND_ENUM(Goo::GooEnum, GooEnum, regular, hazardous)
    DAS_BASE_BIND_ENUM_98(Goo::GooEnum98, GooEnum98, soft, hard)

-------------------------
Builtin module data types
-------------------------

Custom data types and type annotations can be exposed via ``addAnnotation`` or ``addStructure`` functions::

    addAnnotation(make_smart<FileAnnotation>(lib));

See :ref:`handles <handles>` for more details.

-------------------------
Builtin module macros
-------------------------

Custom macros of different type can be added via ``addAnnotation``, ``addTypeInfoMacro``, ``addReaderMacro``, ``addCallMacro`` and such.
It is however strongly preferred to implement macros in daScript.

See :ref:`macros <macros>` for more details.

------------------------
Builtin module functions
------------------------

Functions can be exposed to the builtin module via ``addExtern`` and ``addInterop`` routines.

~~~~~~~~~
addExtern
~~~~~~~~~

``addExtern`` exposes standard C++ functions, which are not specifically designed around daScript interop::

    addExtern<DAS_BIND_FUN(builtin_fprint)>(*this, lib, "fprint", SideEffects::modifyExternal, "builtin_fprint");

Here the builtin_fprint function is exposed to daScript and given the name `fprint`.
AOT name for the function is explicitly specified, to indicate that the function is AOT ready.

Side-effects of the function need to be explicitly specified (see :ref:`Side-effects <modules_function_sideeffects>`).
It's always safe but inefficient to specify ``SideEffects::worstDefault``.

Lets look at the exposed function in detail::

    void builtin_fprint ( const FILE * f, const char * text, Context * context ) {
        if ( !f ) context->throw_error("can't fprint NULL");
        if ( text ) fputs(text,(FILE *)f);
    }

C++ code can explicitly request to be provided with daScript context, by adding `Context` type argument.
Making it last argument of the function makes context substitution transparent for daScript code,
i.e. it can simply call::

    fprint(f, "boo")    // current context with be provided transparently

daScript strings are very similar to C++ ``char *``, however null also indicates empty string.
Thats why in the example above the `fputs` only occurs if text is not null.

Lets look at another integration example from the builtin `math` module::

    addExtern<DAS_BIND_FUN(float4x4_translation), SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "translation",
            SideEffects::none, "float_4x4_translation")->arg("xyz");

Here float4x4_translation function returns ref type by value, i.e. `float4x4`.
This needs to be indicated explicitly by specifying templated SimNode argument for the ``addExtern`` function,
which is ``SimNode_ExtFuncCallAndCopyOrMove``.

Some functions need to return ref type by reference::

    addExtern<DAS_BIND_FUN(fooPtr2Ref),SimNode_ExtFuncCallRef>(*this, lib, "fooPtr2Ref",
        SideEffects::none, "fooPtr2Ref");

This is indicated with ``SimNode_ExtFuncCallRef`` argument.

~~~~~~~~~~
addInterop
~~~~~~~~~~

For some functions it may be necessary to access type information and well as non-marshalled data.
Interop functions are designed specifically for that purpose.

Interop functions are of the following pattern::

    vec4f your_function_name_here ( Context & context, SimNode_CallBase * call, vec4f * args )

They receive context, calling node, and arguments.
They are expected to marshal and return results, or v_zero()

``addInterop`` exposes C++ functions, which are specifically designed around daScript::

    addInterop<
        builtin_read,               // function to register
        int,                        // function return type
        const FILE*,vec4f,int32_t   // function arguments in order
    >(*this, lib, "_builtin_read",SideEffects::modifyExternal, "builtin_read");

Interop function registration template expects function name as a first template argument,
function return value as a second, with the rest of the arguments to follow.

When function argument type needs to remain unspecified, argument type of ``vec4f`` is used.

Lets look at the exposed function in detail::

    vec4f builtin_read ( Context & context, SimNode_CallBase * call, vec4f * args ) {
        DAS_ASSERT ( call->types[1]->isRef() || call->types[1]->isRefType() || call->types[1]->type==Type::tString);
        auto fp = cast<FILE *>::to(args[0]);
        if ( !fp ) context.throw_error("can't read NULL");
        auto buf = cast<void *>::to(args[1]);
        auto len = cast<int32_t>::to(args[2]);
        int32_t res = (int32_t) fread(buf,1,len,fp);
        return cast<int32_t>::from(res);
    }

Argument types can be accessed via call->types array.
Argument values and return value are marshalled via ``cast`` infrastructure (see :ref:`cast <cast>`).

.. _modules_function_sideeffects:

---------------------
Function side-effects
---------------------

daScript compiler is very much an optimizer compiler and pays a lot of attention to function side-effects.

On the C++ side ``enum class SideEffects`` contains possible side effect combinations.

``none`` indicates that function is pure, i.e it has no side-effects whatsoever.
Good example would be purely computational functions like ``cos`` or ``strlen``.
daScript may choose to fold those functions at compilation time
as well as completely remove them in case where result is not used.

Trying to register void function with no arguments and no side-effects will cause module initialization to fail.

``unsafe`` indicates that function has unsafe side-effects, which can cause panic or crash.

``userScenario`` indicates that some other uncategorized side-effects are in works.
daScript will not optimize or fold those functions.

``modifyExternal`` indicates that function modifies state, external to daScript;
typically its some sort of C++ state.

``accessExternal`` indicates that function reads state, external to daScript.

``modifyArgument`` means function modifies one of its input parameters.
daScript will look into non-constant ref arguments and will assume that they may be modified during the function call.

Trying to register function without mutable ref arguments and ``modifyArgument`` side effects will cause module initialization to fail.

``accessGlobal`` indicates that function would access global state, i.e. global daScript variables or constants.

``invoke`` indicates that function may invoke another functions, lambda, or block.

.. _modules_file_access:

-----------
File access
-----------

daScript provides machinery to specify custom file access and module name resolution.

Default file access is implemented with ``FsFileAccess`` class.

File access needs to implement the following file and name resolution routines::

    virtual das::FileInfo * getNewFileInfo(const das::string & fileName) override;
    virtual ModuleInfo getModuleInfo ( const string & req, const string & from ) const override;

``getNewFileInfo`` provides file name to file data machinery. It returns null if file is not found.

``getModuleInfo`` provides module name to file name resolution machinery.
Given require string `req` and module it was called `from` it needs to fully resolve module::

    struct ModuleInfo {
        string  moduleName;     // name of the module (by default tail of req)
        string  fileName;       // file name, where the module is to be found
        string  importName;     // import name, i.e. module namespace (by default same as module name)
    };

It is better to implement module resolution in daScript itself, via project.

.. _modules_project:

-------
Project
-------

Project needs to export ``module_get`` function, which essentially implements default C++ ``getModuleInfo`` routine::

    require strings
    require daslib/strings_boost

    typedef
        module_info = tuple<string;string;string> const // mirror of C++ ModuleInfo

    [export]
    def module_get(req,from:string) : module_info
        let rs <- split_by_chars(req,"./")                  // split request
        var fr <- split_by_chars(from,"/")
        let mod_name = rs[length(rs)-1]
        if length(fr)==0                                    // relative to local
            return [[auto mod_name, req + ".das", ""]]
        elif length(fr)==1 && fr[0]=="daslib"               // process `daslib` prefix
            return [[auto mod_name, "{get_das_root()}/daslib/{req}.das", ""]]
        else
            pop(fr)
            for se in rs
                push(fr,se)
            let path_name = join(fr,"/") + ".das"           // treat as local path
            return [[auto mod_name, path_name, ""]]

The implementation above splits the require string, and looks for recognized prefixes.
If module is requested from another module, parent module prefixes are used.
If root `daslib` prefix is recognized, modules are looked for from ``get_das_root`` path.
Otherwise request is treated as local path.



