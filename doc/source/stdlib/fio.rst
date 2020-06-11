
.. _stdlib_fio:

=========================
File input output library
=========================

+++++++++++
TypeAliases
+++++++++++

.. _alias-file:

.. das:attribute:: file = fio::FILE const?

typedef|fio|file to be documented

.. _struct-fio-df_header:

.. das:attribute:: df_header



df_header fields are

+-----+----+
+magic+uint+
+-----+----+
+size +int +
+-----+----+


structure|fio|df_header to be documented

++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-fio-FILE:

.. das:attribute:: FILE

structure_annotation|fio|FILE to be documented

.. _handle-fio-FStat:

.. das:attribute:: FStat

FStat fields are

+--------+----+
+is_valid+bool+
+--------+----+


FStat properties are

+------+----------------------------------------------+
+atime + :ref:`builtin::clock <handle-builtin-clock>` +
+------+----------------------------------------------+
+size  +uint64                                        +
+------+----------------------------------------------+
+ctime + :ref:`builtin::clock <handle-builtin-clock>` +
+------+----------------------------------------------+
+is_dir+bool                                          +
+------+----------------------------------------------+
+mtime + :ref:`builtin::clock <handle-builtin-clock>` +
+------+----------------------------------------------+
+is_reg+bool                                          +
+------+----------------------------------------------+


structure_annotation|fio|FStat to be documented

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_fio_c__c_base_name_string_hh_const__hh_const:

.. das:function:: base_name(arg0: string const)

base_name returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|fio|base_name to be documented

.. _function-_at_fio_c__c_builtin_dir_string_hh_const__hh_block_hh_const_hh_implicit__hh_const:

.. das:function:: builtin_dir(arg0: string const; arg1: block<> const implicit)

arguments are

+----+----------------------+
+arg0+string const          +
+----+----------------------+
+arg1+block<> const implicit+
+----+----------------------+


function|fio|builtin_dir to be documented

.. _function-_at_fio_c__c_dir_name_string_hh_const__hh_const:

.. das:function:: dir_name(arg0: string const)

dir_name returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|fio|dir_name to be documented

.. _function-_at_fio_c__c_fclose__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit__hh_const:

.. das:function:: fclose(arg0: fio::FILE const? const implicit)

arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+----+----------------------------------------------------------+


function|fio|fclose to be documented

.. _function-_at_fio_c__c_feof__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit:

.. das:function:: feof(arg0: fio::FILE const? const implicit)

feof returns bool

arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+----+----------------------------------------------------------+


function|fio|feof to be documented

.. _function-_at_fio_c__c_fgets__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit__hh_const:

.. das:function:: fgets(arg0: fio::FILE const? const implicit)

fgets returns string

arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+----+----------------------------------------------------------+


function|fio|fgets to be documented

