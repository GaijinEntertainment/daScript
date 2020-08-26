
.. _stdlib_fio:

=========================
File input output library
=========================

.. include:: detail/fio.rst

The FIO module exposes C++ FILE * API, file mapping,  directory and file stat manipulation routines to daScript.

All functions and symbols are in "fio" module, use require to get access to it. ::

    require fio


++++++++++++
Type aliases
++++++++++++

.. _alias-file:

.. das:attribute:: file = fio::FILE const?

|typedef-fio-file|

+++++++++
Constants
+++++++++

.. _global-fio-df_magic:

.. das:attribute:: df_magic = 0x12345678

|variable-fio-df_magic|

.. _struct-fio-df_header:

.. das:attribute:: df_header



df_header fields are

+-----+----+
+magic+uint+
+-----+----+
+size +int +
+-----+----+


|structure-fio-df_header|

++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-fio-FILE:

.. das:attribute:: FILE

|structure_annotation-fio-FILE|

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


|structure_annotation-fio-FStat|

++++++++++++++++++++++++++
Internal builtin functions
++++++++++++++++++++++++++

.. warning:: 
  This group of functions is hidden. It will not be in the final documentation.

  *  :ref:`builtin_dir (arg0:string const implicit;arg1:block\<\> const implicit;arg2: const) : void <function-_at_fio_c__c_builtin_dir_string_hh_const_hh_implicit__hh_block_hh_const_hh_implicit__hh_const>` 

.. _function-_at_fio_c__c_builtin_dir_string_hh_const_hh_implicit__hh_block_hh_const_hh_implicit__hh_const:

.. das:function:: builtin_dir(arg0: string const implicit; arg1: block<> const implicit)

+--------+----------------------+
+argument+argument type         +
+========+======================+
+arg0    +string const implicit +
+--------+----------------------+
+arg1    +block<> const implicit+
+--------+----------------------+


|function-fio-builtin_dir|

+++++++++++++++++
File manipulation
+++++++++++++++++

  *  :ref:`fclose (arg0:fio::FILE const? const implicit;arg1: const) : void <function-_at_fio_c__c_fclose__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit__hh_const>` 
  *  :ref:`feof (arg0:fio::FILE const? const implicit) : bool <function-_at_fio_c__c_feof__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit>` 
  *  :ref:`fgets (arg0:fio::FILE const? const implicit;arg1: const) : string <function-_at_fio_c__c_fgets__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit__hh_const>` 
  *  :ref:`fmap (arg0:fio::FILE const? const implicit;arg1:block\<(string const#):void\> const implicit;arg2: const) : void <function-_at_fio_c__c_fmap__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit__hh_block_hh_string_hh_const_hh_temporary_hh__c_void_hh_const_hh_implicit__hh_const>` 
  *  :ref:`fopen (arg0:string const implicit;arg1:string const implicit) : fio::FILE const? const <function-_at_fio_c__c_fopen_string_hh_const_hh_implicit_string_hh_const_hh_implicit>` 
  *  :ref:`fprint (arg0:fio::FILE const? const implicit;arg1:string const implicit;arg2: const) : void <function-_at_fio_c__c_fprint__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit_string_hh_const_hh_implicit__hh_const>` 
  *  :ref:`fread (arg0:fio::FILE const? const implicit;arg1: const) : string <function-_at_fio_c__c_fread__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit__hh_const>` 
  *  :ref:`fstat (arg0:fio::FILE const? const implicit;arg1:fio::FStat implicit) : bool <function-_at_fio_c__c_fstat__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit__hh_handle_hh_FStat_hh_implicit>` 
  *  :ref:`fstderr () : fio::FILE const? const <function-_at_fio_c__c_fstderr>` 
  *  :ref:`fstdin () : fio::FILE const? const <function-_at_fio_c__c_fstdin>` 
  *  :ref:`fstdout () : fio::FILE const? const <function-_at_fio_c__c_fstdout>` 
  *  :ref:`fwrite (arg0:fio::FILE const? const implicit;arg1:string const implicit;arg2: const) : void <function-_at_fio_c__c_fwrite__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit_string_hh_const_hh_implicit__hh_const>` 
  *  :ref:`stat (arg0:string const implicit;arg1:fio::FStat implicit) : bool <function-_at_fio_c__c_stat_string_hh_const_hh_implicit__hh_handle_hh_FStat_hh_implicit>` 
  *  :ref:`fload (f:fio::FILE const? const;buf:auto(BufType) const -const) : auto <function-_at_fio_c__c_fload__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const__hh_auto_hh_const>` 
  *  :ref:`fopen (name:string const;mode:string const;blk:block\<(f:fio::FILE const? const):void\> const) : auto <function-_at_fio_c__c_fopen_string_hh_const_string_hh_const__hh_block_hh__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh__rq_f_hh__c_void_hh_const>` 
  *  :ref:`fsave (f:fio::FILE const? const;buf:auto(BufType) const) : auto <function-_at_fio_c__c_fsave__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const__hh_auto_hh_const>` 
  *  :ref:`fstat (f:fio::FILE const? const) : fio::FStat <function-_at_fio_c__c_fstat__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const>` 
  *  :ref:`stat (path:string const) : fio::FStat <function-_at_fio_c__c_stat_string_hh_const>` 

