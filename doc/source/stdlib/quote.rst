
.. _stdlib_quote:

===============================
AST quasiquoting infrastructure
===============================

The QUOTE module provides quasiquotation support for AST construction.
It allows building AST nodes using daScript syntax with ``$``-prefixed
splice points for inserting computed values, making macro writing more
readable and less error-prone than manual AST construction.

All functions and symbols are in "quote" module, use require to get access to it. ::

    require daslib/quote

+++++++++++++
Uncategorized
+++++++++++++

.. _function-quote_clone_dasvector_rq_CaptureEntry_array_ls_CaptureEntryInitData_gr_:

.. das:function:: clone(a: dasvector`CaptureEntry; b: array<CaptureEntryInitData>)

|detail/function-quote-clone-0x98a6e7e6ce04868f|

:Arguments: * **a** : vector<CaptureEntry>

            * **b** : array< :ref:`CaptureEntryInitData <struct-quote-CaptureEntryInitData>` >

.. _function-quote_clone_line_info_LineInfoInitData:

.. das:function:: clone_line_info(b: LineInfoInitData) : LineInfo

|detail/function-quote-clone_line_info-0xe873c51cd19a8f92|

:Arguments: * **b** :  :ref:`LineInfoInitData <struct-quote-LineInfoInitData>` 

.. _function-quote_clone_dasvector_rq_LineInfo_array_ls_LineInfoInitData_gr_:

.. das:function:: clone(a: dasvector`LineInfo; b: array<LineInfoInitData>)

|detail/function-quote-clone-0x4f61cf6f2a0932e7|

:Arguments: * **a** : vector<LineInfo>

            * **b** : array< :ref:`LineInfoInitData <struct-quote-LineInfoInitData>` >

.. _function-quote_clone_AnnotationList_array_ls_smart_ptr_ls_AnnotationDeclaration_gr__gr_:

.. das:function:: clone(args: AnnotationList; nargs: array<smart_ptr<AnnotationDeclaration>>)

|detail/function-quote-clone-0x2e978299757f5070|

:Arguments: * **args** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` 

            * **nargs** : array<smart_ptr< :ref:`AnnotationDeclaration <handle-rtti-AnnotationDeclaration>` >>

.. _function-quote_clone_dasvector_rq_EnumEntry_array_ls_EnumEntryInitData_gr_:

.. das:function:: clone(a: dasvector`EnumEntry; b: array<EnumEntryInitData>)

|detail/function-quote-clone-0xbf3e68887f05b633|

:Arguments: * **a** : vector<EnumEntry>

            * **b** : array< :ref:`EnumEntryInitData <struct-quote-EnumEntryInitData>` >

.. _function-quote_clone_AnnotationArgumentList_array_ls_AnnotationArgumentInitData_gr_:

.. das:function:: clone(a: AnnotationArgumentList; b: array<AnnotationArgumentInitData>)

|detail/function-quote-clone-0x71dd92c9e0edd381|

:Arguments: * **a** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

            * **b** : array< :ref:`AnnotationArgumentInitData <struct-quote-AnnotationArgumentInitData>` >

.. _function-quote_clone_file_info_FileInfoInitData:

.. das:function:: clone_file_info(b: FileInfoInitData) : FileInfo?

|detail/function-quote-clone_file_info-0x650e31c6b8e5ea4f|

:Arguments: * **b** :  :ref:`FileInfoInitData <struct-quote-FileInfoInitData>` 

.. _function-quote_cvt_to_mks_auto:

.. das:function:: cvt_to_mks(args: auto) : smart_ptr<MakeStruct>

|detail/function-quote-cvt_to_mks-0x16b6d9940665549|

:Arguments: * **args** : auto


