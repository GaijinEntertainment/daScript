
.. _stdlib_jobque_boost:

==================================
Boost package for jobs and threads
==================================

.. include:: detail/jobque_boost.rst

|module-jobque_boost|

+++++++
Classes
+++++++

.. _struct-jobque_boost-NewJobMacro:

.. das:attribute:: NewJobMacro : AstFunctionAnnotation

|class-jobque_boost-NewJobMacro|

.. das:function:: NewJobMacro.transform(self: AstFunctionAnnotation; call: smart_ptr<ast::ExprCallFunc>; errors: das_string)

transform returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+----------------------------------------------------------------------+
+argument+argument type                                                         +
+========+======================================================================+
+self    + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>` +
+--------+----------------------------------------------------------------------+
+call    +smart_ptr< :ref:`ast::ExprCallFunc <handle-ast-ExprCallFunc>` >       +
+--------+----------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`               +
+--------+----------------------------------------------------------------------+


|method-jobque_boost-NewJobMacro.transform|

.. das:function:: NewJobMacro.apply(self: AstFunctionAnnotation; func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

apply returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>`           +
+--------+--------------------------------------------------------------------------------+
+func    + :ref:`FunctionPtr <alias-FunctionPtr>`                                         +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-jobque_boost-NewJobMacro.apply|

.. das:function:: NewJobMacro.finish(self: AstFunctionAnnotation; func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList const; progArgs: AnnotationArgumentList const; errors: das_string)

finish returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>`           +
+--------+--------------------------------------------------------------------------------+
+func    + :ref:`FunctionPtr <alias-FunctionPtr>`                                         +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`rtti::ModuleGroup <handle-rtti-ModuleGroup>`                             +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+progArgs+ :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-jobque_boost-NewJobMacro.finish|

.. das:function:: NewJobMacro.isCompatible(self: AstFunctionAnnotation; func: FunctionPtr; types: VectorTypeDeclPtr; decl: AnnotationDeclaration const; errors: das_string)

isCompatible returns bool

+--------+------------------------------------------------------------------------------+
+argument+argument type                                                                 +
+========+==============================================================================+
+self    + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>`         +
+--------+------------------------------------------------------------------------------+
+func    + :ref:`FunctionPtr <alias-FunctionPtr>`                                       +
+--------+------------------------------------------------------------------------------+
+types   + :ref:`VectorTypeDeclPtr <alias-VectorTypeDeclPtr>`                           +
+--------+------------------------------------------------------------------------------+
+decl    + :ref:`rtti::AnnotationDeclaration <handle-rtti-AnnotationDeclaration>`  const+
+--------+------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                       +
+--------+------------------------------------------------------------------------------+


|method-jobque_boost-NewJobMacro.isCompatible|

.. das:function:: NewJobMacro.isSpecialized(self: AstFunctionAnnotation)

isSpecialized returns bool

|method-jobque_boost-NewJobMacro.isSpecialized|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_jobque_boost_c__c_new_job__hh_lambda:

.. das:function:: new_job(l: lambda<>)

+--------+-------------+
+argument+argument type+
+========+=============+
+l       +lambda<>     +
+--------+-------------+


|function-jobque_boost-new_job|

.. _function-_at_jobque_boost_c__c_new_thread__hh_lambda:

.. das:function:: new_thread(l: lambda<>)

+--------+-------------+
+argument+argument type+
+========+=============+
+l       +lambda<>     +
+--------+-------------+


|function-jobque_boost-new_thread|

.. _function-_at_jobque_boost_c__c_each__hh_const_hh__hh_ptr_hh__hh_handle_hh_Channel__hh_const_hh__hh_auto_hh_TT:

.. das:function:: each(channel: jobque::Channel? const; tinfo: auto(TT) const)

each returns auto

+--------+-------------------------------------------------------+
+argument+argument type                                          +
+========+=======================================================+
+channel + :ref:`jobque::Channel <handle-jobque-Channel>` ? const+
+--------+-------------------------------------------------------+
+tinfo   +auto(TT) const                                         +
+--------+-------------------------------------------------------+


|function-jobque_boost-each|

.. _function-_at_jobque_boost_c__c_for_each__hh_const_hh__hh_ptr_hh__hh_handle_hh_Channel__hh_const_hh__hh_block_hh__hh_const_hh__hh_temporary_hh__hh_auto_hh_TT_hh__rq_res_hh__c_void:

.. das:function:: for_each(channel: jobque::Channel? const; blk: block<(res:auto(TT) const#):void> const)

for_each returns auto

+--------+-------------------------------------------------------+
+argument+argument type                                          +
+========+=======================================================+
+channel + :ref:`jobque::Channel <handle-jobque-Channel>` ? const+
+--------+-------------------------------------------------------+
+blk     +block<(res:auto(TT) const#):void> const                +
+--------+-------------------------------------------------------+


|function-jobque_boost-for_each|

.. _function-_at_jobque_boost_c__c_push__hh_const_hh__hh_ptr_hh__hh_handle_hh_Channel__hh_const_hh__hh_ptr_hh__hh_auto:

.. das:function:: push(channel: jobque::Channel? const; data: auto? const)

push returns auto

+--------+-------------------------------------------------------+
+argument+argument type                                          +
+========+=======================================================+
+channel + :ref:`jobque::Channel <handle-jobque-Channel>` ? const+
+--------+-------------------------------------------------------+
+data    +auto? const                                            +
+--------+-------------------------------------------------------+


|function-jobque_boost-push|

.. _function-_at_jobque_boost_c__c_push_clone__hh_const_hh__hh_ptr_hh__hh_handle_hh_Channel__hh_const_hh__hh_auto_hh_TT:

.. das:function:: push_clone(channel: jobque::Channel? const; data: auto(TT) const)

push_clone returns auto

+--------+-------------------------------------------------------+
+argument+argument type                                          +
+========+=======================================================+
+channel + :ref:`jobque::Channel <handle-jobque-Channel>` ? const+
+--------+-------------------------------------------------------+
+data    +auto(TT) const                                         +
+--------+-------------------------------------------------------+


|function-jobque_boost-push_clone|


