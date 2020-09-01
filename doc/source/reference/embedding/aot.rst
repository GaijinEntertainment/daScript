.. _aot:

====================================================
Ahead of time compilation and C++ operation bindings
====================================================

For optimal performance and seamless integration daScript is capable of ahead of time compilation,
i.e. producing C++ files, which are semantically equivalent to simulated daScript nodes.

Output C++ is designed to be to some extent human readable.

For the most part daScript will produce AOT automatically,
however some integration effort may be required for custom types.
Plus certain performance optimizations can be achieved with additional integration effort.

daScript AOT integration is done on the ast expression tree level, and not on the simulation node level.

---------
das_index
---------

``das_index`` template is to provide implementation of ``ExprAt`` and ``ExprSafeAt`` ast nodes.

Given the input type `VecT`, output result `TT` and index type of int32_t,
``das_index`` needs to implement the following functions::

    // regular index
        static __forceinline TT & at ( VecT & value, int32_t index, Context * __context__ );
        static __forceinline const TT & at ( const VecT & value, int32_t index, Context * __context__ );
    // safe index
        static __forceinline TT * safe_at ( VecT * value, int32_t index, Context * );
        static __forceinline const TT * safe_at ( const VecT * value, int32_t index, Context * );

Note that sometimes more than one index type is possible.
In that case implementation for each index type is required.

Note how both const and not const versions are available.
Additionally const and non const version of das_index template itself may be required.

------------
das_iterator
------------

``das_iterator`` template is to provide for loop backend for the ``ExprFor`` sources.

Lets review the following example, which implements iteration over range type::

    template <>
    struct das_iterator <const range> {
        __forceinline das_iterator(const range & r) : that(r) {}
        __forceinline bool first ( Context *, int32_t & i ) { i = that.from; return i!=that.to; }
        __forceinline bool next  ( Context *, int32_t & i ) { i++; return i!=that.to; }
        __forceinline void close ( Context *, int32_t &   ) {}
        range that;
    };

``das_iterator`` template needs to implement constructor form the specified type,
also ``first``, ``next`` and ``close`` functions similar to that of the Iterator.

Both const and regular version of the ``das_iterator`` template are to be provided::

    template <>
    struct das_iterator <range> : das_iterator<const range> {
        __forceinline das_iterator(const range & r) : das_iterator<const range>(r) {}
    };

Ref iterator return types are C++ pointers::

    template <typename TT>
    struct das_iterator<TArray<TT>> {
        __forceinline bool first(Context * __context__, TT * & i) {

Out of the box das_iterator is implemented for all integrated types.

---------------------
AOT template function
---------------------

By default AOT generated functions expect blocks to be passed as C++ TBlock class (see :ref:`Blocks <blocks>`).
This creates significant performance overhead, which can be reduced by AOT template machinery.

Lets review the following example::

    void peek_das_string(const string & str, const TBlock<void,TTemporary<const char *>> & block, Context * context) {
        vec4f args[1];
        args[0] = cast<const char *>::from(str.c_str());
        context->invoke(block, args, nullptr);
    }

The overhead consists of type marshaling, as well as context block invocation.
However, the following template can be called instead::

    template <typename TT>
    void peek_das_string_T(const string & str, TT && block, Context *) {
        block((char *)str.c_str());
    }

Here the block is templated, and can be called without any marshaling whatsoever.
To achieve this, function registration in the module needs to be modified::

    addExtern<DAS_BIND_FUN(peek_das_string)>(*this, lib, "peek",
        SideEffects::modifyExternal,"peek_das_string_T")->setAotTemplate();

-------------------------------------
AOT settings for individual functions
-------------------------------------

There are several function annotations, which control how function AOT is generated.

``[hybrid]`` annotation indicates, that function is always called via full daScript interop ABI (slower),
as oppose to direct function call via C++ language construct (faster).
Doing this removes dependency between the two functions in the semantic hash,
which in turn allows replacing only one of the function with the simulated version.

``[no_aot]`` annotation indicates, that AOT version of the function will not be generated.
This is useful for working around AOT code-generation issues, as well as during builtin module development.

---------------------
AOT prefix and suffix
---------------------

Function or type trait expression can have custom annotation to specify prefix and suffix text around the generated call.
This may be necessary to completely replace the call itself, provide additional type conversions, and other customizations.

Lets review the following example::

    struct ClassInfoMacro : TypeInfoMacro {
        ....
        virtual void aotPrefix ( TextWriter & ss, const ExpressionPtr & ) override {
            ss << "(void *)(&";
        }
        virtual void aotSuffix ( TextWriter & ss, const ExpressionPtr & ) override {
            ss << ")";
        }

Here the class info macro converts requested type information to `void *`.
This part of the class machinery allows ``__rtti`` pointer of the class to remain void,
without including RTTI everywhere class is included.

---------------------------
AOT field prefix and suffix
---------------------------

``ExprField`` is covered by the following functions in the handled type annotation (see :ref:`Handles <handles>`)::

    virtual void aotPreVisitGetField ( TextWriter &, const string & fieldName )
    virtual void aotPreVisitGetFieldPtr ( TextWriter &, const string & fieldName )
    virtual void aotVisitGetField ( TextWriter & ss, const string & fieldName )
    virtual void aotVisitGetFieldPtr ( TextWriter & ss, const string & fieldName )

By default prefix functions do nothing, and postfix functions append `.fieldName` and `->fieldName` accordingly.

Note that ``ExprSafeField`` is not covered yet, and to be implemented for AOT at some point.
