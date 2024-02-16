options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module hash_map shared private

require daslib/spoof public

let public TBucketHashMap = %spoof_template~(BucketHashMap,KeyType,ValueType)
[skip_field_lock_check]
struct %BucketHashMap
    keys   : array<array<%KeyType>>
    hashes : array<array<uint64>>
    values : array<array<%ValueType>>
    mask : int
    distance : int
    length : int

def %BucketHashMap ( data : array<tuple<k:%KeyType;v:%ValueType>> ) : %BucketHashMap
    var self : %BucketHashMap
    with self
        let bit = int(clz(uint(length(data))))
        let initialSize = 1 << (32 - bit)
        mask = initialSize - 1
        distance = 4 + (32-bit)
        length = 0
        keys |> resize(initialSize)
        hashes |> resize(initialSize)
        values |> resize(initialSize)
        for kv in data
            self[kv.k] := kv.v
    return <- self

def %BucketHashMap ( initialSize : int = 64 ) : %BucketHashMap
    assert((initialSize & (initialSize-1))==0, "size must be a power of 2")
    var self : %BucketHashMap
    with self
        keys |> resize(initialSize)
        hashes |> resize(initialSize)
        values |> resize(initialSize)
        mask = initialSize - 1
        distance = 4
    return <- self

def empty ( var self:%BucketHashMap explicit ) : bool
    return self.length == 0

def length ( var self:%BucketHashMap explicit ) : int
    return self.length

def clear ( var self:%BucketHashMap explicit )
    with self
        for k,v,h in keys, values, hashes
            k |> resize(0)
            v |> resize(0)
            h |> resize(0)
        length = 0

def grow ( var self:%BucketHashMap explicit )
    with self
        let newSize = length(keys) * 2
        distance ++
        self |> reserve(newSize)

def reserve ( var self:%BucketHashMap explicit; newSize : int )
    assert((newSize & (newSize-1))==0, "size must be a power of 2")
    with self
        var newKeys : array<array<%KeyType>>
        var newHashes : array<array<uint64>>
        var newValues : array<array<%ValueType>>
        newKeys |> resize(newSize)
        newHashes |> resize(newSize)
        newValues |> resize(newSize)
        mask = newSize - 1
        swap(newKeys, keys)
        swap(newHashes, hashes)
        swap(newValues, values)
        for K,V,H in newKeys, newValues, newHashes
            for k,v,h in K,V,H
                let index = self |> find_index(h)
                keys[index] |> emplace(k)
                hashes[index] |> emplace(h)
                values[index] |> emplace(v)
        newKeys |> resize(0)
        newHashes |> resize(0)
        newValues |> resize(0)
        delete newKeys
        delete newHashes
        delete newValues

def find_index ( self:%BucketHashMap explicit; hash : uint64 ) : int
    with self
        return int(hash) & mask

def key_exists ( self:%BucketHashMap explicit; key : %KeyType )
    with self
        let hash = hash(key)
        let index = self |> find_index(hash)
        for k,v,h in keys[index], values[index], hashes[index]
            if h == hash && k == key
                return true
        return false

def get ( var self:%BucketHashMap ==const explicit; key : %KeyType; blk:block<(var v:%ValueType):void> ) : bool
    with self
        let hash = hash(key)
        let index = self |> find_index(hash)
        for k,v,h in keys[index], values[index], hashes[index]
            if h == hash && k == key
                blk |> invoke(v)
                return true
        return false

def get ( self:%BucketHashMap ==const explicit; key : %KeyType; blk:block<(v:%ValueType):void> ) : bool
    with self
        let hash = hash(key)
        let index = self |> find_index(hash)
        for k,v,h in keys[index], values[index], hashes[index]
            if h == hash && k == key
                blk |> invoke(v)
                return true
        return false

def erase ( var self:%BucketHashMap explicit; key : %KeyType ) : bool
    with self
        let hash = hash(key)
        let index = self |> find_index(hash)
        var eindex = -1
        for k,h,c in keys[index], hashes[index], count()
            if h == hash && k == key
                eindex = c
                break
        if eindex != -1
            keys[index] |> erase(eindex)
            hashes[index] |> erase(eindex)
            values[index] |> erase(eindex)
            length--
            return true
        else
            return false

def foreach ( self:%BucketHashMap ==const explicit; blk:block<(k:%KeyType;v:%ValueType):void> )
    with self
        for K,V,H in keys, values, hashes
            for k,v,h in K,V,H
                blk |> invoke(k,v)

def foreach ( var self:%BucketHashMap ==const explicit; blk:block<(k:%KeyType;var v:%ValueType):void> )
    with self
        for K,V,H in keys, values, hashes
            for k,v,h in K,V,H
                blk |> invoke(k,v)

[unsafe_outside_of_for]
def keys ( var self:%BucketHashMap explicit )
    var pself = unsafe(addr(self))
    return <- generator<%KeyType&>() <|
        for K in pself.keys
            for k in K
                yield k
        return false
    finally
        pself = null

[unsafe_outside_of_for]
def values ( var self:%BucketHashMap ==const explicit )
    var pself = unsafe(addr(self))
    return <- generator<%ValueType&>() <|
        for V in pself.values
            for v in V
                yield v
        return false
    finally
        pself = null

[unsafe_outside_of_for]
def values ( self:%BucketHashMap ==const explicit )
    var pself = unsafe(addr(self))
    return <- generator<%ValueType const&>() <|
        for V in pself.values
            for v in V
                yield v
        return false
    finally
        pself = null

def operator [] ( var self:%BucketHashMap ==const explicit; key : %KeyType ) : %ValueType&
    with self
        let hash = hash(key)
        var index = self |> find_index(hash)
        for k,v,h in keys[index], values[index], hashes[index]
            if h == hash && k == key
                unsafe
                    return v
        var lastIndex = length(keys[index])
        if lastIndex >= distance
            self |> grow()
            index = self |> find_index(hash)
            lastIndex = length(keys[index])
        length ++
        keys[index] |> push_clone(key)
        hashes[index] |> push(hash)
        static_if typeinfo(can_copy type<%ValueType>)
            values[index] |> push <| [[%ValueType]]
        else
            values[index] |> emplace <| [[%ValueType]]
        unsafe
            return values[index][lastIndex]

def operator [] ( self:%BucketHashMap ==const explicit; key : %KeyType ) : %ValueType const&
    with self
        let hash = hash(key)
        var index = self |> find_index(hash)
        for k,v,h in keys[index], values[index], hashes[index]
            if h == hash && k == key
                unsafe
                    return v
        panic("key not found")
        unsafe
            return values[0][0] // we never get here due to panic

def operator ?[] ( var self:%BucketHashMap ==const explicit; key : %KeyType ) : %ValueType?
    with self
        let hash = hash(key)
        let index = self |> find_index(hash)
        for k,v,h in keys[index], values[index], hashes[index]
            if h == hash && k == key
                return unsafe(addr(v))
        return null

def operator ?[] ( self:%BucketHashMap ==const explicit; key : %KeyType ) : %ValueType const?
    with self
        let hash = hash(key)
        let index = self |> find_index(hash)
        for k,v,h in keys[index], values[index], hashes[index]
            if h == hash && k == key
                return unsafe(addr(v))
        return null
%%
