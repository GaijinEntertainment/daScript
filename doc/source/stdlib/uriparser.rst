
.. _stdlib_uriparser:

===========================================
URI manipulation library based on UriParser
===========================================

.. das:module:: uriparser

The URIPARSER module provides URI parsing and manipulation based on the uriparser library.
It supports parsing URI strings into components (scheme, host, path, query, fragment),
normalization, resolution of relative URIs, and GUID generation.

All functions and symbols are in "uriparser" module, use require to get access to it.

.. code-block:: das

    require uriparser



++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-uriparser-UriTextRangeA:

.. das:attribute:: UriTextRangeA

 Range of text in the URI.



.. _handle-uriparser-UriIp4Struct:

.. das:attribute:: UriIp4Struct

 IPv4 address portion of the URI.

:Fields: * **data** : uint8[4] - IPv4 address data.



.. _handle-uriparser-UriIp6Struct:

.. das:attribute:: UriIp6Struct

 IPv6 address portion of the URI.

:Fields: * **data** : uint8[16] - IPv6 address data.



.. _handle-uriparser-UriHostDataA:

.. das:attribute:: UriHostDataA

Host data portion of the URI (IPv4 or IPv6, or some future data).

:Fields: * **ip4** :  :ref:`UriIp4Struct <handle-uriparser-UriIp4Struct>`? - IPv4 address data.

         * **ip6** :  :ref:`UriIp6Struct <handle-uriparser-UriIp6Struct>`? - IPv6 address data.

         * **ipFuture** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>` - Future host address data.



.. _handle-uriparser-UriPathSegmentStructA:

.. das:attribute:: UriPathSegmentStructA

Part of the path portion of the URI.

:Fields: * **text** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>` - Text of the path segment.

         * **next** :  :ref:`UriPathSegmentStructA <handle-uriparser-UriPathSegmentStructA>`? - Next path segment, or null if this is the last segment.



.. _handle-uriparser-UriUriA:

.. das:attribute:: UriUriA

URI base class, contains all URI data.

