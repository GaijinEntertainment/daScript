options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module flat_hash_table shared private

require daslib/ast_boost
require daslib/templates_boost
require daslib/typemacro_boost

struct TFlatHashTableBase {}

//! TFlatHashTable is a hash map that uses open addressing with linear probing.
[skip_field_lock_check]
struct template TFlatHashTable : TFlatHashTableBase {
    key_data   : array<KeyType>
    hashes : array<uint64>
    value_data : array<ValueType>
    mask : int
    data_length : int
    capacity : int
    tombstones : int

    def static TFlatHashTable(data : tuple<k : KeyType; v : ValueType>[]) : TFlatHashTable {
        var self : TFlatHashTable
        with (self) {
            let bit = int(clz(uint(length(data))))
            let initialSize = 1 << (32 - bit)
            mask = initialSize - 1
            data_length = 0
            capacity = initialSize
            key_data |> resize(initialSize)
            hashes |> resize(initialSize)
            value_data |> resize(initialSize)
            for (kv in data) {
                self[kv.k] := kv.v
            }
        }
        return <- self
    }

    def TFlatHashTable(data : array<tuple<k : KeyType; v : ValueType>>) {
        let bit = int(clz(uint(length(data))))
        let initialSize = 1 << (32 - bit)
        mask = initialSize - 1
        data_length = 0
        capacity = initialSize
        key_data |> resize(initialSize)
        hashes |> resize(initialSize)
        value_data |> resize(initialSize)
        for (kv in data) {
            self[kv.k] := kv.v
        }
    }

    def TFlatHashTable(initialSize : int = 8) {
        assert((initialSize & (initialSize - 1)) == 0, "size must be a power of 2")
        capacity = initialSize
        key_data |> resize(initialSize)
        hashes |> resize(initialSize)
        value_data |> resize(initialSize)
        mask = initialSize - 1
    }

    def static empty(var self : TFlatHashTable explicit) : bool {
        return self.data_length == 0
    }

    def static length(var self : TFlatHashTable explicit) : int {
        return self.data_length
    }

    def static clear(var self : TFlatHashTable explicit) {
        with (self) {
            for (h in hashes) {
                h = 0ul
            }
            data_length = 0
            tombstones = 0
        }
    }

    def static grow(var self : TFlatHashTable explicit) {
        with (self) {
            let newSize = capacity * 2
            self |> reserve(newSize)
        }
    }

