
.. _stdlib_fuzzer:

======
Fuzzer
======

.. include:: detail/fuzzer.rst

The FUZZER module implements facilities for the fuzz testing.

The idea behind the fuzz testing is to feed random data to the testing function and see if it crashes.
`panic` is considered a valid behavior, and in fact ignored.
Fuzz tests work really well in combination with the sanitizers (asan, ubsan, etc).

All functions and symbols are in "fuzzer" module, use require to get access to it. ::

    require daslib/fuzzer

++++++++++++
Fuzzer tests
++++++++++++

  *  :ref:`fuzz (blk:block\<\> const) : void <function-_at_fuzzer_c__c_fuzz_C_builtin_>` 
  *  :ref:`fuzz (fuzz_count:int const;blk:block\<\> const) : void <function-_at_fuzzer_c__c_fuzz_Ci_C_builtin_>` 
  *  :ref:`fuzz_debug (blk:block\<\> const) : void <function-_at_fuzzer_c__c_fuzz_debug_C_builtin_>` 
  *  :ref:`fuzz_debug (fuzz_count:int const;blk:block\<\> const) : void <function-_at_fuzzer_c__c_fuzz_debug_Ci_C_builtin_>` 

.. _function-_at_fuzzer_c__c_fuzz_C_builtin_:

.. das:function:: fuzz(blk: block<> const)

+--------+-------------+
+argument+argument type+
+========+=============+
+blk     +block<> const+
+--------+-------------+


run block however many times
ignore panic, so that we can see that runtime crashes

.. _function-_at_fuzzer_c__c_fuzz_Ci_C_builtin_:

.. das:function:: fuzz(fuzz_count: int const; blk: block<> const)

+----------+-------------+
+argument  +argument type+
+==========+=============+
+fuzz_count+int const    +
+----------+-------------+
+blk       +block<> const+
+----------+-------------+


run block however many times
ignore panic, so that we can see that runtime crashes

.. _function-_at_fuzzer_c__c_fuzz_debug_C_builtin_:

.. das:function:: fuzz_debug(blk: block<> const)

+--------+-------------+
+argument+argument type+
+========+=============+
+blk     +block<> const+
+--------+-------------+


run block however many times
do not ignore panic, so that we can see where the runtime fails
this is here so that `fuzz` can be easily replaced with `fuzz_debug` for the purpose of debugging

.. _function-_at_fuzzer_c__c_fuzz_debug_Ci_C_builtin_:

.. das:function:: fuzz_debug(fuzz_count: int const; blk: block<> const)

+----------+-------------+
+argument  +argument type+
+==========+=============+
+fuzz_count+int const    +
+----------+-------------+
+blk       +block<> const+
+----------+-------------+


run block however many times
do not ignore panic, so that we can see where the runtime fails
this is here so that `fuzz` can be easily replaced with `fuzz_debug` for the purpose of debugging