:Fields: * **scheme** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>` - Scheme of the URI.

         * **userInfo** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>` - User information.

         * **hostText** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>` - Host text.

         * **hostData** :  :ref:`UriHostDataA <handle-uriparser-UriHostDataA>` - Host data portion of the URI (IPv4 or IPv6, or some future data).

         * **portText** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>` - Port text.

         * **pathHead** :  :ref:`UriPathSegmentStructA <handle-uriparser-UriPathSegmentStructA>`? - Head of the path.

         * **pathTail** :  :ref:`UriPathSegmentStructA <handle-uriparser-UriPathSegmentStructA>`? - Tail of the path.

         * **query** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>` - Query portion of the URI.

         * **fragment** :  :ref:`UriTextRangeA <handle-uriparser-UriTextRangeA>` - Fragment portion of the URI.

         * **absolutePath** : int - Whether the path is absolute.

         * **owner** : int - Whether the URI is owned by the parser.



.. _handle-uriparser-Uri:

.. das:attribute:: Uri

.. _function-uriparser__dot__rq_empty_Uri:

.. das:function:: Uri.empty() : bool

Returns ``true`` if the ``Uri`` object contains no URI data.


.. _function-uriparser__dot__rq_size_Uri:

.. das:function:: Uri.size() : int

Returns the string length of the URI.


.. _function-uriparser__dot__rq_status_Uri:

.. das:function:: Uri.status() : int

Returns the parse status code of the ``Uri`` object.


:Properties: * **empty** : bool

             * **size** : int

             * **status** : int

:Fields: * **uri** :  :ref:`UriUriA <handle-uriparser-UriUriA>` -  URI implementation.




+++++++++++++++++++++++++++++++
Initialization and finalization
+++++++++++++++++++++++++++++++

  *  :ref:`Uri (arg0: string) : Uri <function-uriparser_Uri_string>`
  *  :ref:`Uri () : Uri <function-uriparser_Uri>`
  *  :ref:`clone (dest: Uri; src: Uri) <function-uriparser_clone_Uri_Uri>`
  *  :ref:`finalize (uri: Uri) <function-uriparser_finalize_Uri>`
  *  :ref:`using (arg0: string; arg1: block\<(Uri#):void\>) <function-uriparser_using_string_block_ls_Uri_hh__c_void_gr_>`
  *  :ref:`using (arg0: block\<(Uri#):void\>) <function-uriparser_using_block_ls_Uri_hh__c_void_gr_>`


Uri
^^^

.. _function-uriparser_Uri_string:

.. das:function:: Uri(arg0: string) : Uri

Constructs a new ``Uri`` object by parsing the given URI string.


:Arguments: * **arg0** : string implicit

.. _function-uriparser_Uri:

.. das:function:: Uri() : Uri

----

.. _function-uriparser_clone_Uri_Uri:

.. das:function:: clone(dest: Uri; src: Uri)

Creates a deep copy of the given ``Uri`` object.


:Arguments: * **dest** :  :ref:`Uri <handle-uriparser-Uri>` implicit

            * **src** :  :ref:`Uri <handle-uriparser-Uri>` implicit

.. _function-uriparser_finalize_Uri:

.. das:function:: finalize(uri: Uri)

Releases all resources held by the ``Uri`` object.


:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>` implicit


using
^^^^^

.. _function-uriparser_using_string_block_ls_Uri_hh__c_void_gr_:

.. das:function:: using(arg0: string; arg1: block<(Uri#):void>)

Creates a scoped ``Uri`` variable that is automatically finalized at end of block.


:Arguments: * **arg0** : string implicit

            * **arg1** : block<( :ref:`Uri <handle-uriparser-Uri>`\ #):void> implicit

.. _function-uriparser_using_block_ls_Uri_hh__c_void_gr_:

.. das:function:: using(arg0: block<(Uri#):void>)


+++++++++++++++++++
Escape and unescape
+++++++++++++++++++

  *  :ref:`escape_uri (uriStr: string; spaceToPlus: bool; normalizeBreaks: bool) : string <function-uriparser_escape_uri_string_bool_bool>`
  *  :ref:`unescape_uri (uriStr: string) : string <function-uriparser_unescape_uri_string>`

.. _function-uriparser_escape_uri_string_bool_bool:

.. das:function:: escape_uri(uriStr: string; spaceToPlus: bool; normalizeBreaks: bool) : string

Percent-encodes reserved and special characters in the URI string.


:Arguments: * **uriStr** : string implicit

            * **spaceToPlus** : bool

            * **normalizeBreaks** : bool

.. _function-uriparser_unescape_uri_string:

.. das:function:: unescape_uri(uriStr: string) : string

Decodes percent-encoded characters in the URI string.


:Arguments: * **uriStr** : string implicit


+++++++++++++++++
Uri manipulations
+++++++++++++++++

  *  :ref:`add_base_uri (base: Uri; relative: Uri) : Uri <function-uriparser_add_base_uri_Uri_Uri>`
  *  :ref:`normalize (uri: Uri) : bool <function-uriparser_normalize_Uri>`
  *  :ref:`normalize_uri (uriStr: string) : string <function-uriparser_normalize_uri_string>`
  *  :ref:`remove_base_uri (base: Uri; relative: Uri) : Uri <function-uriparser_remove_base_uri_Uri_Uri>`
  *  :ref:`string (uri: Uri) : string <function-uriparser_string_Uri>`
  *  :ref:`string (range: UriTextRangeA) : string <function-uriparser_string_UriTextRangeA>`
  *  :ref:`strip_uri (uri: Uri; query: bool; fragment: bool) : Uri <function-uriparser_strip_uri_Uri_bool_bool>`
  *  :ref:`uri_for_each_query_kv (uri: Uri; block: block\<(string#;string#):void\>) <function-uriparser_uri_for_each_query_kv_Uri_block_ls_string_hh_;string_hh__c_void_gr_>`

.. _function-uriparser_add_base_uri_Uri_Uri:

.. das:function:: add_base_uri(base: Uri; relative: Uri) : Uri

Resolves a relative URI against a base URI, producing an absolute URI.


:Arguments: * **base** :  :ref:`Uri <handle-uriparser-Uri>` implicit

            * **relative** :  :ref:`Uri <handle-uriparser-Uri>` implicit

.. _function-uriparser_normalize_Uri:

.. das:function:: normalize(uri: Uri) : bool

Normalizes a ``Uri`` in place, removing redundant ``/``, ``.``, and ``..`` path segments.


:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>` implicit

.. _function-uriparser_normalize_uri_string:

.. das:function:: normalize_uri(uriStr: string) : string

Returns a normalized copy of the URI string with redundant ``/``, ``.``, and ``..`` segments removed.


:Arguments: * **uriStr** : string implicit

.. _function-uriparser_remove_base_uri_Uri_Uri:

.. das:function:: remove_base_uri(base: Uri; relative: Uri) : Uri

Computes a relative URI by removing the base URI prefix from an absolute URI.


:Arguments: * **base** :  :ref:`Uri <handle-uriparser-Uri>` implicit

            * **relative** :  :ref:`Uri <handle-uriparser-Uri>` implicit


string
^^^^^^

.. _function-uriparser_string_Uri:

.. das:function:: string(uri: Uri) : string

Converts a ``Uri`` object to its string representation.


:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>` implicit

.. _function-uriparser_string_UriTextRangeA:

.. das:function:: string(range: UriTextRangeA) : string

----

.. _function-uriparser_strip_uri_Uri_bool_bool:

.. das:function:: strip_uri(uri: Uri; query: bool; fragment: bool) : Uri

Removes the query string and fragment from the URI.


:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>` implicit

            * **query** : bool

            * **fragment** : bool

.. _function-uriparser_uri_for_each_query_kv_Uri_block_ls_string_hh_;string_hh__c_void_gr_:

.. das:function:: uri_for_each_query_kv(uri: Uri; block: block<(string#;string#):void>)

Iterates over each key-value pair in the URI's query string, invoking a block for each.


:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>` implicit

            * **block** : block<(string\ #;string\ #):void> implicit


+++++++++++++++++++++
File name conversions
+++++++++++++++++++++

  *  :ref:`file_name_to_uri (uriStr: string) : string <function-uriparser_file_name_to_uri_string>`
  *  :ref:`to_file_name (uri: Uri) : string <function-uriparser_to_file_name_Uri>`
  *  :ref:`to_unix_file_name (uri: Uri) : string <function-uriparser_to_unix_file_name_Uri>`
  *  :ref:`to_windows_file_name (uri: Uri) : string <function-uriparser_to_windows_file_name_Uri>`
  *  :ref:`unix_file_name_to_uri (uriStr: string) : string <function-uriparser_unix_file_name_to_uri_string>`
  *  :ref:`uri_from_file_name (filename: string) : Uri <function-uriparser_uri_from_file_name_string>`
  *  :ref:`uri_from_unix_file_name (filename: string) : Uri <function-uriparser_uri_from_unix_file_name_string>`
  *  :ref:`uri_from_windows_file_name (filename: string) : Uri <function-uriparser_uri_from_windows_file_name_string>`
  *  :ref:`uri_to_file_name (uriStr: string) : string <function-uriparser_uri_to_file_name_string>`
  *  :ref:`uri_to_unix_file_name (uriStr: string) : string <function-uriparser_uri_to_unix_file_name_string>`
  *  :ref:`uri_to_windows_file_name (uriStr: string) : string <function-uriparser_uri_to_windows_file_name_string>`
  *  :ref:`windows_file_name_to_uri (uriStr: string) : string <function-uriparser_windows_file_name_to_uri_string>`

.. _function-uriparser_file_name_to_uri_string:

.. das:function:: file_name_to_uri(uriStr: string) : string

Converts a platform-native file path to a ``file://`` URI string.


:Arguments: * **uriStr** : string implicit

.. _function-uriparser_to_file_name_Uri:

.. das:function:: to_file_name(uri: Uri) : string

Converts a ``Uri`` to a platform-native file path.


:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>` implicit

.. _function-uriparser_to_unix_file_name_Uri:

.. das:function:: to_unix_file_name(uri: Uri) : string

Converts a ``Uri`` to a Unix-style file path.


:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>` implicit

.. _function-uriparser_to_windows_file_name_Uri:

.. das:function:: to_windows_file_name(uri: Uri) : string

Converts a ``Uri`` to a Windows-style file path.


:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>` implicit

.. _function-uriparser_unix_file_name_to_uri_string:

.. das:function:: unix_file_name_to_uri(uriStr: string) : string

Converts a Unix-style file path to a ``file://`` URI string.


:Arguments: * **uriStr** : string implicit

.. _function-uriparser_uri_from_file_name_string:

.. das:function:: uri_from_file_name(filename: string) : Uri

Converts a platform-native file path to a ``file://`` URI string.


:Arguments: * **filename** : string implicit

.. _function-uriparser_uri_from_unix_file_name_string:

.. das:function:: uri_from_unix_file_name(filename: string) : Uri

Converts a Unix-style file path to a ``file://`` URI string.


:Arguments: * **filename** : string implicit

.. _function-uriparser_uri_from_windows_file_name_string:

.. das:function:: uri_from_windows_file_name(filename: string) : Uri

Converts a Windows-style file path to a ``file://`` URI string.


:Arguments: * **filename** : string implicit

.. _function-uriparser_uri_to_file_name_string:

.. das:function:: uri_to_file_name(uriStr: string) : string

Converts a URI string to a platform-native file path.


:Arguments: * **uriStr** : string implicit

.. _function-uriparser_uri_to_unix_file_name_string:

.. das:function:: uri_to_unix_file_name(uriStr: string) : string

Converts a URI string to a Unix-style file path.


:Arguments: * **uriStr** : string implicit

.. _function-uriparser_uri_to_windows_file_name_string:

.. das:function:: uri_to_windows_file_name(uriStr: string) : string

Converts a URI string to a Windows-style file path.


:Arguments: * **uriStr** : string implicit

.. _function-uriparser_windows_file_name_to_uri_string:

.. das:function:: windows_file_name_to_uri(uriStr: string) : string

Converts a Windows-style file path to a ``file://`` URI string.


:Arguments: * **uriStr** : string implicit


++++
GUID
++++

  *  :ref:`make_new_guid () : string <function-uriparser_make_new_guid>`

.. _function-uriparser_make_new_guid:

.. das:function:: make_new_guid() : string

Generates a new random GUID/UUID string.



