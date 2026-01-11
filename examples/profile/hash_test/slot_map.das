options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module slot_map shared private

require daslib/ast_boost
require daslib/templates_boost
require daslib/typemacro_boost
require daslib/macro_boost

[skip_field_lock_check]
struct template TSlotMap {
    id : uint64
    hashMap : HashMapType
    def TSlotMap {
        static_if (!typeinfo is_table(type<HashMapType>)) {
            hashMap <- HashMapType()
        }
    }
    def static emplace(var self : TSlotMap explicit; val : int) {
        with (self) {
            let r = id
            unsafe(hashMap[id]) = val
            id ++
            return r
        }
    }
}

[typemacro_template_function(TSlotMap)]
def private makeSlotMap(macroArgument, passArgument : TypeDeclPtr; HashMapType : TypeDeclPtr) : TypeDeclPtr {
    //! generated template boilerplate for TSlotMap
    return <- default<TypeDeclPtr>
}

[typemacro_function]
def SlotMap(macroArgument, passArgument : TypeDeclPtr; HashMapType : TypeDeclPtr) : TypeDeclPtr {
    return <- makeSlotMap(macroArgument, passArgument, HashMapType)
}

