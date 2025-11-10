
.. _stdlib_uriparser_boost:

================================
Boost package for the URI parser
================================

The uriparser_boost module implements additional infrastructure for the URI parser.

All functions and symbols are in "uriparser_boost" module, use require to get access to it. ::

    require daslib/uriparser_boost

+++++++++++++++++
Split and compose
+++++++++++++++++

  *  :ref:`uri_split_full_path (uri: Uri implicit) : array\<string\> <function-uriparser_boost_uri_split_full_path_Uri_implicit>` 
  *  :ref:`uri_compose_query (query: table\<string, string\>) : string <function-uriparser_boost_uri_compose_query_table_ls_string,_string_gr_>` 
  *  :ref:`uri_compose_query_in_order (query: table\<string, string\>) : string <function-uriparser_boost_uri_compose_query_in_order_table_ls_string,_string_gr_>` 
  *  :ref:`uri_compose (scheme: string; userInfo: string; hostText: string; portText: string; path: string; query: string; fragment: string) : Uri <function-uriparser_boost_uri_compose_string_string_string_string_string_string_string>` 

.. _function-uriparser_boost_uri_split_full_path_Uri_implicit:

.. das:function:: uri_split_full_path(uri: Uri implicit) : array<string>

Split the full path of a URI into its components.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-uriparser_boost_uri_compose_query_table_ls_string,_string_gr_:

.. das:function:: uri_compose_query(query: table<string, string>) : string

Compose a query string from a table of key-value pairs.

:Arguments: * **query** : table<string;string>

.. _function-uriparser_boost_uri_compose_query_in_order_table_ls_string,_string_gr_:

.. das:function:: uri_compose_query_in_order(query: table<string, string>) : string

Compose a query string from a table of key-value pairs, in the sorted order.

:Arguments: * **query** : table<string;string>

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

+++++++++++++++++++
Component accessors
+++++++++++++++++++

  *  :ref:`scheme (uri: Uri implicit) : string <function-uriparser_boost_scheme_Uri_implicit>` 
  *  :ref:`user_info (uri: Uri implicit) : string <function-uriparser_boost_user_info_Uri_implicit>` 
  *  :ref:`host (uri: Uri implicit) : string <function-uriparser_boost_host_Uri_implicit>` 
  *  :ref:`port (uri: Uri implicit) : string <function-uriparser_boost_port_Uri_implicit>` 
  *  :ref:`path (uri: Uri implicit) : string <function-uriparser_boost_path_Uri_implicit>` 
  *  :ref:`query (uri: Uri implicit) : string <function-uriparser_boost_query_Uri_implicit>` 
  *  :ref:`fragment (uri: Uri implicit) : string <function-uriparser_boost_fragment_Uri_implicit>` 

.. _function-uriparser_boost_scheme_Uri_implicit:

.. das:function:: scheme(uri: Uri implicit) : string

Returns the scheme of a URI.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-uriparser_boost_user_info_Uri_implicit:

.. das:function:: user_info(uri: Uri implicit) : string

Return the user info of a URI.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-uriparser_boost_host_Uri_implicit:

.. das:function:: host(uri: Uri implicit) : string

Return the host of a URI.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-uriparser_boost_port_Uri_implicit:

.. das:function:: port(uri: Uri implicit) : string

Return the port of a URI.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-uriparser_boost_path_Uri_implicit:

.. das:function:: path(uri: Uri implicit) : string

Return the path of a URI.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-uriparser_boost_query_Uri_implicit:

.. das:function:: query(uri: Uri implicit) : string

Return the query of a URI.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-uriparser_boost_fragment_Uri_implicit:

.. das:function:: fragment(uri: Uri implicit) : string

Return the fragment of a URI.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit


