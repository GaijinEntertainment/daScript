
.. _stdlib_uriparser:

===========================================
URI manipulation library based on UriParser
===========================================

The URIPARSER module exposes uriParser library https://uriparser.github.io to Daslang.

All functions and symbols are in "uriparser" module, use require to get access to it. ::

    require uriparser


++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-uriparser-UriTextRangeA:

.. das:attribute:: UriTextRangeA

// stub


.. _handle-uriparser-UriIp4Struct:

.. das:attribute:: UriIp4Struct

:Fields: * **data** : uint8[4]

// stub


.. _handle-uriparser-UriIp6Struct:

.. das:attribute:: UriIp6Struct

:Fields: * **data** : uint8[16]

// stub


.. _handle-uriparser-UriHostDataA:

.. das:attribute:: UriHostDataA

:Fields: * **ip4** :  :ref:`UriIp4Struct <handle-uriparser-UriIp4Struct>` ?

         * **ip6** :  :ref:`UriIp6Struct <handle-uriparser-UriIp6Struct>` ?

         * **ipFuture** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>` 

// stub


.. _handle-uriparser-UriPathSegmentStructA:

.. das:attribute:: UriPathSegmentStructA

:Fields: * **text** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>` 

         * **next** :  :ref:`UriPathSegmentStructA <handle-uriparser-UriPathSegmentStructA>` ?

// stub


.. _handle-uriparser-UriUriA:

.. das:attribute:: UriUriA

:Fields: * **scheme** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>` 

         * **userInfo** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>` 

         * **hostText** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>` 

         * **hostData** :  :ref:`UriHostDataA <handle-uriparser-UriHostDataA>` 

         * **portText** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>` 

         * **pathHead** :  :ref:`UriPathSegmentStructA <handle-uriparser-UriPathSegmentStructA>` ?

         * **pathTail** :  :ref:`UriPathSegmentStructA <handle-uriparser-UriPathSegmentStructA>` ?

         * **query** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>` 

         * **fragment** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>` 

         * **absolutePath** : int

         * **owner** : int

// stub


.. _handle-uriparser-Uri:

.. das:attribute:: Uri

.. _function-_at_uriparser_c__c__dot__rq_empty_CIH_ls_uriparser_c__c_Uri_gr_:

.. das:function:: Uri implicit.empty() : bool

// stub


.. _function-_at_uriparser_c__c__dot__rq_size_CIH_ls_uriparser_c__c_Uri_gr_:

.. das:function:: Uri implicit.size() : int

// stub


.. _function-_at_uriparser_c__c__dot__rq_status_CIH_ls_uriparser_c__c_Uri_gr_:

.. das:function:: Uri implicit.status() : int

// stub


:Properties: * **empty** : bool

             * **size** : int

             * **status** : int

:Fields: * **uri** :  :ref:`UriUriA <handle-uriparser-UriUriA>` 

// stub


