options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module cuckoo_hash_table shared private

require daslib/ast_boost
require daslib/templates_boost
require daslib/typemacro_boost
require daslib/macro_boost
require daslib/random public
require strings public

def public hash0(data) {
    return hash(data) >>> 32ul
}

/*
def hash0(s : string) : uint64 {
    var h : uint64 = 14695981039346656037ul
    for (c in s) {
        h = (h ^ uint64(c)) * 1099511628211ul
    }
    return h
}
*/

[skip_field_lock_check]
struct template TCuckooHashTable {
    typedef KHV_TCuckooHashTable = tuple<key : KeyType; hash : uint64; value : ValueType>
    khv : array<KHV_TCuckooHashTable>
    mask : int
    data_length : int
    seed : int4

    def static TCuckooHashTable(data : tuple<k : KeyType; v : ValueType>[]) : TCuckooHashTable {
        var self : TCuckooHashTable
        with (self) {
            seed = random_seed(13)
            let bit = int(clz(uint(length(data))))
            let initialSize = 1 << (32 - bit)
            mask = initialSize - 1
            data_length = 0
            khv |> resize(initialSize)
            for (kv in data) {
                self[kv.k] := kv.v
            }
        }
        return <- self
    }

    def TCuckooHashTable(data : array<tuple<k : KeyType; v : ValueType>>) {
        seed = random_seed(13)
        let bit = int(clz(uint(length(data))))
        let initialSize = 1 << (32 - bit)
        mask = initialSize - 1
        data_length = 0
        khv |> resize(initialSize)
        for (kv in data) {
            self[kv.k] := kv.v
        }
    }

    def TCuckooHashTable(initialSize : int = 64) {
        assert((initialSize & (initialSize - 1)) == 0, "size must be a power of 2")
        seed = random_seed(13)
        khv |> resize(initialSize)
        mask = initialSize - 1
    }

    def static empty(var self : TCuckooHashTable explicit) : bool {
        return self.data_length == 0
    }

    def static length(var self : TCuckooHashTable explicit) : int {
        return self.data_length
    }

    def static clear(var self : TCuckooHashTable explicit) {
        with (self) {
            for (t in khv) {
                t.hash = 0ul
            }
            data_length = 0
        }
    }

