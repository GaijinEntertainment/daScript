options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers = true

module dynamic_cast_rtti shared private

require rtti
require daslib/contracts
require daslib/templates

[expect_class(instance), template(otherclass)]
def public is_instance_of(instance; otherclass : auto(TT) )
    var refinfo = unsafe(reinterpret<TypeInfo?> typeinfo(rtti_classinfo type<TT - const>))
    var clsinfo = unsafe(reinterpret<TypeInfo?> instance.__rtti)
    while clsinfo != null
        if clsinfo.hash == refinfo.hash
            return true
        clsinfo = clsinfo.firstType
    return false

[expect_class(instance), template(otherclass)]
def public dynamic_type_cast(instance; otherclass : auto(TT) ) : TT -const ?
    var refinfo = unsafe(reinterpret<TypeInfo?> typeinfo(rtti_classinfo type<TT - const>))
    var clsinfo = unsafe(reinterpret<TypeInfo?> instance.__rtti)
    while clsinfo != null
        if clsinfo.hash == refinfo.hash
            return unsafe(reinterpret<TT - const?> instance)
        clsinfo = clsinfo.firstType
    return null


