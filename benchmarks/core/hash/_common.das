options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

// Shared helpers for hash map benchmarks.
// Provides deterministic random number generation and shuffling utilities
// used across multiple hash benchmark files.

module _common shared public

require daslib/random
require math

let public TOTAL_RANDOM_NUMBERS = 1000000

// Generate an array of `kNumberOfItems` unique positive random integers
// using a fixed seed for reproducibility.
def public generate_unique_positive_random_numbers(kNumberOfItems : int = TOTAL_RANDOM_NUMBERS) {
    var data : array<int>
    data |> reserve(kNumberOfItems)
    var seed = random_seed(13)
    var set : table<int>
    for (r in range(kNumberOfItems)) {
        var n = random_big_int(seed)
        while (set |> key_exists(n)) {
            n = random_big_int(seed)
        }
        set |> insert(n)
        data |> push(n)
    }
    return <- data
}

// Fisher-Yates shuffle with a fixed seed.
def public shuffle(var data : array<int>; var seed : int4&) {
    let len = length(data)
    for (i in range(len)) {
        let j : int = abs(random_int(seed) % len)
        var temp <- data[i]
        data[i] <- data[j]
        data[j] <- temp
    }
}

// Generate random numbers where `1/factor` of entries are duplicated from
// later positions, then shuffled. Used to create controlled collision rates.
def public get_random_numbers_with_intersections(factor : int) {
    var data <- generate_unique_positive_random_numbers()
    let cutoff = length(data) / factor
    var j = cutoff
    for (i in range(cutoff)) {
        data[i] <- data[j]
        j ++
    }
    var seed = random_seed(13)
    shuffle(data, seed)
    return <- data
}

// 10% of the resulting data set has duplicate keys.
def public get_random_numbers_with_intersections_10 {
    return <- get_random_numbers_with_intersections(10)
}

// 50% of the resulting data set has duplicate keys.
def public get_random_numbers_with_intersections_50 {
    return <- get_random_numbers_with_intersections(2)
}
