.. variants:

=======
Variant
=======

Variant is a nameless type which provides support for values that can be one of a number of named cases,
possibly each with different values and types::

    var t : variant<i_value:uint;f_value:float>

There is shorthand type alias syntax to define a variant::

    variant U_F
        i_value : uint
        f_value : float

    typedef
        U_F = variant<i_value:uint;f_value:float> // exactly the same as the declaration above

Any two variants are the same type, if they have the same named cases of the same types in the same order.

Variant holds ``index`` of current case, as well as value for the current case only.

Current case selection can be checked via ``is`` operator, and accessed via ``as`` operator::

    assert(t is i_value)
    assert(t as i_value == 0x3f800000)

Entire variant selection can be modified by copying properly constructed variant of different case::

    t = [[U_F i_value = 0x40000000]]    // now case is i_value
    t = [[U_F f_value = 1.0]]           // now case is f_value

Accessing variant case of incorrect type will cause panic::

    t = [[U_F i_value = 0x40000000]]
    return t as f_value                 // panic, invalid variant index

Safe navigation is available via ``?as`` operation::

    return t ?as f_value ?? 1.0         // will return 1.0 if t is not f_value

Cases can also be accessed in unsafe manner without checking the type::

    unsafe
        t.i_value = 0x3f800000
        return t.f_value                    // will return memory, occupied by f_value - i.e. 1.0f

Current index can be determined via ``variant_index`` function::

    var t : U_F
    assert(variant_index(t)==0)

Index value for specific case can be determine via ``variant_index`` and ``safe_variant_index`` type traits.
``safe_variant_index`` will return -1 for the invalid indices and types, whereas ``variant_index`` will report a compilation error::

    assert(typeinfo(variant_index<i_value> t)==0)
    assert(typeinfo(variant_index<f_value> t)==1)
    assert(typeinfo(variant_index<unknown_value> t)==-1) // compilation error

    assert(typeinfo(safe_variant_index<i_value> t)==0)
    assert(typeinfo(safe_variant_index<f_value> t)==1)
    assert(typeinfo(safe_variant_index<unknown_value> t)==-1)

Current case selection can be modified with unsafe operation ``safe_variant_index``::

    unsafe
        set_variant_index(t, typeinfo(variant_index<f_value> t))

-------------------------
Alignment and data layout
-------------------------

Variant contains 'index' of the current case, followed by union of individual cases, similar to the following C++ layout::

    struct MyVariantName {
        int32_t __variant_index;
        union {
            type0   case0;
            type1   case1;
            ...
        };
    };

Individual cases start from the same offset.

Variant type is aligned by alignment of its largest case, but no less than that of int32.

