
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

 Range of text in the URI.


.. _handle-uriparser-UriIp4Struct:

.. das:attribute:: UriIp4Struct

:Fields: * **data** : uint8[4] -  IPv4 address portion of the URI.


.. _handle-uriparser-UriIp6Struct:

.. das:attribute:: UriIp6Struct

:Fields: * **data** : uint8[16] -  IPv6 address porition of the URI.


.. _handle-uriparser-UriHostDataA:

.. das:attribute:: UriHostDataA

Host data portion of the URI (IPv4 or IPv6, or some future data).
IPv4 address data.

:Fields: * **ip4** :  :ref:`UriIp4Struct <handle-uriparser-UriIp4Struct>` ? - IPv6 address data.

         * **ip6** :  :ref:`UriIp6Struct <handle-uriparser-UriIp6Struct>` ? - Future host address data.

         * **ipFuture** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>` 


.. _handle-uriparser-UriPathSegmentStructA:

.. das:attribute:: UriPathSegmentStructA

Part of the path portion of the URI.

:Fields: * **text** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>`  - Text of the path segment.

         * **next** :  :ref:`UriPathSegmentStructA <handle-uriparser-UriPathSegmentStructA>` ? - Next path segment, or null if this is the last segment.


.. _handle-uriparser-UriUriA:

.. das:attribute:: UriUriA

URI base class, contains all URI data.
Scheme of the URI.

:Fields: * **scheme** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>`  - User information.

         * **userInfo** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>`  - Host text.

         * **hostText** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>`  - Host data portion of the URI (IPv4 or IPv6, or some future data).

         * **hostData** :  :ref:`UriHostDataA <handle-uriparser-UriHostDataA>`  - Port text.

         * **portText** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>`  - Head of the path.

         * **pathHead** :  :ref:`UriPathSegmentStructA <handle-uriparser-UriPathSegmentStructA>` ? - Tail of the path.

         * **pathTail** :  :ref:`UriPathSegmentStructA <handle-uriparser-UriPathSegmentStructA>` ? - Query portion of the URI.

         * **query** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>`  - Fragment portion of the URI.

         * **fragment** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>`  - Whether the path is absolute.

         * **absolutePath** : int - Whether the URI is owned by the parser.

         * **owner** : int


.. _handle-uriparser-Uri:

.. das:attribute:: Uri

.. _function-uriparser__dot__rq_empty_Uri_implicit:

.. das:function:: Uri implicit.empty() : bool

Returns true if the given URI is empty.



.. _function-uriparser__dot__rq_size_Uri_implicit:

.. das:function:: Uri implicit.size() : int

Returns the size of the given URI.



.. _function-uriparser__dot__rq_status_Uri_implicit:

.. das:function:: Uri implicit.status() : int

Returns the status of the given URI (URI_SUCCESS, URI_ERROR_SYNTAX, etc.).



:Properties: * **empty** : bool

             * **size** : int

             * **status** : int

:Fields: * **uri** :  :ref:`UriUriA <handle-uriparser-UriUriA>`  -  URI implementation.


+++++++++++++++++++++++++++++++
Initialization and finalization
+++++++++++++++++++++++++++++++

  *  :ref:`Uri () : Uri <function-uriparser_Uri>` 
  *  :ref:`using (arg0: block\<(Uri#):void\>) <function-uriparser_using_block_ls_Uri_hh__c_void_gr_>` 
  *  :ref:`Uri (arg0: string implicit) : Uri <function-uriparser_Uri_string_implicit>` 
  *  :ref:`using (arg0: string implicit; arg1: block\<(Uri#):void\>) <function-uriparser_using_string_implicit_block_ls_Uri_hh__c_void_gr_>` 
  *  :ref:`finalize (uri: Uri implicit) <function-uriparser_finalize_Uri_implicit>` 
  *  :ref:`clone (dest: Uri implicit; src: Uri implicit) <function-uriparser_clone_Uri_implicit_Uri_implicit>` 

.. _function-uriparser_Uri:

.. das:function:: Uri() : Uri

 Creates new URI.

.. _function-uriparser_using_block_ls_Uri_hh__c_void_gr_:

