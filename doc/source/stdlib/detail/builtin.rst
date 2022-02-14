.. |typedef-builtin-print_flags| replace:: to be documented

.. |function-builtin-breakpoint| replace:: breakpoint will call os_debugbreakpoint, which is link-time unresolved dependency. It's supposed to call breakpoint in debugger tool, as sample implementation does.

.. |function-builtin-builtin_get_command_line_arguments| replace:: to be documented

.. |function-builtin-capacity| replace:: capacity will return current capacity of table or array `arg`. Capacity is the count of elements, allocating (or pushing) until that size won't cause reallocating dynamic heap.

.. |function-builtin-clear| replace:: clear will clear whole table or array `arg`. The size of `arg` after clear is 0.

.. |function-builtin-clone| replace:: to be documented

.. |function-builtin-clone_string| replace:: to be documented

.. |function-builtin-collect_profile_info| replace:: to be documented

.. |function-builtin-dump_profile_info| replace:: to be documented

.. |function-builtin-empty| replace:: to be documented

.. |function-builtin-gc0_reset| replace:: to be documented

.. |function-builtin-gc0_restore_ptr| replace:: to be documented

.. |function-builtin-gc0_restore_smart_ptr| replace:: to be documented

.. |function-builtin-gc0_save_ptr| replace:: to be documented

.. |function-builtin-gc0_save_smart_ptr| replace:: to be documented

.. |function-builtin-get_clock| replace:: return a current calendar time. The value returned generally represents the number of seconds since 00:00 hours, Jan 1, 1970 UTC (i.e., the current unix timestamp).

.. |function-builtin-get_command_line_arguments| replace:: to be documented

.. |function-builtin-get_das_root| replace:: to be documented

.. |function-builtin-hash| replace:: to be documented

.. |function-builtin-heap_bytes_allocated| replace:: will return bytes allocated on heap (i.e. really used, not reserved)

.. |function-builtin-heap_depth| replace:: to be documented

.. |function-builtin-heap_report| replace:: to be documented

.. |function-builtin-i_das_ptr_add| replace:: to be documented

.. |function-builtin-i_das_ptr_dec| replace:: to be documented

.. |function-builtin-i_das_ptr_diff| replace:: to be documented

.. |function-builtin-i_das_ptr_inc| replace:: to be documented

.. |function-builtin-i_das_ptr_set_add| replace:: to be documented

.. |function-builtin-i_das_ptr_set_sub| replace:: to be documented

.. |function-builtin-i_das_ptr_sub| replace:: to be documented

.. |function-builtin-is_compiling| replace:: to be documented

.. |function-builtin-is_compiling_macros| replace:: to be documented

.. |function-builtin-is_compiling_macros_in_module| replace:: to be documented

.. |function-builtin-length| replace:: length will return current size of table or array `arg`.

.. |function-builtin-memcmp| replace:: to be documented

.. |function-builtin-panic| replace:: will cause panic. The program will be determinated if there is no recover. Panic is not a error handling mechanism and can not be used as such. It is indeed panic, fatal error. It is not supposed that program can completely correctly recover from panic, recover construction is provided so program can try to correcly shut-down or report fatal error. If there is no recover withing script, it will be called in calling eval (in C++ callee code).

.. |function-builtin-peek| replace:: to be documented

.. |function-builtin-print| replace:: outputs string into current context log output

.. |function-builtin-profile| replace:: to be documented

.. |function-builtin-reset_profiler| replace:: to be documented

.. |function-builtin-set| replace:: to be documented

.. |function-builtin-set_variant_index| replace:: to be documented

.. |function-builtin-smart_ptr_clone| replace:: to be documented

.. |function-builtin-smart_ptr_use_count| replace:: to be documented

.. |function-builtin-sprint| replace:: to be documented

.. |function-builtin-stackwalk| replace:: stackwalk prints call stack and local variables values

.. |function-builtin-string_heap_bytes_allocated| replace:: to be documented

.. |function-builtin-string_heap_collect| replace:: to be documented

.. |function-builtin-string_heap_depth| replace:: to be documented

.. |function-builtin-string_heap_report| replace:: to be documented

.. |function-builtin-terminate| replace:: terminates current context execution

.. |function-builtin-variant_index| replace:: to be documented

.. |function-builtin-binary_load| replace:: to be documented

.. |function-builtin-binary_save| replace:: to be documented

.. |function-builtin-clone_dim| replace:: to be documented

.. |function-builtin-clone_to_move| replace:: to be documented

.. |function-builtin-each| replace:: returns iterator, which iterates though each element of the object. object can be range, static or dynamic array, another iterator.

.. |function-builtin-each_enum| replace:: to be documented

.. |function-builtin-each_ref| replace:: to be documented

.. |function-builtin-emplace| replace:: emplace will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be moved (<-) to it.

.. |function-builtin-erase| replace:: erase will erase `at` index element in `arg` array.

.. |function-builtin-finalize| replace:: to be documented

.. |function-builtin-finalize_dim| replace:: to be documented

.. |function-builtin-find| replace:: will execute `block_arg` with argument pointer-to-value in `table_arg` pointing to value indexed by `key`, or null if `key` doesn't exist in `table_arg`.

.. |function-builtin-find_for_edit| replace:: to be documented

.. |function-builtin-find_for_edit_if_exists| replace:: to be documented

.. |function-builtin-find_if_exists| replace:: to be documented

.. |function-builtin-find_index| replace:: to be documented

.. |function-builtin-find_index_if| replace:: to be documented

.. |function-builtin-get_ptr| replace:: to be documented

.. |function-builtin-has_value| replace:: to be documented

.. |function-builtin-intptr| replace:: to be documented

.. |function-builtin-key_exists| replace:: will return true if element `key` exists in table `table_arg`.

.. |function-builtin-keys| replace:: returns iterator to all keys of the table

.. |function-builtin-lock| replace:: to be documented

.. |function-builtin-lock_forever| replace:: to be documented

.. |function-builtin-next| replace:: to be documented

.. |function-builtin-nothing| replace:: to be documented

.. |function-builtin-pop| replace:: to be documented

.. |function-builtin-push| replace:: push will push to dynamic array `array_arg` the content of `value`. `value` has to be of the same type (or const reference to same type) as array values. if `at` is provided `value` will be pushed at index `at`, otherwise to the end of array. The `content` of value will be copied (assigned) to it.

.. |function-builtin-push_clone| replace:: to be documented

.. |function-builtin-reserve| replace:: to be documented

.. |function-builtin-resize| replace:: Resize will resize `array_arg` array to a new size of `new_size`. If new_size is bigger than current, new elements will be zeroed.

.. |function-builtin-sort| replace:: to be documented

.. |function-builtin-to_array| replace:: will convert argument (static array, iterator, another dynamic array) to an array. argument elements will be cloned

.. |function-builtin-to_array_move| replace:: will convert argument (static array, iterator, another dynamic array) to an array. argument elements will be copied or moved

.. |function-builtin-to_table| replace:: will convert an array of key-value tuples into a table<key;value> type. arguments will be cloned

.. |function-builtin-to_table_move| replace:: will convert an array of key-value tuples into a table<key;value> type. arguments will be copied or moved

.. |function-builtin-values| replace:: returns iterator to all values of the table

.. |any_annotation-builtin-clock| replace:: to be documented

.. |any_annotation-builtin-das_string| replace:: to be documented

.. |variable-builtin-DBL_MAX| replace:: to be documented

.. |variable-builtin-DBL_MIN| replace:: to be documented

.. |variable-builtin-FLT_MAX| replace:: to be documented

.. |variable-builtin-FLT_MIN| replace:: to be documented

.. |variable-builtin-INT_MAX| replace:: to be documented

.. |variable-builtin-INT_MIN| replace:: to be documented

.. |variable-builtin-LONG_MAX| replace:: to be documented

.. |variable-builtin-LONG_MIN| replace:: to be documented

.. |variable-builtin-UINT_MAX| replace:: to be documented

.. |variable-builtin-ULONG_MAX| replace:: to be documented

.. |variable-builtin-print_flags_debugger| replace:: to be documented

