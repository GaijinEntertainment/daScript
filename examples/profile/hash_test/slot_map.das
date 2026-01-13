options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module slot_map shared private

require daslib/ast_boost
require daslib/templates_boost
require daslib/typemacro_boost
require daslib/macro_boost

[skip_field_lock_check, template_structure(HashMapType)]
struct template SlotMap {
    id : uint64
    hashMap : HashMapType
    def SlotMap {
        static_if (!typeinfo is_table(type<HashMapType>)) {
            hashMap <- HashMapType()
        }
    }
    def static emplace(var self : SlotMap explicit; val : int) {
        with (self) {
            let r = id
            unsafe(hashMap[id]) = val
            id ++
            return r
        }
    }
}
