Looks up `at` in the temporary table `Tab` (whose values are fixed-size arrays) and, if found, invokes `blk` with a const handle reference to the array value; returns `true` if the key existed.
