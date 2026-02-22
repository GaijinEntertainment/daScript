# Test Suite Index

> **Keep up to date:** When adding, removing, or renaming test files under `tests/`, update this file. See `skills/writing_tests.md` for the rule.

Every `.das` file in this directory tree is listed below, grouped by subdirectory. Files marked **expect** use `expect` directives and are expected to produce specific compile errors. Helper/module files that are not standalone tests are marked *(helper)*.

## algorithm/

| File | Description | Expects errors |
|---|---|---|
| sets.das | Set operations — intersection, union, difference | |
| test_algorithm.das | unique, sort, erase, find, and other core algorithms | |
| test_algorithm_new.das | upper_bound, lower_bound, custom comparators | |
| test_algorithm_noncopyable.das | Algorithm operations on non-copyable structs (erase_all, sort, swap) | |
| topological_sort.das | Topological sort on dependency graphs | |

## apply/

| File | Description | Expects errors |
|---|---|---|
| test_apply.das | `apply` macro on variants, structs, nested types | |

## archive/

| File | Description | Expects errors |
|---|---|---|
| test_archive.das | mem_archive_save/load for variants, tables, arrays, structs | |

## async/

| File | Description | Expects errors |
|---|---|---|
| test_async_await.das | let/var await, copy/move/clone assign await, void await, nested await, await in loops | |
| test_async_basic.das | `[async]` annotation, void/typed async, await_next_frame, yield, early return | |
| test_async_coroutine.das | async/coroutine interop — mixing `[async]` with `[coroutine]`, awaiting coroutines | |
| test_async_iter.das | Iterating async generators — for-loop over typed async, zip with count() | |
| test_async_run.das | async_run, async_run_all, async_timeout, async_race | |

## base64/

| File | Description | Expects errors |
|---|---|---|
| test_base64.das | base64 encode/decode with fuzzing | |

## bitfields/

| File | Description | Expects errors |
|---|---|---|
| test_bitfield_boost.das | bitfield_boost — indexing, each_bit, &&= operator | |

## bool_array/

| File | Description | Expects errors |
|---|---|---|
| test_bool_array.das | BoolArray — push, index, clear, resize, iteration, operators | |

## constexpr/

| File | Description | Expects errors |
|---|---|---|
| test_constexpr_fail.das | `[constexpr]` annotation rejects non-constant arguments | **expect** `40102:1` |

## data_walker/

| File | Description | Expects errors |
|---|---|---|
| dw_common.das | *(helper)* Shared module — LogWalker class and test data types | |
| test_walk_containers.das | DapiDataWalker on arrays, dim, tables | |
| test_walk_edge_cases.das | Null/non-null pointers, lambdas, edge values, deeply nested data | |
| test_walk_filtering.das | canVisitStructure, canVisitArray, canVisitArrayData filtering | |
| test_walk_mutation.das | In-place mutation via walker callbacks | |
| test_walk_scalars.das | Walking scalar types — int, uint, float, double, bool, string, enum | |
| test_walk_structs.das | Walking structures, classes, nested structs, field-last flags | |
| test_walk_tuples_variants.das | Walking tuples and variants | |
| test_walk_vectors_ranges.das | Walking int2/3/4, float2/3/4, range, urange, range64 | |

## debug/

| File | Description | Expects errors |
|---|---|---|
| deval.das | debug_eval — expression evaluation in debug context | |

## debug_agent/

| File | Description | Expects errors |
|---|---|---|
| test_invoke_in_context.das | invoke_in_context with `[pinvoke]` — cross-context function invocation | |
| test_invoke_method.das | invoke_debug_agent_method with various argument counts | |
| test_lifecycle.das | Debug agent lifecycle — create, check, context access | |
| test_on_log.das | to_log/print triggering onLog on debug agents | |
| test_state_collection.das | State collection via onCollect and onVariable | |
| test_threadlock.das | threadlock_context option propagation | |

## decs/

| File | Description | Expects errors |
|---|---|---|
| dim_test.das | decs_template with fixed-size array (dim) components | |
| empty_query.das | Empty decs query and stage function | |
| failed_query_eid_ro.das | Read-only query on const component fails | **expect** `30303` |
| failed_test_arguments.das | Invalid query arguments (optional with defaults, & + const) | **expect** `40104:4` `30103:2` |
| failed_test_empty_template.das | Empty decs_template struct is rejected | **expect** `30111` |
| test_archetype.das | Archetype creation, lookup, migration | |
| test_arrays.das | Arrays as decs components — iterate, modify | |
| test_boundaries.das | commit/restart inside query (expected failures) | |
| test_commit_deferred.das | Deferred semantics — entities not visible until commit | |
| test_component_map.das | ComponentMap set/get/has/remove | |
| test_create_update_remove.das | Entity create, update, remove lifecycle | |
| test_default_values.das | decs_template with default values | |
| test_entity_lifecycle.das | Entity ID uniqueness, validity, invalidation after delete | |
| test_gc.das | decs with GC — archive arrays, strings, gc_collect | |
| test_live.das | Live reload — go_live, recompile, invoke_live | |
| test_live_app.das | *(helper)* App for test_live — initialize, act, shutdown | |
| test_objects.das | Creating objects/entities with set and clone syntax | |
| test_optional_values.das | `@optional` query arguments | |
| test_queries.das | Nested queries, by-eid, with different types | |
| test_queries_comprehensive.das | Query across archetypes, mutation, const-correctness | |
| test_query_eid_rw.das | Read-write query with eid parameter | |
| test_serialize.das | decs serialization — save/load state | |
| test_stages.das | decs_stage — update stage with velocity | |
| test_stages_comprehensive.das | Physics/scoring stages, REQUIRE filtering | |
| test_stages_extra.das | *(helper)* Extra stage functions — make_invisible, check_visible | |
| test_templates_comprehensive.das | decs_template — default prefix, custom prefix, nested templates | |
| test_update_entity_comprehensive.das | update_entity — modify, add/remove components, archetype migration | |
| test_utility_functions.das | is_alive, INVALID_ENTITY_ID, entity counting | |

## dynamic_cast_rtti/

| File | Description | Expects errors |
|---|---|---|
| instance_and_cast.das | is_instance_of and dynamic_cast_rtti | |

## fio/

| File | Description | Expects errors |
|---|---|---|
| fio_file.das | File I/O — fopen, fread, fwrite with fuzzing | |

## functional/

| File | Description | Expects errors |
|---|---|---|
| test_functional.das | flatten on nested iterators | |
| test_functional_edge.das | Edge cases — map, filter, reduce, zip, scan, take_while, skip_while, etc. | |

## handle_types/

| File | Description | Expects errors |
|---|---|---|
| handle_vector.das | Point3Array handle type — indexing, each, custom operator [] | |

## hash_map/

| File | Description | Expects errors |
|---|---|---|
| all_hash_table.das | flat_hash_table and cuckoo_hash_table — dispatch, generics | |

## interfaces/

| File | Description | Expects errors |
|---|---|---|
| test_inheritance.das | Interface inheritance — IAnimal → IPet → IGuideDog | |
| test_interfaces.das | `[interface]` macro — ITick, ILogger, IEmpty, `[implements]` | |
| test_missing_inherited.das | Completeness check catches missing inherited abstract methods | **expect** `30111` |
| test_missing_method.das | Completeness check catches missing abstract methods | **expect** `30111` |

## jit_tests/

46 files testing JIT compilation code generation. None have `expect` directives.

| File | Description | Expects errors |
|---|---|---|
| addr.das | Address operations | |
| arithmetics.das | Arithmetic operations with fuzzing | |
| array.das | Array operations | |
| assert.das | Assert behavior | |
| bit_arithmetics.das | Bitwise arithmetic | |
| bitfield.das | Bitfield operations | |
| block.das | Block invocation | |
| bool_arithmetics.das | Boolean arithmetic | |
| chain_invoke_method.das | Chained method invocation | |
| cmres.das | CMRes (call-me-result) return convention | |
| compare.das | Comparison operators | |
| dim.das | Fixed-size arrays (dim) | |
| dim2d.das | 2D fixed arrays | |
| extern_call.das | External function calls | |
| finally.das | Finally blocks | |
| for_loop.das | For loop codegen | |
| generator.das | Generator codegen | |
| global_variables.das | Global variable access | |
| global_variables_solid.das | Solid global variables | |
| handle_types.das | Handle type operations | |
| interop_call.das | Interop calls | |
| intrinsics.das | Intrinsic functions | |
| invoke.das | Invoke operations | |
| labels.das | Label/goto codegen | |
| memset.das | Memset operations | |
| move.das | Move semantics | |
| new_ascend_and_delete.das | New/delete heap operations | |
| phi.das | Phi node (SSA) codegen | |
| pointer.das | Pointer operations | |
| range64.das | 64-bit range operations | |
| ref.das | Reference semantics | |
| safe_navigation.das | Safe navigation (?.) | |
| string_builder.das | String builder operations | |
| string_op.das | String operations | |
| struct.das | Struct operations | |
| table.das | Table operations | |
| table_value_key.das | Table value/key access | |
| try_recover.das | Try/recover codegen | |
| tuple.das | Tuple operations | |
| type_constructors.das | Type constructor codegen | |
| typeinfo.das | Typeinfo intrinsics | |
| using_and_placement_new.das | Using/placement new | |
| variant.das | Variant operations | |
| vector_navigation.das | Vector field navigation | |
| vector_swizzle.das | Vector swizzle operations | |
| while_loop.das | While loop codegen | |

