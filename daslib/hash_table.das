options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module hash_table shared private

require daslib/ast_boost
require daslib/templates_boost
require daslib/typemacro_boost

struct public TemplateCounterBase {}

require daslib/random public
require strings public

struct TFlatHashMapBase {}

//! FlatHashMap is a hash map that uses open addressing with linear probing.
[skip_field_lock_check]
struct template TFlatHashMap : TFlatHashMapBase {
    // macro adds KeyType and ValueType typedefs
    kys  : array<KeyType>
    hashes : array<uint64>
    vals : array<ValueType>
    mask : int
    len  : int
    capacity : int
    tombstones : int

    def TFlatHashMap(data : array<tuple<k : KeyType; v : ValueType>>) {
        let bit = int(clz(uint(length(data))))
        let initialSize = 1 << (32 - bit)
        mask = initialSize - 1
        len = 0
        capacity = initialSize
        kys |> resize(initialSize)
        hashes |> resize(initialSize)
        vals |> resize(initialSize)
        for (kv in data) {
            self[kv.k] := kv.v
        }
    }

    def TFlatHashMap(initialSize : int = 8) {
        assert((initialSize & (initialSize - 1)) == 0, "size must be a power of 2")
        capacity = initialSize
        kys |> resize(initialSize)
        hashes |> resize(initialSize)
        vals |> resize(initialSize)
        mask = initialSize - 1
    }

    def const empty() : bool {
        return len == 0
    }

    def const length() : int {
        return len
    }

    def clear() {
        for (h in hashes) {
            h = 0ul
        }
        len = 0
        tombstones = 0
    }

    def grow() {
        let newSize = capacity * 2
        reserve(newSize)
    }

    def rehash() {
        reserve(capacity)
    }

    [hint(unsafe_range_check, noalias=self)]
    def reserve(newSize : int) {
        assert((newSize & (newSize - 1)) == 0, "size must be a power of 2")
        var newKeys : array<KeyType>
        var newHashes : array<uint64>
        var newValues : array<ValueType>
        newKeys |> resize(newSize)
        newHashes |> resize(newSize)
        newValues |> resize(newSize)
        mask = newSize - 1
        swap(newKeys, kys)
        swap(newHashes, hashes)
        swap(newValues, vals)
        len = 0
        tombstones = 0
        capacity = newSize
        for (k, v, h in newKeys, newValues, newHashes) {
            if (h > 1ul) {
                self[k] <- v
            }
        }
        newKeys |> resize(0)
        newHashes |> resize(0)
        newValues |> resize(0)
        delete newKeys
        delete newHashes
        delete newValues
    }

    [hint(unsafe_range_check, noalias=self)]
    def key_index(key : KeyType) : int {
        let hash = hash(key) // TODO: hash function macro
        var index = int(hash) & mask
        while (true) {
            let h = hashes[index]
            if (h == hash) {
                if (kys[index] == key) {   // otherwise skip matching hash collision
                    return index
                }
            } elif (h == 0ul) {               // empty, key not found
                break
            }
            // skip hash collision or tombstone(1)
            index = (index + 1) & mask
        }
        return -1
    }

    [hint(unsafe_range_check, noalias=self)]
    def key_exists(key : KeyType) {
        let index = key_index(key)
        return index != -1
    }