.. _function-_at_fio_c__c_fclose__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit__hh_const:

.. das:function:: fclose(arg0: fio::FILE const? const implicit)

+--------+----------------------------------------------------------+
+argument+argument type                                             +
+========+==========================================================+
+arg0    + :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+--------+----------------------------------------------------------+


|function-fio-fclose|

.. _function-_at_fio_c__c_feof__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit:

.. das:function:: feof(arg0: fio::FILE const? const implicit)

feof returns bool

+--------+----------------------------------------------------------+
+argument+argument type                                             +
+========+==========================================================+
+arg0    + :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+--------+----------------------------------------------------------+


|function-fio-feof|

.. _function-_at_fio_c__c_fgets__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit__hh_const:

.. das:function:: fgets(arg0: fio::FILE const? const implicit)

fgets returns string

+--------+----------------------------------------------------------+
+argument+argument type                                             +
+========+==========================================================+
+arg0    + :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+--------+----------------------------------------------------------+


|function-fio-fgets|

.. _function-_at_fio_c__c_fmap__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit__hh_block_hh_string_hh_const_hh_temporary_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: fmap(arg0: fio::FILE const? const implicit; arg1: block<(string const#):void> const implicit)

+--------+----------------------------------------------------------+
+argument+argument type                                             +
+========+==========================================================+
+arg0    + :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+--------+----------------------------------------------------------+
+arg1    +block<(string const#):void> const implicit                +
+--------+----------------------------------------------------------+


|function-fio-fmap|

.. _function-_at_fio_c__c_fopen_string_hh_const_hh_implicit_string_hh_const_hh_implicit:

.. das:function:: fopen(arg0: string const implicit; arg1: string const implicit)

fopen returns  :ref:`fio::FILE <handle-fio-FILE>`  const? const

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+
+arg1    +string const implicit+
+--------+---------------------+


|function-fio-fopen|

.. _function-_at_fio_c__c_fprint__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit_string_hh_const_hh_implicit__hh_const:

.. das:function:: fprint(arg0: fio::FILE const? const implicit; arg1: string const implicit)

+--------+----------------------------------------------------------+
+argument+argument type                                             +
+========+==========================================================+
+arg0    + :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+--------+----------------------------------------------------------+
+arg1    +string const implicit                                     +
+--------+----------------------------------------------------------+


|function-fio-fprint|

.. _function-_at_fio_c__c_fread__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit__hh_const:

.. das:function:: fread(arg0: fio::FILE const? const implicit)

fread returns string

+--------+----------------------------------------------------------+
+argument+argument type                                             +
+========+==========================================================+
+arg0    + :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+--------+----------------------------------------------------------+


|function-fio-fread|

.. _function-_at_fio_c__c_fstat__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit__hh_handle_hh_FStat_hh_implicit:

.. das:function:: fstat(arg0: fio::FILE const? const implicit; arg1: FStat implicit)

fstat returns bool

+--------+----------------------------------------------------------+
+argument+argument type                                             +
+========+==========================================================+
+arg0    + :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+--------+----------------------------------------------------------+
+arg1    + :ref:`fio::FStat <handle-fio-FStat>`  implicit           +
+--------+----------------------------------------------------------+


|function-fio-fstat|

.. _function-_at_fio_c__c_fstderr:

.. das:function:: fstderr()

fstderr returns  :ref:`fio::FILE <handle-fio-FILE>`  const? const

|function-fio-fstderr|

.. _function-_at_fio_c__c_fstdin:

.. das:function:: fstdin()

fstdin returns  :ref:`fio::FILE <handle-fio-FILE>`  const? const

|function-fio-fstdin|

.. _function-_at_fio_c__c_fstdout:

.. das:function:: fstdout()

fstdout returns  :ref:`fio::FILE <handle-fio-FILE>`  const? const

|function-fio-fstdout|

.. _function-_at_fio_c__c_fwrite__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh_implicit_string_hh_const_hh_implicit__hh_const:

.. das:function:: fwrite(arg0: fio::FILE const? const implicit; arg1: string const implicit)

+--------+----------------------------------------------------------+
+argument+argument type                                             +
+========+==========================================================+
+arg0    + :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+--------+----------------------------------------------------------+
+arg1    +string const implicit                                     +
+--------+----------------------------------------------------------+


|function-fio-fwrite|

.. _function-_at_fio_c__c_stat_string_hh_const_hh_implicit__hh_handle_hh_FStat_hh_implicit:

.. das:function:: stat(arg0: string const implicit; arg1: FStat implicit)

stat returns bool

+--------+-----------------------------------------------+
+argument+argument type                                  +
+========+===============================================+
+arg0    +string const implicit                          +
+--------+-----------------------------------------------+
+arg1    + :ref:`fio::FStat <handle-fio-FStat>`  implicit+
+--------+-----------------------------------------------+


|function-fio-stat|

.. _function-_at_fio_c__c_fload__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const__hh_auto_hh_const:

.. das:function:: fload(f: file; buf: auto(BufType) const)

fload returns auto

+--------+--------------------------+
+argument+argument type             +
+========+==========================+
+f       + :ref:`file <alias-file>` +
+--------+--------------------------+
+buf     +auto(BufType) const       +
+--------+--------------------------+


|function-fio-fload|

.. _function-_at_fio_c__c_fopen_string_hh_const_string_hh_const__hh_block_hh__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const_hh__rq_f_hh__c_void_hh_const:

.. das:function:: fopen(name: string const; mode: string const; blk: block<(f:fio::FILE const? const):void> const)

fopen returns auto

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+name    +string const                                    +
+--------+------------------------------------------------+
+mode    +string const                                    +
+--------+------------------------------------------------+
+blk     +block<(f: :ref:`file <alias-file>` ):void> const+
+--------+------------------------------------------------+


|function-fio-fopen|

.. _function-_at_fio_c__c_fsave__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const__hh_auto_hh_const:

.. das:function:: fsave(f: file; buf: auto(BufType) const)

fsave returns auto

+--------+--------------------------+
+argument+argument type             +
+========+==========================+
+f       + :ref:`file <alias-file>` +
+--------+--------------------------+
+buf     +auto(BufType) const       +
+--------+--------------------------+


|function-fio-fsave|

.. _function-_at_fio_c__c_fstat__hh_ptr_hh__hh_handle_hh_FILE_hh_const_hh_const:

.. das:function:: fstat(f: file)

fstat returns  :ref:`fio::FStat <handle-fio-FStat>` 

+--------+--------------------------+
+argument+argument type             +
+========+==========================+
+f       + :ref:`file <alias-file>` +
+--------+--------------------------+


|function-fio-fstat|

.. _function-_at_fio_c__c_stat_string_hh_const:

.. das:function:: stat(path: string const)

stat returns  :ref:`fio::FStat <handle-fio-FStat>` 

+--------+-------------+
+argument+argument type+
+========+=============+
+path    +string const +
+--------+-------------+


|function-fio-stat|

+++++++++++++++++
Path manipulation
+++++++++++++++++

  *  :ref:`base_name (arg0:string const implicit;arg1: const) : string <function-_at_fio_c__c_base_name_string_hh_const_hh_implicit__hh_const>` 
  *  :ref:`dir_name (arg0:string const implicit;arg1: const) : string <function-_at_fio_c__c_dir_name_string_hh_const_hh_implicit__hh_const>` 

.. _function-_at_fio_c__c_base_name_string_hh_const_hh_implicit__hh_const:

.. das:function:: base_name(arg0: string const implicit)

base_name returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+


|function-fio-base_name|

.. _function-_at_fio_c__c_dir_name_string_hh_const_hh_implicit__hh_const:

.. das:function:: dir_name(arg0: string const implicit)

dir_name returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+


|function-fio-dir_name|

++++++++++++++++++++++
Directory manipulation
++++++++++++++++++++++

  *  :ref:`mkdir (arg0:string const implicit) : bool <function-_at_fio_c__c_mkdir_string_hh_const_hh_implicit>` 
  *  :ref:`dir (path:string const;blk:block\<(filename:string const):void\> const) : auto <function-_at_fio_c__c_dir_string_hh_const__hh_block_hh_string_hh_const_hh__rq_filename_hh__c_void_hh_const>` 

.. _function-_at_fio_c__c_mkdir_string_hh_const_hh_implicit:

.. das:function:: mkdir(arg0: string const implicit)

mkdir returns bool

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+


|function-fio-mkdir|

.. _function-_at_fio_c__c_dir_string_hh_const__hh_block_hh_string_hh_const_hh__rq_filename_hh__c_void_hh_const:

.. das:function:: dir(path: string const; blk: block<(filename:string const):void> const)

dir returns auto

+--------+-----------------------------------------+
+argument+argument type                            +
+========+=========================================+
+path    +string const                             +
+--------+-----------------------------------------+
+blk     +block<(filename:string const):void> const+
+--------+-----------------------------------------+


|function-fio-dir|

++++++++++++++++
OS thread helper
++++++++++++++++

  *  :ref:`sleep (arg0:uint const) : void <function-_at_fio_c__c_sleep_uint_hh_const>` 

.. _function-_at_fio_c__c_sleep_uint_hh_const:

.. das:function:: sleep(arg0: uint const)

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +uint const   +
+--------+-------------+


|function-fio-sleep|


