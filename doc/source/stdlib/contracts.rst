
.. _stdlib_contracts:

=================================
Miscelanious contract annotations
=================================

.. include:: detail/contracts.rst

The CONTRACTS module exposes collection of type matching contracts.

All functions and symbols are in "contracts" module, use require to get access to it. ::

    require daslib/contracts


++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-contracts-expect_any_array:

.. das:attribute:: expect_any_array

[expect_any_array(argname)] contract, which only accepts array<T>, T[], or das`vector<T>

.. _handle-contracts-expect_dim:

.. das:attribute:: expect_dim

[expect_dim(argname)] contract, which only accepts T[]

.. _handle-contracts-expect_not_dim:

.. das:attribute:: expect_not_dim

[expect_not_dim(argname)] contract, which never accepts T[]

.. _handle-contracts-expect_any_enum:

.. das:attribute:: expect_any_enum

[expect_any_enum(argname)] contract, which only accepts enumerations

.. _handle-contracts-expect_any_vector_type:

.. das:attribute:: expect_any_vector_type

[expect_any_vector_type(argname)] contract, which only accepts vector types, i.e. int2, float3, range, etc

.. _handle-contracts-expect_any_struct:

.. das:attribute:: expect_any_struct

[expect_any_struct(argname)] contract, which only accepts structs (byt not classes)

.. _handle-contracts-expect_any_numeric:

.. das:attribute:: expect_any_numeric

[expect_any_numeric(argname)] contract, which only accepts numeric types (int, float, etc)

.. _handle-contracts-expect_any_workhorse:

.. das:attribute:: expect_any_workhorse

[expect_any_workhorse(argname)] contract, which only accepts workhorse types (int, float, etc)
Workhorse types are: bool,int*,uint*,float*,double,range and urange,string,enumeration,and non-smart pointers

.. _handle-contracts-expect_any_workhorse_raw:

.. das:attribute:: expect_any_workhorse_raw

[expect_any_workhorse_raw(argname)] contract, which only accepts workhorse types which are raw (not pointer or bool)

.. _handle-contracts-expect_any_tuple:

.. das:attribute:: expect_any_tuple

[expect_any_tuple(argname)] contract, which only accepts tuples

.. _handle-contracts-expect_any_variant:

.. das:attribute:: expect_any_variant

[expected_any_variant(argname)] contract, which only accepts variants

.. _handle-contracts-expect_any_function:

.. das:attribute:: expect_any_function

[expect_any_function(argname)] contract, which only accepts functions

++++++++++++
Type queries
++++++++++++

  *  :ref:`isYetAnotherVectorTemplate (td:smart_ptr\<ast::TypeDecl\> -const) : bool <function-_at_contracts_c__c_isYetAnotherVectorTemplate_Y_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr_?M>` 

.. _function-_at_contracts_c__c_isYetAnotherVectorTemplate_Y_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr_?M:

.. das:function:: isYetAnotherVectorTemplate(td: TypeDeclPtr)

isYetAnotherVectorTemplate returns bool

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+td      + :ref:`TypeDeclPtr <alias-TypeDeclPtr>` +
+--------+----------------------------------------+


returns true if the given type declaration is a das::vector template bound on C++ side


