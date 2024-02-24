options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module framework shared public

require daslib/random
require math
require jit

let TEST_REPEAT_COUNT = 1

let TOTAL_RANDOM_NUMBERS = 1000000

def profile_test ( name:string; var hashMap : auto(HT); randomData : auto(TT) ) : void
    if !is_jit_function(@@generate_unique_positive_random_numbers)
        panic("jit is not enabled!!! test is expected to run with jit enabled")
    profile(TEST_REPEAT_COUNT,name) <|
        __::test(hashMap,randomData)

def generate_unique_positive_random_numbers ( kNumberOfItems : int = TOTAL_RANDOM_NUMBERS )
    var data : array<int>
    data |> reserve(kNumberOfItems)
    var seed = random_seed(13)
    var set : table<int>
    for r in range(kNumberOfItems)
        var n = random_big_int(seed)
        while set |> key_exists(n)
            n = random_big_int(seed)
        set |> insert(n)
        data |> push(n)
    return <- data

def get_random_numbers_with_intersections ( factor:int )
    var data <- generate_unique_positive_random_numbers()
    let cutoff = length(data) / factor
    var j = int(cutoff)
    for i in range(cutoff)
        data[i] <- data[j]
        j ++
    var seed = random_seed(13)
    shuffle(data,seed)
    return <- data

// 10% of the resulting data set is intersected
def get_random_numbers_with_intersections_10
    return <- get_random_numbers_with_intersections(10)

// 50% of the resulting data set is intersected
def get_random_numbers_with_intersections_50
    return <- get_random_numbers_with_intersections(2)

def shuffle ( var data : array<int>; var seed:int4& )
    let len = data |> length
    for i in range(len)
        let j = abs(random_int(seed) % len)
        var temp <- data[i]
        data[i] <- data[j]
        data[j] <- temp


// this is good for the jit version of the test
// when measuring the performance with the interpreter, this needs to be commented out

let FNV_PRIME = 1099511628211ul
let FNV_OFFSET_BASIS = 14695981039346656037ul

[hint(alwaysinline)]
def hash ( t : int )
    unsafe
        let data : uint8? = reinterpret<uint8?> addr(t)
        var offset_basis = FNV_OFFSET_BASIS
        offset_basis = ( offset_basis ^ uint64(data[0]) ) * FNV_PRIME
        offset_basis = ( offset_basis ^ uint64(data[1]) ) * FNV_PRIME
        offset_basis = ( offset_basis ^ uint64(data[2]) ) * FNV_PRIME
        offset_basis = ( offset_basis ^ uint64(data[3]) ) * FNV_PRIME
        if offset_basis <= 0x0000FFFFFFFFFFFFul
            return FNV_PRIME
        return offset_basis

[hint(alwaysinline)]
def hash ( t : uint64 )
    unsafe
        let data : uint8? = reinterpret<uint8?> addr(t)
        var offset_basis = FNV_OFFSET_BASIS
        offset_basis = ( offset_basis ^ uint64(data[0]) ) * FNV_PRIME
        offset_basis = ( offset_basis ^ uint64(data[1]) ) * FNV_PRIME
        offset_basis = ( offset_basis ^ uint64(data[2]) ) * FNV_PRIME
        offset_basis = ( offset_basis ^ uint64(data[3]) ) * FNV_PRIME
        offset_basis = ( offset_basis ^ uint64(data[4]) ) * FNV_PRIME
        offset_basis = ( offset_basis ^ uint64(data[5]) ) * FNV_PRIME
        offset_basis = ( offset_basis ^ uint64(data[6]) ) * FNV_PRIME
        offset_basis = ( offset_basis ^ uint64(data[7]) ) * FNV_PRIME
        if offset_basis <= 0x0000FFFFFFFFFFFFul
            return FNV_PRIME
        return offset_basis