## jobque/

| File | Description | Expects errors |
|---|---|---|
| atomics.das | Atomic32/atomic64 set/get/inc/dec | |
| test_jobque_atomics.das | with_atomic32/atomic64 — set, get, inc, dec, initial value | |
| test_jobque_channels.das | Channel boost — push_clone, for_each_clone, with_channel | |
| test_jobque_edge.das | Channel edge cases — single-item, large batch | |
| test_jobque_jobs.das | JobStatus, new_job with channels and messages | |
| test_jobque_lockbox.das | LockBox basics — create, destroy, validity | |
| test_jobque_parallel.das | parallel_for, parallel work distribution | |
| test_jobque_try_pop.das | try_pop_clone — non-blocking pop, empty/data/drain | |
| test_jobque_wait_group.das | with_wait_group — convenience wrapper for job completion | |

## json/

| File | Description | Expects errors |
|---|---|---|
| broken.das | Parsing/round-tripping JSON with emoji and long strings | |
| safe.das | Safe JSON operators — ?[], ?., ?? fallback | |
| test_json_edge.das | JSON edge cases — parsing scalars, arrays, nested, whitespace, error recovery | |
| test_sprint_json.das | sprint_json — basic types, structs, arrays, tables, variants, annotations, RTTI | |
| types.das | JV/from_JV JSON serialization for all basic/vector/struct types | |

## language/