    def static grow(var self : TCuckooHashTable explicit) {
        with (self) {
            let newSize = length(khv) * 2
            self |> reserve(newSize)
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static reserve(var self : TCuckooHashTable explicit; newSize : int) {
        assert((newSize & (newSize - 1)) == 0, "size must be a power of 2")
        with (self) {
            var newKHV : array<KHV_TCuckooHashTable>
            newKHV |> resize(newSize)
            mask = newSize - 1
            swap(newKHV, khv)
            data_length = 0
            for (t in newKHV) {
                if (t.hash != 0ul) {
                    self[t.key] <- t.value
                }
            }
            newKHV |> resize(0)
            delete newKHV
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static key_index(self : TCuckooHashTable explicit; key : KeyType) {
        with (self) {
            let firstHash = $c(hashFunction0Name)(key)
            var index = int(firstHash) & mask
            if (khv[index].hash == firstHash && khv[index].key == key) {
                return index
            }
            var secondHash = $c(hashFunction1Name)(key)
            index = int(secondHash) & mask
            if (khv[index].hash == secondHash && khv[index].key == key) {
                return index
            }
            return -1
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static key_exists(self : TCuckooHashTable explicit; key : KeyType) {
        with (self) {
            let index = self |> key_index(key)
            return index != -1
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static get(var self : TCuckooHashTable == const explicit; key : KeyType; blk : block<(var v : ValueType) : void>) : bool {
        with (self) {
            let index = self |> key_index(key)
            if (index != -1) {
                blk |> invoke(khv[index].value)
                return true
            } else {
                return false
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static get(self : TCuckooHashTable == const explicit; key : KeyType; blk : block<(v : ValueType) : void>) : bool {
        with (self) {
            let index = self |> key_index(key)
            if (index != -1) {
                blk |> invoke(khv[index].value)
                return true
            } else {
                return false
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static erase(var self : TCuckooHashTable explicit; key : KeyType) : bool {
        with (self) {
            let index = self |> key_index(key)
            if (index != -1) {
                khv[index].hash = 0ul
                data_length --
                return true
            } else {
                return false
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static foreach(self : TCuckooHashTable == const explicit; blk : block<(k : KeyType; v : ValueType) : void>) {
        with (self) {
            for (t in khv) {
                if (t.hash != 0ul) {
                    blk |> invoke(t.key, t.value)
                }
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static foreach(var self : TCuckooHashTable == const explicit; blk : block<(k : KeyType; var v : ValueType) : void>) {
        with (self) {
            for (t in khv) {
                if (t.hash != 0ul) {
                    blk |> invoke(t.key, t.value)
                }
            }
        }
    }

    [unsafe_outside_of_for]
    def static keys(var self : TCuckooHashTable explicit) {
        var pself = unsafe(addr(self))
        return <- generator<KeyType&>() <| $() {
            for (t in pself.khv) {
                if (t.hash != 0ul) {
                    yield t.key
                }
            }
            return false
        } finally {
            pself = null
        }
    }

    [unsafe_outside_of_for]
    def static values(var self : TCuckooHashTable == const explicit) {
        var pself = unsafe(addr(self))
        return <- generator<ValueType&>() <| $() {
            for (t in pself.khv) {
                if (t.hash != 0ul) {
                    yield t.value
                }
            }
            return false
        } finally {
            pself = null
        }
    }

    [unsafe_outside_of_for]
    def static values(self : TCuckooHashTable == const explicit) {
        var pself = unsafe(addr(self))
        return <- generator<ValueType const&>() <| $() {
            for (t in pself.khv) {
                if (t.hash != 0ul) {
                    yield t.value
                }
            }
            return false
        } finally {
            pself = null
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static insert(var self : TCuckooHashTable explicit; _key : KeyType; _value : ValueType) {
        //! insert a key value pair into the hash map
        with (self) {
            var key := _key
            var value := _value
            while (true) {
                for (i in range(length(khv) / 2)) {
                    var firstHash = $c(hashFunction0Name)(key)
                    var firstIndex = int(firstHash) & mask
                    if (khv[firstIndex].hash == 0ul) {
                        khv[firstIndex].hash = firstHash
                        khv[firstIndex].key <- key
                        khv[firstIndex].value <- value
                        data_length ++
                        return
                    }
                    var secondHash = $c(hashFunction1Name)(key)
                    var secondIndex = int(secondHash) & mask
                    if (khv[secondIndex].hash == 0ul) {
                        khv[secondIndex].hash = secondHash
                        khv[secondIndex].key <- key
                        khv[secondIndex].value <- value
                        data_length ++
                        return
                    }
                    if ((random_int(seed) & 1) == 0) {
                        swap(key, khv[firstIndex].key)
                        swap(value, khv[firstIndex].value)
                        khv[firstIndex].hash = firstHash
                    } else {
                        swap(key, khv[secondIndex].key)
                        swap(value, khv[secondIndex].value)
                        khv[secondIndex].hash = secondHash
                    }
                }
                self |> grow()
            }
        }
    }

    def static describe(var self : TCuckooHashTable == const explicit) {
        with (self) {
            return build_string <| $(writer) {
                writer |> write("\{\{\n")
                for (t, index in khv, count()) {
                    if (t.hash != 0ul) {
                        writer |> write(t.key)
                        writer |> write("=>")
                        writer |> write(t.value)
                        writer |> write(index)
                        writer |> write(";")
                    }
                }
                writer |> write("\}\}\n")
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static operator [] (var self : TCuckooHashTable == const explicit; key : KeyType) : ValueType & {
        with (self) {
            if (data_length > ((mask + 1) >> 1)) {// ((mask+1)*2/3)
                self |> grow()
            }
            // if its already there
            var firstHash = $c(hashFunction0Name)(key)
            var firstIndex = int(firstHash) & mask
            if (khv[firstIndex].hash == firstHash && khv[firstIndex].key == key) {
                unsafe {
                    return khv[firstIndex].value
                }
            }
            var secondHash = $c(hashFunction1Name)(key)
            var secondIndex = int(secondHash) & mask
            if (khv[secondIndex].hash == secondHash && khv[secondIndex].key == key) {
                unsafe {
                    return khv[secondIndex].value
                }
            }
            // now, if there is a room to insert
            if (khv[firstIndex].hash == 0ul) {
                khv[firstIndex].hash = firstHash
                khv[firstIndex].key := key
                data_length ++
                unsafe {
                    return khv[firstIndex].value
                }
            }
            if (khv[secondIndex].hash == 0ul) {
                khv[secondIndex].hash = secondHash
                khv[secondIndex].key := key
                data_length ++
                unsafe {
                    return khv[secondIndex].value
                }
            }
            // now we have to kick out one of the existing keys
            self |> insert(key, default<ValueType>)
            let index = self |> key_index(key)
            unsafe {
                return khv[index].value
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static operator [] (self : TCuckooHashTable == const explicit; key : KeyType) : ValueType const & {
        with (self) {
            let index = self |> key_index(key)
            if (index != -1) {
                unsafe {
                    return khv[index].value
                }
            }
            panic("key not found")
            unsafe {
                return khv[0].value // we never get here due to panic
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static operator ?[] (var self : TCuckooHashTable == const explicit; key : KeyType) : ValueType ? {
        with (self) {
            let index = self |> key_index(key)
            if (index != -1) {
                return unsafe(addr(khv[index].value))
            } else {
                return null
            }
        }
    }

    [hint(unsafe_range_check, noalias=self)]
    def static operator ?[] (self : TCuckooHashTable == const explicit; key : KeyType) : ValueType const ? {
        with (self) {
            let index = self |> key_index(key)
            if (index != -1) {
                return unsafe(addr(khv[index].value))
            } else {
                return null
            }
        }
    }
}

[typemacro_template_function(TCuckooHashTable)]
def private makeCuckooHashTable(macroArgument, passArgument : TypeDeclPtr; KeyType, ValueType : TypeDeclPtr; hashFunction0Name, hashFunction1Name : string) : TypeDeclPtr {
    //! generated template boilerplate for CuckooHashTable
    return <- default<TypeDeclPtr>
}

[typemacro_function]
def CuckooHashTable(macroArgument, passArgument : TypeDeclPtr; KeyType, ValueType : TypeDeclPtr) : TypeDeclPtr {
    //! CuckooHashTable<K;V> is a hash map that uses open addressing with linear probing and default hash function.
    return <- makeCuckooHashTable(macroArgument, passArgument, KeyType, ValueType, "hash", "hash0")
}

[typemacro_function]
def CuckooHashTableEx(macroArgument, passArgument : TypeDeclPtr; KeyType, ValueType : TypeDeclPtr; hashName, hash0name : string) : TypeDeclPtr {
    //! CuckooHashTableEx<K;V;hashName> is a hash map that uses open addressing with linear probing and custom hash function.
    return <- makeCuckooHashTable(macroArgument, passArgument, KeyType, ValueType, hashName, hash0name)
}
