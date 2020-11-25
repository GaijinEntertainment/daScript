.. _cast:

=======================================
C++ ABI and type factory infrastructure
=======================================

----
Cast
----

When C++ interfaces with daScript, `cast` ABI is followed.

 * by value types are converted to and from `vec4f`, in specific memory layout
 * by reference types have their address converted to and from `vec4f`

It is expected that vec4f * can be prune to by value type by simple pointer cast,
becase daScript interpreter will in certain cases access pre-cast data via v_ldu intrinsic::

    template <typename TT>
    TT get_data ( vec4f * dasData ) {           // default version
        return cast<TT>::to(v_ldu((float *)dasData));
    }

    int32_t get_data ( vec4f * dasData ) {      // legally optimized version
        return * (int32_t *) dasData;
    }

ABI infrastructure is implemented via C++ cast template, which servers two primary functions

 * casting ``from`` C++ to daScript
 * casting ``to`` C++ from daScript

The ``from`` function expects daScript type as an input, and outputs vec4f.

The ``to`` function expects vec4f, and outputs daScript type.

Lets review the following example::

    template <>
    struct cast <int32_t> {
        static __forceinline int32_t to ( vec4f x )            { return v_extract_xi(v_cast_vec4i(x)); }
        static __forceinline vec4f from ( int32_t x )          { return v_cast_vec4f(v_splatsi(x)); }
    };

It implements ABI for the int32_t, which packs int32_t value at the beginning of vec4f using multiplatform intrinsics.

Lets review example, which implements default packing of by reference type::

    template <typename TT>
    struct cast <TT &> {
        static __forceinline TT & to ( vec4f a )               { return *(TT *) v_extract_ptr(v_cast_vec4i((a))); }
        static __forceinline vec4f from ( const TT & p )       { return v_cast_vec4f(v_splats_ptr((const void *)&p)); }
    };

Here pointer to the data is packed in vec4f using multiplatform intrinsics.

------------
Type factory
------------

When C++ types are exposed to daScript, type factory infrastructure is employed.

To expose any custom C++ type, use ``MAKE_TYPE_FACTORY`` macro,
or ``MAKE_EXTERNAL_TYPE_FACTORY`` and ``IMPLEMENT_EXTERNAL_TYPE_FACTORY`` macro pair::

    MAKE_TYPE_FACTORY(clock, das::Time)

Example above tells daScript that C++ type `das::Time` will be exposed to the daScript with the name `clock`.

Lets look at implementation of ``MAKE_TYPE_FACTORY`` macro::

    #define MAKE_TYPE_FACTORY(TYPE,CTYPE) \
        namespace das { \
        template <> \
        struct typeFactory<CTYPE> { \
            static TypeDeclPtr make(const ModuleLibrary & library ) { \
                return makeHandleType(library,#TYPE); \
            } \
        }; \
        template <> \
        struct typeName<CTYPE> { \
            constexpr static const char * name() { return #TYPE; } \
        }; \
        };

What happens in the example above is that two templated policies are exposed to C++.

``typeName`` policy has single static function ``name``, which returns string name of the type.

``typeFactory`` policy creates a smart pointer to daScript `das::TypeDecl` type, which corresponds to C++ type.
It expects to find type somewhere in the provided ModuleLibrary (see :ref:`Modules <modules>`).

------------
Type aliases
------------

Custom type factory is preferable way to create aliases::

    struct Point3 { float x, y, z; };

    template <>
    struct typeFactory<Point3> {
        static TypeDeclPtr make(const ModuleLibrary &) {
            auto t = make_smart<TypeDecl>(Type::tFloat3);
            t->alias = "Point3";
            t->aotAlias = true;
            return t;
        }
    };

    template <> struct typeName<Point3>   { constexpr static const char * name() { return "Point3"; } };

In the example above C++ application already has `Point3` type, which is very similar to daScript float3.
Exposing C++ functions which operate on Point3 is preferable, so implementation creates an alias named `Point3`
which corresponds to das Type::tFloat3.

Sometimes custom implementation of ``typeFactory`` is be required to expose C++ to daScript
type in a more native fashion. Lets review the following example::

    struct SampleVariant {
        int32_t _variant;
        union {
            int32_t     i_value;
            float       f_value;
            char *      s_value;
        };
    };

  template <>
  struct typeFactory<SampleVariant> {
      static TypeDeclPtr make(const ModuleLibrary & library ) {
          auto vtype = make_smart<TypeDecl>(Type::tVariant);
          vtype->alias = "SampleVariant";
          vtype->aotAlias = true;
          vtype->addVariant("i_value", typeFactory<decltype(SampleVariant::i_value)>::make(library));
          vtype->addVariant("f_value", typeFactory<decltype(SampleVariant::f_value)>::make(library));
          vtype->addVariant("s_value", typeFactory<decltype(SampleVariant::s_value)>::make(library));
          // optional validation
          DAS_ASSERT(sizeof(SampleVariant) == vtype->getSizeOf());
          DAS_ASSERT(alignof(SampleVariant) == vtype->getAlignOf());
          DAS_ASSERT(offsetof(SampleVariant, i_value) == vtype->getVariantFieldOffset(0));
          DAS_ASSERT(offsetof(SampleVariant, f_value) == vtype->getVariantFieldOffset(1));
          DAS_ASSERT(offsetof(SampleVariant, s_value) == vtype->getVariantFieldOffset(2));
          return vtype;
      }
  };

Here C++ type `SomeVariant` matches daScript variant type with its memory layout.
The code above exposes C++ type alias and creates corresponding TypeDecl.
