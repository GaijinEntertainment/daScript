
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

++++++++++++++++
Clone operations
++++++++++++++++

  *  :ref:`clone (var a: dasvector`LineInfo; b: array\<LineInfoInitData\>) <function-quote_clone_dasvector_rq_LineInfo_array_ls_LineInfoInitData_gr_>` 
  *  :ref:`clone (var a: dasvector`CaptureEntry; b: array\<CaptureEntryInitData\>) <function-quote_clone_dasvector_rq_CaptureEntry_array_ls_CaptureEntryInitData_gr_>` 
  *  :ref:`clone (var args: AnnotationList; var nargs: array\<smart_ptr\<AnnotationDeclaration\>\>) <function-quote_clone_AnnotationList_array_ls_smart_ptr_ls_AnnotationDeclaration_gr__gr_>` 
  *  :ref:`clone (var a: dasvector`EnumEntry; var b: array\<EnumEntryInitData\>) <function-quote_clone_dasvector_rq_EnumEntry_array_ls_EnumEntryInitData_gr_>` 
  *  :ref:`clone (var a: AnnotationArgumentList; b: array\<AnnotationArgumentInitData\>) <function-quote_clone_AnnotationArgumentList_array_ls_AnnotationArgumentInitData_gr_>` 
  *  :ref:`clone_file_info (b: FileInfoInitData) : FileInfo? <function-quote_clone_file_info_FileInfoInitData>` 
  *  :ref:`clone_line_info (var b: LineInfoInitData) : LineInfo <function-quote_clone_line_info_LineInfoInitData>` 


clone
^^^^^

.. _function-quote_clone_dasvector_rq_LineInfo_array_ls_LineInfoInitData_gr_:

.. das:function:: clone(a: dasvector`LineInfo; b: array<LineInfoInitData>)

Clones an array of LineInfoInitData into a dasvector of LineInfo.

:Arguments: * **a** : vector<LineInfo>

            * **b** : array< :ref:`LineInfoInitData <struct-quote-LineInfoInitData>` >

.. _function-quote_clone_dasvector_rq_CaptureEntry_array_ls_CaptureEntryInitData_gr_:

.. das:function:: clone(a: dasvector`CaptureEntry; b: array<CaptureEntryInitData>)

.. _function-quote_clone_AnnotationList_array_ls_smart_ptr_ls_AnnotationDeclaration_gr__gr_:

.. das:function:: clone(args: AnnotationList; nargs: array<smart_ptr<AnnotationDeclaration>>)

.. _function-quote_clone_dasvector_rq_EnumEntry_array_ls_EnumEntryInitData_gr_:

.. das:function:: clone(a: dasvector`EnumEntry; b: array<EnumEntryInitData>)

.. _function-quote_clone_AnnotationArgumentList_array_ls_AnnotationArgumentInitData_gr_:

.. das:function:: clone(a: AnnotationArgumentList; b: array<AnnotationArgumentInitData>)

----

.. _function-quote_clone_file_info_FileInfoInitData:

.. das:function:: clone_file_info(b: FileInfoInitData) : FileInfo?

Creates a FileInfo from a FileInfoInitData struct.

:Arguments: * **b** :  :ref:`FileInfoInitData <struct-quote-FileInfoInitData>` 

.. _function-quote_clone_line_info_LineInfoInitData:

.. das:function:: clone_line_info(b: LineInfoInitData) : LineInfo

Creates a LineInfo from a LineInfoInitData struct.

:Arguments: * **b** :  :ref:`LineInfoInitData <struct-quote-LineInfoInitData>` 

++++++++++
Conversion
++++++++++

  *  :ref:`cvt_to_mks (var args: auto) : smart_ptr\<MakeStruct\> <function-quote_cvt_to_mks_auto_0x83>` 

.. _function-quote_cvt_to_mks_auto_0x83:

.. das:function:: cvt_to_mks(args: auto) : smart_ptr<MakeStruct>

Converts an array of arguments into a MakeStruct smart pointer.

:Arguments: * **args** : auto