| File | Description | Expects errors |
|---|---|---|
| _glob.das | *(helper)* Shared module defining `AAA = 10` | |
| _operators_derived.das | *(helper)* Derived class BarOp | |
| _operators_parent.das | *(helper)* Parent class FooOp with property operator | |
| aka.das | `aka` variable aliasing in various contexts | |
| aliasing.das | Aliasing behavior with `no_aliasing = false` | |
| ascend_ctor.das | `new` with class constructors and fields | |
| assume.das | `assume` for deep field access | |
| bitfields.das | Bitfield operations — 32-bit and 64-bit, operators, `typeinfo` | |
| block.das | Block creation and invocation | |
| capture_string.das | String capture in lambdas and string builder (gen2=false, module) | |
| containers.das | Temporary container operations (with containers.inc) | |
| containers_failed.das | Container compile-time failures | **expect** `30304:4` |
| contracts.das | `daslib/contracts` — expect_dim, expect_any_tuple, expect_any_variant | |
| coroutines.das | Typed coroutines — count_up, count_down, yield, co_continue | |
| default_method_arguments.das | Default argument values in abstract/override methods | |
| dynamic_type_checking.das | dynamic_cast, is_instance_of with class hierarchies | |
| enumerations.das | Enum conversion — to/from string, enum_trait | |
| failed_aka.das | Global aka and typedef aka produce errors | **expect** `20000:1` |
| failed_aliasing.das | Aliasing errors with `no_aliasing` option | **expect** `40211:23` `40212:3` |
| failed_block.das | Block variable initialization failures | **expect** `30108` `30113` |
| failed_capture_self.das | Capturing `self` in lambda fails | **expect** `30508` `30124` |
| failed_constants.das | Out-of-range numeric literal errors | **expect** `10006:12` `10010:4` |
| global_variable_order.das | Global variable initialization ordering | **expect** `30305:1` |
| global_variable_order_itself.das | Global variable self-initialization error | **expect** `30305:1` |
| hash.das | Hash function for various types | |
| init_order.das | `[init]` function ordering with before/tag | |
| intrinsics.das | clz, ctz, popcnt for u32/u64 with fuzzing | |
| invalid_types.das | Oversized types and arguments | **expect** `30101:2` `30108:4` `30109:2` |
| lambda_capture.das | Lambda capturing const values, finalizer behavior | |
| local_classes_failed.das | no_local_class_members restriction | **expect** `31300:1` |
| memset.das | memset8, memset16, memset32, memset64, memset128 | |
| move_and_return_move.das | Move semantics — self-move, function move, struct with arrays | |
| named_call.das | Named arguments — reordering, skipping, defaults, error cases | **expect** `30304:12` `30101:1` `30507:1` |
| no_default_initializer.das | `[no_default_initializer]` annotation | |
| no_init.das | `options no_init` preventing `[init]` functions | **expect** `40214:3` |
| operators.das | Custom operators — `as`, `?as`, derived class operators | |
| option_type.das | Option types (int& \| auto) ref preservation | |
| pointers.das | Pointer operations — new, deref, safe navigation, null checks | |
| properties.das | Property operators (.res :=, getter/setter) | |
| random_numbers.das | Random seeding, distributions, reproducibility | |
| resize_locked.das | Locked array operations — resize-while-iterating protection | |
| serialization.das | Archive serialization — structs, custom serialize, arrays, tables | |
| static.das | Static class members and methods | |
| strict_smart_ptr.das | strict_smart_pointers — emplace, inscope, smart_ptr operations | |
| table.das | Table tombstone handling and iteration | |
| testing_tools.das | Faker, fuzzer, testing_boost tools | |
| tuple_expansion.das | Tuple expansion — `let (i, s, f) = foo()` | |
| typefunction.das | `[type_function]` annotation (gen2=false) | |
| typeinfo.das | typeinfo sizeof, has_field, struct_get_annotation_argument | |
| typeinfo_traits.das | typeinfo trait queries — is_local, is_ref, is_numeric, etc. | |
| utility_patterns.das | defer, static_let utility patterns | |
| variants.das | Variant type — construction, match, access | |
| vector_fields.das | float4 .r/.g/.b/.a fields and swizzle | |

## linq/

| File | Description | Expects errors |
|---|---|---|
| _common.das | *(helper)* Shared module — ComplexType, Person, Pet test data | |
| test_linq.das | to_table, core LINQ operations | |
| test_linq_aggregation.das | count, long_count, sum, average, min, max | |
| test_linq_bugs.das | LINQ bug fixes — unique_inplace empty, edge cases | |
| test_linq_concat.das | append, prepend, concat operations | |
| test_linq_element.das | element_at, first, last, single | |
| test_linq_fold.das | Comprehension fold, method chaining syntax | |
| test_linq_from_decs.das | LINQ querying decs entities | |
| test_linq_generation.das | default_empty, repeat, range generation | |
| test_linq_join.das | join, group_join operations | |
| test_linq_partition.das | skip, take, skip_while, take_while | |
| test_linq_querying.das | any, all, contains, where | |
| test_linq_set.das | distinct, union, intersect, except | |
| test_linq_sorting.das | reverse, order_by, order_by_descending | |
| test_linq_transform.das | select_transform, select_many, zip, cast | |

## lpipe/

| File | Description | Expects errors |
|---|---|---|
| test_pipes.das | lpipe macro — pipe into function calls, chain operators | |

## match/

| File | Description | Expects errors |
|---|---|---|
| all_matches.das | match/multi_match/static_match on enums, variants, structs, tuples | |
| test_match_edge.das | Match edge cases — nested, empty struct, wildcard, bindings, guards | |

## math/

| File | Description | Expects errors |
|---|---|---|
| fake_numeric.das | *(helper)* Macro module — FakeNumericMacro for comprehensive faker tests | |
| inf_and_nan.das | Infinity and NaN comparisons | |
| math_matrix.das | Matrix operations — multiply, equality, transpose with fuzzing | |
| math_numeric.das | Numeric math functions (sin, cos, tan, etc.) via fake_numeric macro | |
| math_pack_unpack.das | pack_float_to_byte / unpack_byte_to_float with fuzzing | |
| math_quaternions.das | Quaternion operations with fuzzing | |

## regex/