.. _function-_at_fio_c__c_fmap__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit__hh_block_hh_string_hh_const_hh_temporary_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: fmap(arg0: fio::FILE const? const implicit; arg1: block<(string const#):void> const implicit)

arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+----+----------------------------------------------------------+
+arg1+block<(string const#):void> const implicit                +
+----+----------------------------------------------------------+


function|fio|fmap to be documented

.. _function-_at_fio_c__c_fopen_string_hh_const_string_hh_const:

.. das:function:: fopen(arg0: string const; arg1: string const)

fopen returns  :ref:`fio::FILE <handle-fio-FILE>`  const? const

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+


function|fio|fopen to be documented

.. _function-_at_fio_c__c_fopen_string_hh_const_string_hh_const__hh_block_hh__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh__rq_f_hh__c_void_hh_const:

.. das:function:: fopen(name: string const; mode: string const; blk: block<(f:fio::FILE const? const):void> const)

arguments are

+----+------------------------------------------------+
+name+string const                                    +
+----+------------------------------------------------+
+mode+string const                                    +
+----+------------------------------------------------+
+blk +block<(f: :ref:`file <alias-file>` ):void> const+
+----+------------------------------------------------+


function|fio|fopen to be documented

.. _function-_at_fio_c__c_fprint__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit_string_hh_const__hh_const:

.. das:function:: fprint(arg0: fio::FILE const? const implicit; arg1: string const)

arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+----+----------------------------------------------------------+
+arg1+string const                                              +
+----+----------------------------------------------------------+


function|fio|fprint to be documented

.. _function-_at_fio_c__c_fread__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit__hh_const:

.. das:function:: fread(arg0: fio::FILE const? const implicit)

fread returns string

arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+----+----------------------------------------------------------+


function|fio|fread to be documented

.. _function-_at_fio_c__c_fstat__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const:

.. das:function:: fstat(f: file)

fstat returns  :ref:`fio::FStat <handle-fio-FStat>` 

arguments are

+-+--------------------------+
+f+ :ref:`file <alias-file>` +
+-+--------------------------+


function|fio|fstat to be documented

.. _function-_at_fio_c__c_fstat__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit__hh_handle_hh_FStat_hh_implicit:

.. das:function:: fstat(arg0: fio::FILE const? const implicit; arg1: FStat implicit)

fstat returns bool

arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+----+----------------------------------------------------------+
+arg1+ :ref:`fio::FStat <handle-fio-FStat>`  implicit           +
+----+----------------------------------------------------------+


function|fio|fstat to be documented

.. _function-_at_fio_c__c_fstderr:

.. das:function:: fstderr()

fstderr returns  :ref:`fio::FILE <handle-fio-FILE>`  const? const

function|fio|fstderr to be documented

.. _function-_at_fio_c__c_fstdin:

.. das:function:: fstdin()

fstdin returns  :ref:`fio::FILE <handle-fio-FILE>`  const? const

function|fio|fstdin to be documented

.. _function-_at_fio_c__c_fstdout:

.. das:function:: fstdout()

fstdout returns  :ref:`fio::FILE <handle-fio-FILE>`  const? const

function|fio|fstdout to be documented

.. _function-_at_fio_c__c_fwrite__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit_string_hh_const__hh_const:

.. das:function:: fwrite(arg0: fio::FILE const? const implicit; arg1: string const)

arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+----+----------------------------------------------------------+
+arg1+string const                                              +
+----+----------------------------------------------------------+


function|fio|fwrite to be documented

.. _function-_at_fio_c__c_mkdir_string_hh_const:

.. das:function:: mkdir(arg0: string const)

mkdir returns bool

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|fio|mkdir to be documented

.. _function-_at_fio_c__c_sleep_uint_hh_const:

.. das:function:: sleep(arg0: uint const)

arguments are

+----+----------+
+arg0+uint const+
+----+----------+


function|fio|sleep to be documented

.. _function-_at_fio_c__c_stat_string_hh_const:

.. das:function:: stat(path: string const)

stat returns  :ref:`fio::FStat <handle-fio-FStat>` 

arguments are

+----+------------+
+path+string const+
+----+------------+


function|fio|stat to be documented

.. _function-_at_fio_c__c_stat_string_hh_const__hh_handle_hh_FStat_hh_implicit:

.. das:function:: stat(arg0: string const; arg1: FStat implicit)

stat returns bool

arguments are

+----+-----------------------------------------------+
+arg0+string const                                   +
+----+-----------------------------------------------+
+arg1+ :ref:`fio::FStat <handle-fio-FStat>`  implicit+
+----+-----------------------------------------------+


function|fio|stat to be documented

.. _function-_at_fio_c__c_dir_string_hh_const__hh_block_hh_string_hh_const_hh__rq_filename_hh__c_void_hh_const:

.. das:function:: dir(path: string const; blk: block<(filename:string const):void> const)

dir returns auto

arguments are

+----+-----------------------------------------+
+path+string const                             +
+----+-----------------------------------------+
+blk +block<(filename:string const):void> const+
+----+-----------------------------------------+


function|fio|dir to be documented

.. _function-_at_fio_c__c_fload__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const__hh_auto_hh_const:

.. das:function:: fload(f: file; buf: auto(BufType) const)

fload returns auto

arguments are

+---+--------------------------+
+f  + :ref:`file <alias-file>` +
+---+--------------------------+
+buf+auto(BufType) const       +
+---+--------------------------+


function|fio|fload to be documented

.. _function-_at_fio_c__c_fsave__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const__hh_auto_hh_const:

.. das:function:: fsave(f: file; buf: auto(BufType) const)

fsave returns auto

arguments are

+---+--------------------------+
+f  + :ref:`file <alias-file>` +
+---+--------------------------+
+buf+auto(BufType) const       +
+---+--------------------------+


function|fio|fsave to be documented


