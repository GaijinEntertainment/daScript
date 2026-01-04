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

//! TFlatHashMap is a hash map that uses open addressing with linear probing.
[skip_field_lock_check]
struct template TFlatHashMap : TFlatHashMapBase {
    keys   : array<KeyType>
    hashes : array<uint64>
    values : array<ValueType>
    mask : int
    length : int
    capacity : int
    tombstones : int

    def static TFlatHashMap(data : tuple<k : KeyType; v : ValueType>[]) : TFlatHashMap {
        var self : TFlatHashMap
        with (self) {
            let bit = int(clz(uint(length(data))))
            let initialSize = 1 << (32 - bit)
            mask = initialSize - 1
            length = 0
            capacity = initialSize
            keys |> resize(initialSize)
            hashes |> resize(initialSize)
            values |> resize(initialSize)
            for (kv in data) {
                self[kv.k] := kv.v
            }
        }
        return <- self
    }

    def TFlatHashMap(data : array<tuple<k : KeyType; v : ValueType>>) {
        let bit = int(clz(uint(length(data))))
        let initialSize = 1 << (32 - bit)
        mask = initialSize - 1
        length = 0
        capacity = initialSize
        keys |> resize(initialSize)
        hashes |> resize(initialSize)
        values |> resize(initialSize)
        for (kv in data) {
            self[kv.k] := kv.v
        }
    }

    def TFlatHashMap(initialSize : int = 8) {
        assert((initialSize & (initialSize - 1)) == 0, "size must be a power of 2")
        capacity = initialSize
        keys |> resize(initialSize)
        hashes |> resize(initialSize)
        values |> resize(initialSize)
        mask = initialSize - 1
    }

    def static empty(var self : TFlatHashMap explicit) : bool {
        return self.length == 0
    }

    def static length(var self : TFlatHashMap explicit) : int {
        return self.length
    }

    def static clear(var self : TFlatHashMap explicit) {
        with (self) {
            for (h in hashes) {
                h = 0ul
            }
            length = 0
            tombstones = 0
        }
    }

    def static grow(var self : TFlatHashMap explicit) {
        with (self) {
            let newSize = capacity * 2
            self |> reserve(newSize)
        }
    }

