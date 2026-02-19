
.. _stdlib_uriparser_boost:

================================
Boost package for the URI parser
================================

.. das:module:: uriparser_boost

The URIPARSER_BOOST module extends URI handling with convenience functions
for common operations like building URIs from components, extracting query
parameters, and resolving relative paths.

All functions and symbols are in "uriparser_boost" module, use require to get access to it.

.. code-block:: das

    require daslib/uriparser_boost



+++++++++++++++++
Split and compose
+++++++++++++++++

  *  :ref:`uri_compose (scheme: string; userInfo: string; hostText: string; portText: string; path: string; query: string; fragment: string) : Uri <function-uriparser_boost_uri_compose_string_string_string_string_string_string_string>`
  *  :ref:`uri_compose_query (query: table\<string, string\>) : string <function-uriparser_boost_uri_compose_query_table_ls_string,_string_gr_>`
  *  :ref:`uri_compose_query_in_order (query: table\<string, string\>) : string <function-uriparser_boost_uri_compose_query_in_order_table_ls_string,_string_gr_>`
  *  :ref:`uri_split_full_path (uri: Uri) : array\<string\> <function-uriparser_boost_uri_split_full_path_Uri>`

.. _function-uriparser_boost_uri_compose_string_string_string_string_string_string_string:

.. das:function:: uri_compose(scheme: string; userInfo: string; hostText: string; portText: string; path: string; query: string; fragment: string) : Uri

Compose a URI from its components.


:Arguments: * **scheme** : string

            * **userInfo** : string

            * **hostText** : string

            * **portText** : string

            * **path** : string

            * **query** : string

            * **fragment** : string

.. _function-uriparser_boost_uri_compose_query_table_ls_string,_string_gr_:

.. das:function:: uri_compose_query(query: table<string, string>) : string

Compose a query string from a table of key-value pairs.


:Arguments: * **query** : table<string;string>

.. _function-uriparser_boost_uri_compose_query_in_order_table_ls_string,_string_gr_:

.. das:function:: uri_compose_query_in_order(query: table<string, string>) : string

Compose a query string from a table of key-value pairs, in the sorted order.


:Arguments: * **query** : table<string;string>

.. _function-uriparser_boost_uri_split_full_path_Uri:

.. das:function:: uri_split_full_path(uri: Uri) : array<string>

Split the full path of a URI into its components.


:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>` implicit


+++++++++++++++++++
Component accessors
+++++++++++++++++++

  *  :ref:`fragment (uri: Uri) : string <function-uriparser_boost_fragment_Uri>`
  *  :ref:`host (uri: Uri) : string <function-uriparser_boost_host_Uri>`
  *  :ref:`path (uri: Uri) : string <function-uriparser_boost_path_Uri>`
  *  :ref:`port (uri: Uri) : string <function-uriparser_boost_port_Uri>`
  *  :ref:`query (uri: Uri) : string <function-uriparser_boost_query_Uri>`
  *  :ref:`scheme (uri: Uri) : string <function-uriparser_boost_scheme_Uri>`
  *  :ref:`user_info (uri: Uri) : string <function-uriparser_boost_user_info_Uri>`

.. _function-uriparser_boost_fragment_Uri:

.. das:function:: fragment(uri: Uri) : string

Return the fragment of a URI.


:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>` implicit

.. _function-uriparser_boost_host_Uri:

.. das:function:: host(uri: Uri) : string

Return the host of a URI.


:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>` implicit

.. _function-uriparser_boost_path_Uri:

.. das:function:: path(uri: Uri) : string

Return the path of a URI.


:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>` implicit

.. _function-uriparser_boost_port_Uri:

.. das:function:: port(uri: Uri) : string

Return the port of a URI.


:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>` implicit

.. _function-uriparser_boost_query_Uri:

.. das:function:: query(uri: Uri) : string

Return the query of a URI.


:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>` implicit

.. _function-uriparser_boost_scheme_Uri:

.. das:function:: scheme(uri: Uri) : string

Returns the scheme of a URI.


:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>` implicit

.. _function-uriparser_boost_user_info_Uri:

.. das:function:: user_info(uri: Uri) : string

Return the user info of a URI.


:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>` implicit