    [hint(unsafe_range_check, noalias=self)]
    def get(key : KeyType; blk : block<(var v : ValueType) : void>) : bool {
        let index = key_index(key)
        if (index != -1) {
            blk |> invoke(vals[index])
            return true
        } else {
            return false
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def erase(key : KeyType) : bool {
        let index = key_index(key)
        if (index != -1) {
            hashes[index] = 1ul   // tombstone
            tombstones ++
            len --
            return true
        } else {
            return false
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def foreach(blk : block<(k : KeyType; var v : ValueType) : void>) {
        for (k, v, h in kys, vals, hashes) {
            if (h > 1ul) {
                blk |> invoke(k, v)
            }
        }
    }

    [unsafe_outside_of_for]
    def keys() {
        var pself = unsafe(addr(self))
        return <- generator<KeyType&>() <| $() {
            for (h, k in pself.hashes, pself.kys) {
                if (h > 1ul) {
                    yield k
                }
            }
            return false
        } finally {
            pself = null
        }
    }

    [unsafe_outside_of_for]
    def values() {
        var pself = unsafe(addr(self))
        return <- generator<ValueType&>() <| $() {
            for (h, v in pself.hashes, pself.vals) {
                if (h > 1ul) {
                    yield v
                }
            }
            return false
        } finally {
            pself = null
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def operator [] (key : KeyType) : ValueType& {
        if (len > (capacity >> 1)) {// ((mask+1)*2/3)
            grow()
        } elif (((capacity - len) >> 1) < tombstones) {
            rehash()
        }
        let hash = hash(key)
        var index = int(hash) & mask
        var lastTombstone = -1
        while (true) {
            let h = hashes[index]
            if (h == hash) {
                if (kys[index] == key) {   // otherwise skip matching hash collision
                    unsafe {
                        return vals[index]
                    }
                }
            } elif (h == 1ul) {
                if (lastTombstone == -1) {
                    lastTombstone = index
                }
            } elif (h == 0ul) {
                if (lastTombstone != -1) {
                    index = lastTombstone
                    tombstones --
                }
                kys[index] := key
                hashes[index] = hash
                len ++
                unsafe {
                    return vals[index]
                }
            }
            index = (index + 1) & mask
        }
        panic("unreachable")
        unsafe {
            return vals[0] // we never get here due to panic
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def const operator [] (key : KeyType) : ValueType const& {
        let index = key_index(key)
        if (index != -1) {
            unsafe {
                return vals[index]
            }
        }
        panic("key not found")
        unsafe {
            return vals[0] // we never get here due to panic
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def operator ?[] (key : KeyType) : ValueType? {
        let index = key_index(key)
        if (index != -1) {
            return unsafe(addr(vals[index]))
        } else {
            return null
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def const operator ?[] (key : KeyType) : ValueType const? {
        let index = key_index(key)
        if (index != -1) {
            return unsafe(addr(vals[index]))
        } else {
            return null
        }
    }
}

[typemacro_function]
def FlatHashMap(macroArgument, passArgument : TypeDeclPtr; key_type, value_type : TypeDeclPtr) : TypeDeclPtr {
    if (passArgument != null) {
        // this is a generic argument match, like $FlatHashMap(type<auto(TT)>, type<auto(UU)>)
        var inscope template_type <- qmacro_type(type<TFlatHashMap>)
        return <- TypeDeclPtr() if (!is_qmacro_template_instance(passArgument, template_type))
        var inscope inferred_key_type <- get_structure_alias(passArgument.structType, "KeyType")
        return <- TypeDeclPtr() if (inferred_key_type == null)
        var inscope inferred_value_type <- get_structure_alias(passArgument.structType, "ValueType")
        return <- TypeDeclPtr() if (inferred_value_type == null)
        return <- infer_template_type(passArgument, key_type, inferred_key_type)
    } else {
        // we are instantiating concrete type here
        var struct_name = "FlatHashMap<{describe(key_type,false,false,true)},{describe(value_type,false,false,true)}>"
        var existing_struct = compiling_program().find_unique_structure(struct_name)
        return <- new TypeDecl(baseType = Type.tStructure, structType = existing_struct, at = key_type.at) if (existing_struct != null)
        var inscope resType <- qmacro_template_class(struct_name, type<TFlatHashMap>)
        var inscope KeyType <- clone_type(key_type)
        add_structure_alias(resType.structType, "KeyType", KeyType)
        var inscope ValueType <- clone_type(value_type)
        add_structure_alias(resType.structType, "ValueType", ValueType)
        // for demo purposes, we can add a print_counter function here
        return <- resType
    }
}