+++++++++++++++++++++++++++++++
Initialization and finalization
+++++++++++++++++++++++++++++++

  *  :ref:`Uri () : Uri <function-_at_uriparser_c__c_Uri>` 
  *  :ref:`using (arg0: block\<(var arg0:Uri#):void\> implicit) <function-_at_uriparser_c__c_using_CI0_ls__hh_XH_ls_uriparser_c__c_Uri_gr__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`Uri (arg0: string implicit) : Uri <function-_at_uriparser_c__c_Uri_CIs>` 
  *  :ref:`using (arg0: string implicit; arg1: block\<(var arg0:Uri#):void\> implicit) <function-_at_uriparser_c__c_using_CIs_CI0_ls__hh_XH_ls_uriparser_c__c_Uri_gr__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`finalize (uri: Uri implicit) <function-_at_uriparser_c__c_finalize_IH_ls_uriparser_c__c_Uri_gr_>` 
  *  :ref:`clone (dest: Uri implicit; src: Uri implicit) <function-_at_uriparser_c__c_clone_IH_ls_uriparser_c__c_Uri_gr__CIH_ls_uriparser_c__c_Uri_gr_>` 

.. _function-_at_uriparser_c__c_Uri:

.. das:function:: Uri() : Uri

// stub


.. _function-_at_uriparser_c__c_using_CI0_ls__hh_XH_ls_uriparser_c__c_Uri_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: using(arg0: block<(var arg0:Uri#):void> implicit)

// stub



:Arguments: * **arg0** : block<( :ref:`Uri <handle-uriparser-Uri>` #):void> implicit

.. _function-_at_uriparser_c__c_Uri_CIs:

.. das:function:: Uri(arg0: string implicit) : Uri

// stub



:Arguments: * **arg0** : string implicit

.. _function-_at_uriparser_c__c_using_CIs_CI0_ls__hh_XH_ls_uriparser_c__c_Uri_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: using(arg0: string implicit; arg1: block<(var arg0:Uri#):void> implicit)

// stub



:Arguments: * **arg0** : string implicit

            * **arg1** : block<( :ref:`Uri <handle-uriparser-Uri>` #):void> implicit

.. _function-_at_uriparser_c__c_finalize_IH_ls_uriparser_c__c_Uri_gr_:

.. das:function:: finalize(uri: Uri implicit)

// stub



:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-_at_uriparser_c__c_clone_IH_ls_uriparser_c__c_Uri_gr__CIH_ls_uriparser_c__c_Uri_gr_:

.. das:function:: clone(dest: Uri implicit; src: Uri implicit)

// stub



:Arguments: * **dest** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

            * **src** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

+++++++++++++++++++
Escape and unescape
+++++++++++++++++++

  *  :ref:`escape_uri (uriStr: string implicit; spaceToPlus: bool; normalizeBreaks: bool) : string <function-_at_uriparser_c__c_escape_uri_CIs_Cb_Cb_C_c_C_l>` 
  *  :ref:`unescape_uri (uriStr: string implicit) : string <function-_at_uriparser_c__c_unescape_uri_CIs_C_c_C_l>` 

.. _function-_at_uriparser_c__c_escape_uri_CIs_Cb_Cb_C_c_C_l:

.. das:function:: escape_uri(uriStr: string implicit; spaceToPlus: bool; normalizeBreaks: bool) : string

// stub



:Arguments: * **uriStr** : string implicit

            * **spaceToPlus** : bool

            * **normalizeBreaks** : bool

.. _function-_at_uriparser_c__c_unescape_uri_CIs_C_c_C_l:

.. das:function:: unescape_uri(uriStr: string implicit) : string

// stub



:Arguments: * **uriStr** : string implicit

+++++++++++++++++
Uri manipulations
+++++++++++++++++

  *  :ref:`strip_uri (uri: Uri implicit; query: bool; fragment: bool) : Uri <function-_at_uriparser_c__c_strip_uri_CIH_ls_uriparser_c__c_Uri_gr__Cb_Cb>` 
  *  :ref:`add_base_uri (base: Uri implicit; relative: Uri implicit) : Uri <function-_at_uriparser_c__c_add_base_uri_CIH_ls_uriparser_c__c_Uri_gr__CIH_ls_uriparser_c__c_Uri_gr_>` 
  *  :ref:`remove_base_uri (base: Uri implicit; relative: Uri implicit) : Uri <function-_at_uriparser_c__c_remove_base_uri_CIH_ls_uriparser_c__c_Uri_gr__CIH_ls_uriparser_c__c_Uri_gr_>` 
  *  :ref:`normalize (uri: Uri implicit) : bool <function-_at_uriparser_c__c_normalize_IH_ls_uriparser_c__c_Uri_gr_>` 
  *  :ref:`string (uri: Uri implicit) : string <function-_at_uriparser_c__c_string_CIH_ls_uriparser_c__c_Uri_gr__C_c_C_l>` 
  *  :ref:`string (range: UriTextRangeA implicit) : string <function-_at_uriparser_c__c_string_CIH_ls_uriparser_c__c_UriTextRangeA_gr__C_c_C_l>` 
  *  :ref:`uri_for_each_query_kv (uri: Uri implicit; block: block\<(var arg0:string#;var arg1:string#):void\> implicit) <function-_at_uriparser_c__c_uri_for_each_query_kv_CIH_ls_uriparser_c__c_Uri_gr__CI0_ls__hh_s;_hh_s_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`normalize_uri (uriStr: string implicit) : string <function-_at_uriparser_c__c_normalize_uri_CIs_C_c_C_l>` 

.. _function-_at_uriparser_c__c_strip_uri_CIH_ls_uriparser_c__c_Uri_gr__Cb_Cb:

.. das:function:: strip_uri(uri: Uri implicit; query: bool; fragment: bool) : Uri

// stub



:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

            * **query** : bool

            * **fragment** : bool

.. _function-_at_uriparser_c__c_add_base_uri_CIH_ls_uriparser_c__c_Uri_gr__CIH_ls_uriparser_c__c_Uri_gr_:

.. das:function:: add_base_uri(base: Uri implicit; relative: Uri implicit) : Uri

// stub



:Arguments: * **base** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

            * **relative** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-_at_uriparser_c__c_remove_base_uri_CIH_ls_uriparser_c__c_Uri_gr__CIH_ls_uriparser_c__c_Uri_gr_:

.. das:function:: remove_base_uri(base: Uri implicit; relative: Uri implicit) : Uri

// stub



:Arguments: * **base** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

            * **relative** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-_at_uriparser_c__c_normalize_IH_ls_uriparser_c__c_Uri_gr_:

.. das:function:: normalize(uri: Uri implicit) : bool

// stub



:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-_at_uriparser_c__c_string_CIH_ls_uriparser_c__c_Uri_gr__C_c_C_l:

.. das:function:: string(uri: Uri implicit) : string

// stub



:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-_at_uriparser_c__c_string_CIH_ls_uriparser_c__c_UriTextRangeA_gr__C_c_C_l:

.. das:function:: string(range: UriTextRangeA implicit) : string

// stub



:Arguments: * **range** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>`  implicit

.. _function-_at_uriparser_c__c_uri_for_each_query_kv_CIH_ls_uriparser_c__c_Uri_gr__CI0_ls__hh_s;_hh_s_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: uri_for_each_query_kv(uri: Uri implicit; block: block<(var arg0:string#;var arg1:string#):void> implicit)

// stub



:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

            * **block** : block<(string#;string#):void> implicit

.. _function-_at_uriparser_c__c_normalize_uri_CIs_C_c_C_l:

.. das:function:: normalize_uri(uriStr: string implicit) : string

// stub



:Arguments: * **uriStr** : string implicit

+++++++++++++++++++++
File name conversions
+++++++++++++++++++++

  *  :ref:`to_unix_file_name (uri: Uri implicit) : string <function-_at_uriparser_c__c_to_unix_file_name_CIH_ls_uriparser_c__c_Uri_gr__C_c_C_l>` 
  *  :ref:`to_windows_file_name (uri: Uri implicit) : string <function-_at_uriparser_c__c_to_windows_file_name_CIH_ls_uriparser_c__c_Uri_gr__C_c_C_l>` 
  *  :ref:`to_file_name (uri: Uri implicit) : string <function-_at_uriparser_c__c_to_file_name_CIH_ls_uriparser_c__c_Uri_gr__C_c_C_l>` 
  *  :ref:`uri_from_file_name (filename: string implicit) : Uri <function-_at_uriparser_c__c_uri_from_file_name_CIs>` 
  *  :ref:`uri_from_windows_file_name (filename: string implicit) : Uri <function-_at_uriparser_c__c_uri_from_windows_file_name_CIs>` 
  *  :ref:`uri_from_unix_file_name (filename: string implicit) : Uri <function-_at_uriparser_c__c_uri_from_unix_file_name_CIs>` 
  *  :ref:`uri_to_unix_file_name (uriStr: string implicit) : string <function-_at_uriparser_c__c_uri_to_unix_file_name_CIs_C_c_C_l>` 
  *  :ref:`uri_to_windows_file_name (uriStr: string implicit) : string <function-_at_uriparser_c__c_uri_to_windows_file_name_CIs_C_c_C_l>` 
  *  :ref:`unix_file_name_to_uri (uriStr: string implicit) : string <function-_at_uriparser_c__c_unix_file_name_to_uri_CIs_C_c_C_l>` 
  *  :ref:`windows_file_name_to_uri (uriStr: string implicit) : string <function-_at_uriparser_c__c_windows_file_name_to_uri_CIs_C_c_C_l>` 
  *  :ref:`uri_to_file_name (uriStr: string implicit) : string <function-_at_uriparser_c__c_uri_to_file_name_CIs_C_c_C_l>` 
  *  :ref:`file_name_to_uri (uriStr: string implicit) : string <function-_at_uriparser_c__c_file_name_to_uri_CIs_C_c_C_l>` 

.. _function-_at_uriparser_c__c_to_unix_file_name_CIH_ls_uriparser_c__c_Uri_gr__C_c_C_l:

.. das:function:: to_unix_file_name(uri: Uri implicit) : string

// stub



:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-_at_uriparser_c__c_to_windows_file_name_CIH_ls_uriparser_c__c_Uri_gr__C_c_C_l:

.. das:function:: to_windows_file_name(uri: Uri implicit) : string

// stub



:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-_at_uriparser_c__c_to_file_name_CIH_ls_uriparser_c__c_Uri_gr__C_c_C_l:

.. das:function:: to_file_name(uri: Uri implicit) : string

// stub



:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-_at_uriparser_c__c_uri_from_file_name_CIs:

.. das:function:: uri_from_file_name(filename: string implicit) : Uri

// stub



:Arguments: * **filename** : string implicit

.. _function-_at_uriparser_c__c_uri_from_windows_file_name_CIs:

.. das:function:: uri_from_windows_file_name(filename: string implicit) : Uri

// stub



:Arguments: * **filename** : string implicit

.. _function-_at_uriparser_c__c_uri_from_unix_file_name_CIs:

.. das:function:: uri_from_unix_file_name(filename: string implicit) : Uri

// stub



:Arguments: * **filename** : string implicit

.. _function-_at_uriparser_c__c_uri_to_unix_file_name_CIs_C_c_C_l:

.. das:function:: uri_to_unix_file_name(uriStr: string implicit) : string

// stub



:Arguments: * **uriStr** : string implicit

.. _function-_at_uriparser_c__c_uri_to_windows_file_name_CIs_C_c_C_l:

.. das:function:: uri_to_windows_file_name(uriStr: string implicit) : string

// stub



:Arguments: * **uriStr** : string implicit

.. _function-_at_uriparser_c__c_unix_file_name_to_uri_CIs_C_c_C_l:

.. das:function:: unix_file_name_to_uri(uriStr: string implicit) : string

// stub



:Arguments: * **uriStr** : string implicit

.. _function-_at_uriparser_c__c_windows_file_name_to_uri_CIs_C_c_C_l:

.. das:function:: windows_file_name_to_uri(uriStr: string implicit) : string

// stub



:Arguments: * **uriStr** : string implicit

.. _function-_at_uriparser_c__c_uri_to_file_name_CIs_C_c_C_l:

.. das:function:: uri_to_file_name(uriStr: string implicit) : string

// stub



:Arguments: * **uriStr** : string implicit

.. _function-_at_uriparser_c__c_file_name_to_uri_CIs_C_c_C_l:

.. das:function:: file_name_to_uri(uriStr: string implicit) : string

// stub



:Arguments: * **uriStr** : string implicit

++++
GUID
++++

  *  :ref:`make_new_guid () : string <function-_at_uriparser_c__c_make_new_guid_C_c_C_l>` 

.. _function-_at_uriparser_c__c_make_new_guid_C_c_C_l:

.. das:function:: make_new_guid() : string

// stub



