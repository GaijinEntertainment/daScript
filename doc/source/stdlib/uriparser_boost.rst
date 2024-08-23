
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

  *  :ref:`uri_split_full_path (uri: Uri implicit) : array\<string\> <function-_at_uriparser_boost_c__c_uri_split_full_path_CIH_ls_uriparser_c__c_Uri_gr_>` 
  *  :ref:`uri_compose_query (query: table\<string;string\>) : string <function-_at_uriparser_boost_c__c_uri_compose_query_C1_ls_s_gr_2_ls_s_gr_T>` 
  *  :ref:`uri_compose_query_in_order (query: table\<string;string\>) : string <function-_at_uriparser_boost_c__c_uri_compose_query_in_order_C1_ls_s_gr_2_ls_s_gr_T>` 
  *  :ref:`uri_compose (scheme: string; userInfo: string; hostText: string; portText: string; path: string; query: string; fragment: string) : Uri <function-_at_uriparser_boost_c__c_uri_compose_Cs_Cs_Cs_Cs_Cs_Cs_Cs>` 

.. _function-_at_uriparser_boost_c__c_uri_split_full_path_CIH_ls_uriparser_c__c_Uri_gr_:

.. das:function:: uri_split_full_path(uri: Uri implicit) : array<string>

Split the full path of a URI into its components.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-_at_uriparser_boost_c__c_uri_compose_query_C1_ls_s_gr_2_ls_s_gr_T:

.. das:function:: uri_compose_query(query: table<string;string>) : string

Compose a query string from a table of key-value pairs.

:Arguments: * **query** : table<string;string>

.. _function-_at_uriparser_boost_c__c_uri_compose_query_in_order_C1_ls_s_gr_2_ls_s_gr_T:

.. das:function:: uri_compose_query_in_order(query: table<string;string>) : string

Compose a query string from a table of key-value pairs, in the sorted order.

:Arguments: * **query** : table<string;string>

.. _function-_at_uriparser_boost_c__c_uri_compose_Cs_Cs_Cs_Cs_Cs_Cs_Cs:

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

  *  :ref:`scheme (uri: Uri implicit) : string <function-_at_uriparser_boost_c__c_scheme_CIH_ls_uriparser_c__c_Uri_gr_>` 
  *  :ref:`user_info (uri: Uri implicit) : string <function-_at_uriparser_boost_c__c_user_info_CIH_ls_uriparser_c__c_Uri_gr_>` 
  *  :ref:`host (uri: Uri implicit) : string <function-_at_uriparser_boost_c__c_host_CIH_ls_uriparser_c__c_Uri_gr_>` 
  *  :ref:`port (uri: Uri implicit) : string <function-_at_uriparser_boost_c__c_port_CIH_ls_uriparser_c__c_Uri_gr_>` 
  *  :ref:`path (uri: Uri implicit) : string <function-_at_uriparser_boost_c__c_path_CIH_ls_uriparser_c__c_Uri_gr_>` 
  *  :ref:`query (uri: Uri implicit) : string <function-_at_uriparser_boost_c__c_query_CIH_ls_uriparser_c__c_Uri_gr_>` 
  *  :ref:`fragment (uri: Uri implicit) : string <function-_at_uriparser_boost_c__c_fragment_CIH_ls_uriparser_c__c_Uri_gr_>` 

.. _function-_at_uriparser_boost_c__c_scheme_CIH_ls_uriparser_c__c_Uri_gr_:

.. das:function:: scheme(uri: Uri implicit) : string

Returns the scheme of a URI.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-_at_uriparser_boost_c__c_user_info_CIH_ls_uriparser_c__c_Uri_gr_:

.. das:function:: user_info(uri: Uri implicit) : string

Return the user info of a URI.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-_at_uriparser_boost_c__c_host_CIH_ls_uriparser_c__c_Uri_gr_:

.. das:function:: host(uri: Uri implicit) : string

Return the host of a URI.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-_at_uriparser_boost_c__c_port_CIH_ls_uriparser_c__c_Uri_gr_:

.. das:function:: port(uri: Uri implicit) : string

Return the port of a URI.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-_at_uriparser_boost_c__c_path_CIH_ls_uriparser_c__c_Uri_gr_:

.. das:function:: path(uri: Uri implicit) : string

Return the path of a URI.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-_at_uriparser_boost_c__c_query_CIH_ls_uriparser_c__c_Uri_gr_:

.. das:function:: query(uri: Uri implicit) : string

Return the query of a URI.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit

.. _function-_at_uriparser_boost_c__c_fragment_CIH_ls_uriparser_c__c_Uri_gr_:

.. das:function:: fragment(uri: Uri implicit) : string

Return the fragment of a URI.

:Arguments: * **uri** :  :ref:`Uri <handle-uriparser-Uri>`  implicit