    def static rehash(var self : TFlatHashMap explicit) {
        with (self) {
            self |> reserve(capacity)
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static reserve(var self : TFlatHashMap explicit; newSize : int) {
        assert((newSize & (newSize - 1)) == 0, "size must be a power of 2")
        with (self) {
            var newKeys : array<KeyType>
            var newHashes : array<uint64>
            var newValues : array<ValueType>
            newKeys |> resize(newSize)
            newHashes |> resize(newSize)
            newValues |> resize(newSize)
            mask = newSize - 1
            swap(newKeys, keys)
            swap(newHashes, hashes)
            swap(newValues, values)
            length = 0
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
    }

    [hint(unsafe_range_check, noalias=self)]
    def static key_index(self : TFlatHashMap explicit; key : KeyType) {
        with (self) {
            let hash = hash(key)
            var index = int(hash) & mask
            while (true) {
                let h = hashes[index]
                if (h == hash) {
                    if (keys[index] == key) {   // otherwise skip matching hash collision
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
    }

    [hint(unsafe_range_check, noalias=self)]
    def static key_exists(self : TFlatHashMap explicit; key : KeyType) {
        with (self) {
            let index = self |> key_index(key)
            return index != -1
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static get(var self : TFlatHashMap == const explicit; key : KeyType; blk : block<(var v : ValueType) : void>) : bool {
        with (self) {
            let index = self |> key_index(key)
            if (index != -1) {
                blk |> invoke(values[index])
                return true
            } else {
                return false
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static get(self : TFlatHashMap == const explicit; key : KeyType; blk : block<(v : ValueType) : void>) : bool {
        with (self) {
            let index = self |> key_index(key)
            if (index != -1) {
                blk |> invoke(values[index])
                return true
            } else {
                return false
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static erase(var self : TFlatHashMap explicit; key : KeyType) : bool {
        with (self) {
            let index = self |> key_index(key)
            if (index != -1) {
                hashes[index] = 1ul   // tombstone
                tombstones ++
                length --
                return true
            } else {
                return false
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static foreach(self : TFlatHashMap == const explicit; blk : block<(k : KeyType; v : ValueType) : void>) {
        with (self) {
            for (k, v, h in keys, values, hashes) {
                if (h > 1ul) {
                    blk |> invoke(k, v)
                }
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static foreach(var self : TFlatHashMap == const explicit; blk : block<(k : KeyType; var v : ValueType) : void>) {
        with (self) {
            for (k, v, h in keys, values, hashes) {
                if (h > 1ul) {
                    blk |> invoke(k, v)
                }
            }
        }
    }

    [unsafe_outside_of_for]
    def static keys(var self : TFlatHashMap explicit) {
        var pself = unsafe(addr(self))
        return <- generator<KeyType&>() <| $() {
            for (h, k in pself.hashes, pself.keys) {
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
    def static values(var self : TFlatHashMap == const explicit) {
        var pself = unsafe(addr(self))
        return <- generator<ValueType&>() <| $() {
            for (h, v in pself.hashes, pself.values) {
                if (h > 1ul) {
                    yield v
                }
            }
            return false
        } finally {
            pself = null
        }
    }

    [unsafe_outside_of_for]
    def static values(self : TFlatHashMap == const explicit) {
        var pself = unsafe(addr(self))
        return <- generator<ValueType const&>() <| $() {
            for (h, v in pself.hashes, pself.values) {
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
    def static operator [] (var self : TFlatHashMap == const explicit; key : KeyType) : ValueType & {
        with (self) {
            if (length > (capacity >> 1)) {// ((mask+1)*2/3)
                self |> grow()
            } elif (((capacity - length) >> 1) < tombstones) {
                self |> rehash()
            }
            let hash = hash(key) // TODO: hash function
            var index = int(hash) & mask
            var lastTombstone = -1
            while (true) {
                let h = hashes[index]
                if (h == hash) {
                    if (keys[index] == key) {   // otherwise skip matching hash collision
                        unsafe {
                            return values[index]
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
                    keys[index] := key
                    hashes[index] = hash
                    length ++
                    unsafe {
                        return values[index]
                    }
                }
                index = (index + 1) & mask
            }
            panic("unreachable")
            unsafe {
                return values[0] // we never get here due to panic
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static operator [] (self : TFlatHashMap == const explicit; key : KeyType) : ValueType const & {
        with (self) {
            let index = self |> key_index(key)
            if (index != -1) {
                unsafe {
                    return values[index]
                }
            }
            panic("key not found")
            unsafe {
                return values[0] // we never get here due to panic
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static operator ?[] (var self : TFlatHashMap == const explicit; key : KeyType) : ValueType ? {
        with (self) {
            let index = self |> key_index(key)
            if (index != -1) {
                return unsafe(addr(values[index]))
            } else {
                return null
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static operator ?[] (self : TFlatHashMap == const explicit; key : KeyType) : ValueType const ? {
        with (self) {
            let index = self |> key_index(key)
            if (index != -1) {
                return unsafe(addr(values[index]))
            } else {
                return null
            }
        }
    }
}

[typemacro_function]
def FlatHashMap(macroArgument, passArgument : TypeDeclPtr; key_type, value_type : TypeDeclPtr) : TypeDeclPtr {
    if (passArgument != null) {
        // this is a generic argument match, like $TFlatHashMap(type<auto(TT)>, type<auto(UU)>)
        var inscope template_type <- qmacro_type(type<TFlatHashMap>)
        return <- TypeDeclPtr() if (!is_qmacro_template_instance(passArgument, template_type))
        var inscope inferred_key_type <- get_structure_alias(passArgument.structType, "KeyType")
        return <- TypeDeclPtr() if (inferred_key_type == null)
        var inscope inferred_value_type <- get_structure_alias(passArgument.structType, "ValueType")
        return <- TypeDeclPtr() if (inferred_value_type == null)
        return <- infer_template_type(passArgument, key_type, inferred_key_type)
    } else {
        // we are instantiating concrete type here
        var struct_name = "TFlatHashMap<{describe(key_type,false,false,true)},{describe(value_type,false,false,true)}>"
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