.. das:function:: using(arg0: block<(Uri#):void>)

 Creates scoped URI variable.

:Arguments: * **arg0** : block<( :ref:`Uri <handle-uriparser-Uri>` #):void> implicit

.. _function-uriparser_Uri_string_implicit:

.. das:function:: Uri(arg0: string implicit) : Uri

 Creates new URI.

:Arguments: * **arg0** : string implicit

.. _function-uriparser_using_string_implicit_block_ls_Uri_hh__c_void_gr_:

.. das:function:: using(arg0: string implicit; arg1: block<(Uri#):void>)

 Creates scoped URI variable.

:Arguments: * **arg0** : string implicit

            * **arg1** : block<( :ref:`Uri <handle-uriparser-Uri>` #):void> implicit

.. _function-uriparser_finalize_Uri_implicit:

.. das:function:: finalize(uri: Uri implicit)

 Finalizer for the URI.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-uriparser_clone_Uri_implicit_Uri_implicit:

.. das:function:: clone(dest: Uri implicit; src: Uri implicit)

 Clones the URI.

:Arguments: * **dest** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

            * **src** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

+++++++++++++++++++
Escape and unescape
+++++++++++++++++++

  *  :ref:`escape_uri (uriStr: string implicit; spaceToPlus: bool; normalizeBreaks: bool) : string <function-uriparser_escape_uri_string_implicit_bool_bool>` 
  *  :ref:`unescape_uri (uriStr: string implicit) : string <function-uriparser_unescape_uri_string_implicit>` 

.. _function-uriparser_escape_uri_string_implicit_bool_bool:

.. das:function:: escape_uri(uriStr: string implicit; spaceToPlus: bool; normalizeBreaks: bool) : string

 Adds escape characters to the URI.

:Arguments: * **uriStr** : string implicit

            * **spaceToPlus** : bool

            * **normalizeBreaks** : bool

.. _function-uriparser_unescape_uri_string_implicit:

.. das:function:: unescape_uri(uriStr: string implicit) : string

 Remove escape characters from the URI.

:Arguments: * **uriStr** : string implicit

+++++++++++++++++
Uri manipulations
+++++++++++++++++

  *  :ref:`strip_uri (uri: Uri implicit; query: bool; fragment: bool) : Uri <function-uriparser_strip_uri_Uri_implicit_bool_bool>` 
  *  :ref:`add_base_uri (base: Uri implicit; relative: Uri implicit) : Uri <function-uriparser_add_base_uri_Uri_implicit_Uri_implicit>` 
  *  :ref:`remove_base_uri (base: Uri implicit; relative: Uri implicit) : Uri <function-uriparser_remove_base_uri_Uri_implicit_Uri_implicit>` 
  *  :ref:`normalize (uri: Uri implicit) : bool <function-uriparser_normalize_Uri_implicit>` 
  *  :ref:`string (uri: Uri implicit) : string <function-uriparser_string_Uri_implicit>` 
  *  :ref:`string (range: UriTextRangeA implicit) : string <function-uriparser_string_UriTextRangeA_implicit>` 
  *  :ref:`uri_for_each_query_kv (uri: Uri implicit; block: block\<(string#;string#):void\>) <function-uriparser_uri_for_each_query_kv_Uri_implicit_block_ls_string_hh_;string_hh__c_void_gr_>` 
  *  :ref:`normalize_uri (uriStr: string implicit) : string <function-uriparser_normalize_uri_string_implicit>` 

.. _function-uriparser_strip_uri_Uri_implicit_bool_bool:

.. das:function:: strip_uri(uri: Uri implicit; query: bool; fragment: bool) : Uri

 Removes query and fragment from the URI.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

            * **query** : bool

            * **fragment** : bool

.. _function-uriparser_add_base_uri_Uri_implicit_Uri_implicit:

.. das:function:: add_base_uri(base: Uri implicit; relative: Uri implicit) : Uri

 Adds `base` URI to the `relative` URI.

:Arguments: * **base** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

            * **relative** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-uriparser_remove_base_uri_Uri_implicit_Uri_implicit:

.. das:function:: remove_base_uri(base: Uri implicit; relative: Uri implicit) : Uri

 Removes `base` URI from the `relative` URI.

:Arguments: * **base** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

            * **relative** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-uriparser_normalize_Uri_implicit:

.. das:function:: normalize(uri: Uri implicit) : bool

 Normalizes URI, i.e. removes redundant `/` and `.` characters.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-uriparser_string_Uri_implicit:

.. das:function:: string(uri: Uri implicit) : string

 Converts URI to string.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-uriparser_string_UriTextRangeA_implicit:

.. das:function:: string(range: UriTextRangeA implicit) : string

 Converts URI to string.

:Arguments: * **range** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>`  implicit

.. _function-uriparser_uri_for_each_query_kv_Uri_implicit_block_ls_string_hh_;string_hh__c_void_gr_:

.. das:function:: uri_for_each_query_kv(uri: Uri implicit; block: block<(string#;string#):void>)

 Iterates over the URI query parameters.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

            * **block** : block<(string#;string#):void> implicit

.. _function-uriparser_normalize_uri_string_implicit:

.. das:function:: normalize_uri(uriStr: string implicit) : string

 Normalizes URI. i.e. removes redundant `/` and `.` characters.

:Arguments: * **uriStr** : string implicit

+++++++++++++++++++++
File name conversions
+++++++++++++++++++++

  *  :ref:`to_unix_file_name (uri: Uri implicit) : string <function-uriparser_to_unix_file_name_Uri_implicit>` 
  *  :ref:`to_windows_file_name (uri: Uri implicit) : string <function-uriparser_to_windows_file_name_Uri_implicit>` 
  *  :ref:`to_file_name (uri: Uri implicit) : string <function-uriparser_to_file_name_Uri_implicit>` 
  *  :ref:`uri_from_file_name (filename: string implicit) : Uri <function-uriparser_uri_from_file_name_string_implicit>` 
  *  :ref:`uri_from_windows_file_name (filename: string implicit) : Uri <function-uriparser_uri_from_windows_file_name_string_implicit>` 
  *  :ref:`uri_from_unix_file_name (filename: string implicit) : Uri <function-uriparser_uri_from_unix_file_name_string_implicit>` 
  *  :ref:`uri_to_unix_file_name (uriStr: string implicit) : string <function-uriparser_uri_to_unix_file_name_string_implicit>` 
  *  :ref:`uri_to_windows_file_name (uriStr: string implicit) : string <function-uriparser_uri_to_windows_file_name_string_implicit>` 
  *  :ref:`unix_file_name_to_uri (uriStr: string implicit) : string <function-uriparser_unix_file_name_to_uri_string_implicit>` 
  *  :ref:`windows_file_name_to_uri (uriStr: string implicit) : string <function-uriparser_windows_file_name_to_uri_string_implicit>` 
  *  :ref:`uri_to_file_name (uriStr: string implicit) : string <function-uriparser_uri_to_file_name_string_implicit>` 
  *  :ref:`file_name_to_uri (uriStr: string implicit) : string <function-uriparser_file_name_to_uri_string_implicit>` 

.. _function-uriparser_to_unix_file_name_Uri_implicit:

.. das:function:: to_unix_file_name(uri: Uri implicit) : string

 Converts URI to Unix file name.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-uriparser_to_windows_file_name_Uri_implicit:

.. das:function:: to_windows_file_name(uri: Uri implicit) : string

 Converts URI to Windows file name.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-uriparser_to_file_name_Uri_implicit:

.. das:function:: to_file_name(uri: Uri implicit) : string

 Converts URI to the current platform file name.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-uriparser_uri_from_file_name_string_implicit:

.. das:function:: uri_from_file_name(filename: string implicit) : Uri

 Converts current platform file name to URI.

:Arguments: * **filename** : string implicit

.. _function-uriparser_uri_from_windows_file_name_string_implicit:

.. das:function:: uri_from_windows_file_name(filename: string implicit) : Uri

 Converts Windows file name to URI.

:Arguments: * **filename** : string implicit

.. _function-uriparser_uri_from_unix_file_name_string_implicit:

.. das:function:: uri_from_unix_file_name(filename: string implicit) : Uri

 Converts Unix file name to URI.

:Arguments: * **filename** : string implicit

.. _function-uriparser_uri_to_unix_file_name_string_implicit:

.. das:function:: uri_to_unix_file_name(uriStr: string implicit) : string

 Converts URI to Unix file name.

:Arguments: * **uriStr** : string implicit

.. _function-uriparser_uri_to_windows_file_name_string_implicit:

.. das:function:: uri_to_windows_file_name(uriStr: string implicit) : string

 Converts URI to Windows file name.

:Arguments: * **uriStr** : string implicit

.. _function-uriparser_unix_file_name_to_uri_string_implicit:

.. das:function:: unix_file_name_to_uri(uriStr: string implicit) : string

 Converts Unix file name to URI.

:Arguments: * **uriStr** : string implicit

.. _function-uriparser_windows_file_name_to_uri_string_implicit:

.. das:function:: windows_file_name_to_uri(uriStr: string implicit) : string

 Converts Windows file name to URI.

:Arguments: * **uriStr** : string implicit

.. _function-uriparser_uri_to_file_name_string_implicit:

.. das:function:: uri_to_file_name(uriStr: string implicit) : string

 Converts URI to the current platform file name.

:Arguments: * **uriStr** : string implicit

.. _function-uriparser_file_name_to_uri_string_implicit:

.. das:function:: file_name_to_uri(uriStr: string implicit) : string

 Converts current file name to URI.

:Arguments: * **uriStr** : string implicit

++++
GUID
++++

  *  :ref:`make_new_guid () : string <function-uriparser_make_new_guid>` 

.. _function-uriparser_make_new_guid:

.. das:function:: make_new_guid() : string

 Generates new GUID.