    def static rehash(var self : TFlatHashTable explicit) {
        with (self) {
            self |> reserve(capacity)
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static reserve(var self : TFlatHashTable explicit; newSize : int) {
        assert((newSize & (newSize - 1)) == 0, "size must be a power of 2")
        with (self) {
            var newkey_data : array<KeyType>
            var newHashes : array<uint64>
            var newvalue_data : array<ValueType>
            newkey_data |> resize(newSize)
            newHashes |> resize(newSize)
            newvalue_data |> resize(newSize)
            mask = newSize - 1
            swap(newkey_data, key_data)
            swap(newHashes, hashes)
            swap(newvalue_data, value_data)
            data_length = 0
            tombstones = 0
            capacity = newSize
            for (k, v, h in newkey_data, newvalue_data, newHashes) {
                if (h > 1ul) {
                    self[k] <- v
                }
            }
            newkey_data |> resize(0)
            newHashes |> resize(0)
            newvalue_data |> resize(0)
            delete newkey_data
            delete newHashes
            delete newvalue_data
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static key_index(self : TFlatHashTable explicit; key : KeyType) {
        with (self) {
            let hash = hash(key)
            var index = int(hash) & mask
            while (true) {
                let h = hashes[index]
                if (h == hash) {
                    if (key_data[index] == key) {   // otherwise skip matching hash collision
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
    def static key_exists(self : TFlatHashTable explicit; key : KeyType) {
        with (self) {
            let index = self |> key_index(key)
            return index != -1
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static get(var self : TFlatHashTable == const explicit; key : KeyType; blk : block<(var v : ValueType) : void>) : bool {
        with (self) {
            let index = self |> key_index(key)
            if (index != -1) {
                blk |> invoke(value_data[index])
                return true
            } else {
                return false
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static get(self : TFlatHashTable == const explicit; key : KeyType; blk : block<(v : ValueType) : void>) : bool {
        with (self) {
            let index = self |> key_index(key)
            if (index != -1) {
                blk |> invoke(value_data[index])
                return true
            } else {
                return false
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static erase(var self : TFlatHashTable explicit; key : KeyType) : bool {
        with (self) {
            let index = self |> key_index(key)
            if (index != -1) {
                hashes[index] = 1ul   // tombstone
                tombstones ++
                data_length --
                return true
            } else {
                return false
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static foreach(self : TFlatHashTable == const explicit; blk : block<(k : KeyType; v : ValueType) : void>) {
        with (self) {
            for (k, v, h in key_data, value_data, hashes) {
                if (h > 1ul) {
                    blk |> invoke(k, v)
                }
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static foreach(var self : TFlatHashTable == const explicit; blk : block<(k : KeyType; var v : ValueType) : void>) {
        with (self) {
            for (k, v, h in key_data, value_data, hashes) {
                if (h > 1ul) {
                    blk |> invoke(k, v)
                }
            }
        }
    }

    [unsafe_outside_of_for]
    def static keys(var self : TFlatHashTable explicit) {
        var pself = unsafe(addr(self))
        return <- generator<KeyType&>() <| $() {
            for (h, k in pself.hashes, pself.key_data) {
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
    def static values(var self : TFlatHashTable == const explicit) {
        var pself = unsafe(addr(self))
        return <- generator<ValueType&>() <| $() {
            for (h, v in pself.hashes, pself.value_data) {
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
    def static values(self : TFlatHashTable == const explicit) {
        var pself = unsafe(addr(self))
        return <- generator<ValueType const&>() <| $() {
            for (h, v in pself.hashes, pself.value_data) {
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
    def static operator [] (var self : TFlatHashTable == const explicit; key : KeyType) : ValueType & {
        with (self) {
            if (data_length > (capacity >> 1)) {// ((mask+1)*2/3)
                self |> grow()
            } elif (((capacity - data_length) >> 1) < tombstones) {
                self |> rehash()
            }
            let hash = hash(key) // TODO: hash function
            var index = int(hash) & mask
            var lastTombstone = -1
            while (true) {
                let h = hashes[index]
                if (h == hash) {
                    if (key_data[index] == key) {   // otherwise skip matching hash collision
                        unsafe {
                            return value_data[index]
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
                    key_data[index] := key
                    hashes[index] = hash
                    data_length ++
                    unsafe {
                        return value_data[index]
                    }
                }
                index = (index + 1) & mask
            }
            panic("unreachable")
            unsafe {
                return value_data[0] // we never get here due to panic
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static operator [] (self : TFlatHashTable == const explicit; key : KeyType) : ValueType const & {
        with (self) {
            let index = self |> key_index(key)
            if (index != -1) {
                unsafe {
                    return value_data[index]
                }
            }
            panic("key not found")
            unsafe {
                return value_data[0] // we never get here due to panic
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static operator ?[] (var self : TFlatHashTable == const explicit; key : KeyType) : ValueType ? {
        with (self) {
            let index = self |> key_index(key)
            if (index != -1) {
                return unsafe(addr(value_data[index]))
            } else {
                return null
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static operator ?[] (self : TFlatHashTable == const explicit; key : KeyType) : ValueType const ? {
        with (self) {
            let index = self |> key_index(key)
            if (index != -1) {
                return unsafe(addr(value_data[index]))
            } else {
                return null
            }
        }
    }
}

[typemacro_function]
def FlatHashTable(macroArgument, passArgument : TypeDeclPtr; key_type, value_type : TypeDeclPtr) : TypeDeclPtr {
    // TODO: consider full template macro, for general purposes
    //  however, once hash_function is specialized, we may need typemacro_function anyway
    if (passArgument != null) {
        // this is a generic argument match, like $TFlatHashTable(type<auto(TT)>, type<auto(UU)>)
        var inscope template_type <- qmacro_type(type<TFlatHashTable>)
        return <- TypeDeclPtr() if (!is_qmacro_template_instance(passArgument, template_type))
        var inscope inferred_key_type <- get_structure_alias(passArgument.structType, "KeyType")
        return <- TypeDeclPtr() if (inferred_key_type == null)
        var inscope inferred_value_type <- get_structure_alias(passArgument.structType, "ValueType")
        return <- TypeDeclPtr() if (inferred_value_type == null)
        var inscope kt = clone_type(key_type)
        var inscope vt = clone_type(value_type)
        return <- infer_template_types(passArgument, [(kt, inferred_key_type), (vt, inferred_value_type)])
    } else {
        // we are instantiating concrete type here
        var struct_name = "TFlatHashTable<{describe(key_type,false,false,true)},{describe(value_type,false,false,true)}>"
        var existing_struct = compiling_program().find_unique_structure(struct_name)
        return <- new TypeDecl(baseType = Type.tStructure, structType = existing_struct, at = key_type.at) if (existing_struct != null)
        var inscope resType <- qmacro_template_class(struct_name, type<TFlatHashTable>)
        var inscope KeyType <- clone_type(key_type)
        add_structure_alias(resType.structType, "KeyType", KeyType)
        var inscope ValueType <- clone_type(value_type)
        add_structure_alias(resType.structType, "ValueType", ValueType)
        // for demo purposes, we can add a print_counter function here
        return <- resType
    }
}