| File | Description | Expects errors |
|---|---|---|
| test_regex_api.das | regex_match API — match, offset, empty string | |
| test_regex_basic.das | Basic regex — literal, dot, star, plus, question mark | |
| test_regex_charclass.das | Regex character classes — \w, \d, \s, negation, ranges | |
| test_regex_edge.das | Regex edge cases — offset parameter, greedy/lazy, empty pattern | |
| test_regex_newfeatures.das | Regex new features — ^, $, anchors, grouping | |
| test_regex_phase2.das | regex_search, regex_find_all, regex_replace | |
| test_regex_phase3.das | Dot-newline, regex_replace with backreferences | |
| test_strings_reexport.das | `require daslib/regex` re-exports strings module functions | |

## safe_addr/

| File | Description | Expects errors |
|---|---|---|
| test_shared_addr.das | shared_addr — address of shared globals, iteration | |

## soa/

| File | Description | Expects errors |
|---|---|---|
| test_soa_basic.das | `[soa]` struct — creation, push, length, field access | |
| test_soa_container.das | SOA container operations — erase, pop, resize, reserve, clear | |
| test_soa_iteration.das | SOA iteration — for loop, index access | |
| test_soa_noncopyable.das | SOA with non-copyable fields (array&lt;int&gt;) — push, erase, iterate | |

## spoof/

| File | Description | Expects errors |
|---|---|---|
| basic_spoof.das | spoof_template — value substitution (number, literal, expression, string) | |
| spoof_linked_list.das | TLinkedList spoof template — linked list iteration | |

## strings/

| File | Description | Expects errors |
|---|---|---|
| delete_strings.das | delete_string — const heap string, heap string, empty string | |
| delete_strings_persistent.das | Same as delete_strings but with persistent_heap option | |
| strings_as_array.das | peek_data, modify_data — string as uint8 array | |
| strings_boost_split_and_join.das | split, join, and related boost functions with fuzzing | |
| strings_builder.das | String builder — write, write_char with fuzzing | |
| strings_charset.das | Charset operations — is_char_in_set, set_total, set_element | |
| strings_convert.das | Numeric string conversions — int, float, out-of-range, errors | |
| strings_distance.das | levenshtein_distance with fuzzing | |
| strings_hash.das | Hash on das_string, hash_builder, builder vs direct hash | |
| strings_levenshtein.das | levenshtein_distance_fast with fuzzing | |
| strings_modifications.das | repeat, strip, replace, escape/unescape with fuzzing | |
| strings_properties.das | length, empty, starts_with, ends_with with fuzzing | |
| strings_replace_multiple.das | replace_multiple — multiple replacements, special chars, empty | |
| strings_search.das | find (string and char) with fuzzing | |
| strings_traits.das | is_alpha, is_new_line, is_white_space, is_number with fuzzing | |
| temporary_intern_strings.das | temp_string with intern_strings option | |
| temporary_strings.das | temp_string, build_temp_string — no heap allocations | |
| temporary_strings_failed.das | temp_string rejects variables and function calls | **expect** `40102:3` |
| test_bug_fixes.das | C++ bug fixes — chop bounds checking, edge cases | |
| test_cpp_functions.das | C++ string functions — fmt, safe_unescape, to_lower_in_place, character_at | |
| test_new_string_functions.das | New string utilities — contains, count_chars, pad_left/right, trim_chars | |
| test_strings_boost_extra.das | Extra strings_boost — wide, is_character_at, eq, join overloads | |

## type_traits/

| File | Description | Expects errors |
|---|---|---|
| test_bitfields.das | bitfield_trait — each_bit_name iteration | |
| test_traits.das | type_traits — fields_count for struct/derived struct | |

## unsafe/

| File | Description | Expects errors |
|---|---|---|
| unsafe_reference.das | Local reference to non-local expression is unsafe | **expect** `31300` |

## uri/

| File | Description | Expects errors |
|---|---|---|
| test_uri.das | URI parsing, normalize, rebase, query params, edge cases | |

## verify/

| File | Description | Expects errors |
|---|---|---|
| test_verify_completion.das | `[verify_completion]` — rejects unbounded loops, recursion, keys() | **expect** `40104:8` |

---

## Summary

- **35** test directories
- **226** `.das` files total (~13 helper/module files, ~213 test files)
- **21** files with `expect` directives (expected compile errors)
