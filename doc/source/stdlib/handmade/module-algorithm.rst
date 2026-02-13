The ALGORITHM module provides array and collection manipulation algorithms including
sorting, searching, set operations, and element removal.

All functions and symbols are in "algorithm" module, use require to get access to it. ::

    require daslib/algorithm

Example: ::

    require daslib/algorithm

        [export]
        def main() {
            var arr <- [3, 1, 4, 1, 5, 9, 2, 6, 5]
            sort_unique(arr)
            print("sort_unique: {arr}\n")
            print("has 4: {binary_search(arr, 4)}\n")
            print("has 7: {binary_search(arr, 7)}\n")
            print("lower_bound(4): {lower_bound(arr, 4)}\n")
            reverse(arr)
            print("reversed: {arr}\n")
        }
        // output:
        // sort_unique: [[ 1; 2; 3; 4; 5; 6; 9]]
        // has 4: true
        // has 7: false
        // lower_bound(4): 3
        // reversed: [[ 9; 6; 5; 4; 3; 2; 1]]
