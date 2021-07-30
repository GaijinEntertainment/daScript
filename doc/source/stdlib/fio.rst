
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

  *  :ref:`builtin_dir (arg0:string const implicit;arg1:block\<\> const implicit;arg2:__context const) : void <function-_at_fio_c__c_builtin_dir__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh__hh_block__hh_const_hh___context>` 

.. _function-_at_fio_c__c_builtin_dir__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh__hh_block__hh_const_hh___context:

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

  *  :ref:`fclose (arg0:fio::FILE const? const implicit;arg1:__context const) : void <function-_at_fio_c__c_fclose__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh___context>` 
  *  :ref:`feof (arg0:fio::FILE const? const implicit) : bool <function-_at_fio_c__c_feof__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE>` 
  *  :ref:`fgets (arg0:fio::FILE const? const implicit;arg1:__context const) : string <function-_at_fio_c__c_fgets__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh___context>` 
  *  :ref:`fload (file:fio::FILE const? const;size:int const;blk:block\<(data:array\<uint8\> const):void\> const) : void <function-_at_fio_c__c_fload__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh_int__hh_const_hh__hh_block_hh__hh_const_hh__hh_array_hh_uint8_hh__rq_data_hh__c_void>` 
  *  :ref:`fmap (arg0:fio::FILE const? const implicit;arg1:block\<(array\<uint8\>#):void\> const implicit;arg2:__context const) : void <function-_at_fio_c__c_fmap__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_implicit_hh__hh_block_hh__hh_temporary_hh__hh_array_hh_uint8_hh__c_void__hh_const_hh___context>` 
  *  :ref:`fopen (arg0:string const implicit;arg1:string const implicit) : fio::FILE const? const <function-_at_fio_c__c_fopen__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh_string>` 
  *  :ref:`fprint (arg0:fio::FILE const? const implicit;arg1:string const implicit;arg2:__context const) : void <function-_at_fio_c__c_fprint__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 
  *  :ref:`fread (arg0:fio::FILE const? const implicit;arg1:__context const) : string <function-_at_fio_c__c_fread__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh___context>` 
  *  :ref:`fstat (arg0:fio::FILE const? const implicit;arg1:fio::FStat implicit;arg2:__context const;arg3:__lineInfo const) : bool <function-_at_fio_c__c_fstat__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_implicit_hh__hh_handle_hh_FStat__hh_const_hh___context__hh_const_hh___lineInfo>` 
  *  :ref:`fstderr () : fio::FILE const? const <function-_at_fio_c__c_fstderr>` 
  *  :ref:`fstdin () : fio::FILE const? const <function-_at_fio_c__c_fstdin>` 
  *  :ref:`fstdout () : fio::FILE const? const <function-_at_fio_c__c_fstdout>` 
  *  :ref:`fwrite (arg0:fio::FILE const? const implicit;arg1:string const implicit;arg2:__context const) : void <function-_at_fio_c__c_fwrite__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 
  *  :ref:`stat (arg0:string const implicit;arg1:fio::FStat implicit) : bool <function-_at_fio_c__c_stat__hh_const_hh__hh_implicit_hh_string__hh_implicit_hh__hh_handle_hh_FStat>` 
  *  :ref:`fload (f:fio::FILE const? const;buf:auto(BufType) const -const) : auto <function-_at_fio_c__c_fload__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_auto_hh_BufType>` 
  *  :ref:`fopen (name:string const;mode:string const;blk:block\<(f:fio::FILE const? const):void\> const) : auto <function-_at_fio_c__c_fopen__hh_const_hh_string__hh_const_hh_string__hh_const_hh__hh_block_hh__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE_hh__rq_f_hh__c_void>` 
  *  :ref:`fread (f:fio::FILE const? const;blk:block\<(data:string const#):auto\> const) : auto <function-_at_fio_c__c_fread__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_block_hh__hh_const_hh__hh_temporary_hh_string_hh__rq_data_hh__c__hh_auto>` 
  *  :ref:`fsave (f:fio::FILE const? const;buf:auto(BufType) const) : auto <function-_at_fio_c__c_fsave__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_auto_hh_BufType>` 
  *  :ref:`fstat (f:fio::FILE const? const) : fio::FStat <function-_at_fio_c__c_fstat__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE>` 
  *  :ref:`fwrite (f:fio::FILE const? const;buf:array\<auto(BufType)\> const implicit) : auto <function-_at_fio_c__c_fwrite__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_implicit_hh__hh_array_hh__hh_auto_hh_BufType>` 
  *  :ref:`fwrite (f:fio::FILE const? const;buf:auto(BufType) const implicit) : auto <function-_at_fio_c__c_fwrite__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_implicit_hh__hh_auto_hh_BufType>` 
  *  :ref:`stat (path:string const) : fio::FStat <function-_at_fio_c__c_stat__hh_const_hh_string>` 

.. _function-_at_fio_c__c_fclose__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh___context:

.. das:function:: fclose(arg0: fio::FILE const? const implicit)

+--------+----------------------------------------------------------+
+argument+argument type                                             +
+========+==========================================================+
+arg0    + :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+--------+----------------------------------------------------------+


|function-fio-fclose|

.. _function-_at_fio_c__c_feof__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE:

.. das:function:: feof(arg0: fio::FILE const? const implicit)

feof returns bool

+--------+----------------------------------------------------------+
+argument+argument type                                             +
+========+==========================================================+
+arg0    + :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+--------+----------------------------------------------------------+


|function-fio-feof|

.. _function-_at_fio_c__c_fgets__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh___context:

.. das:function:: fgets(arg0: fio::FILE const? const implicit)

fgets returns string

+--------+----------------------------------------------------------+
+argument+argument type                                             +
+========+==========================================================+
+arg0    + :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+--------+----------------------------------------------------------+


|function-fio-fgets|

.. _function-_at_fio_c__c_fload__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh_int__hh_const_hh__hh_block_hh__hh_const_hh__hh_array_hh_uint8_hh__rq_data_hh__c_void:

.. das:function:: fload(file: file; size: int const; blk: block<(data:array<uint8> const):void> const)

+--------+-------------------------------------------+
+argument+argument type                              +
+========+===========================================+
+file    + :ref:`file <alias-file>`                  +
+--------+-------------------------------------------+
+size    +int const                                  +
+--------+-------------------------------------------+
+blk     +block<(data:array<uint8> const):void> const+
+--------+-------------------------------------------+


|function-fio-fload|

.. _function-_at_fio_c__c_fmap__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_implicit_hh__hh_block_hh__hh_temporary_hh__hh_array_hh_uint8_hh__c_void__hh_const_hh___context:

.. das:function:: fmap(arg0: fio::FILE const? const implicit; arg1: block<(array<uint8>#):void> const implicit)

+--------+----------------------------------------------------------+
+argument+argument type                                             +
+========+==========================================================+
+arg0    + :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+--------+----------------------------------------------------------+
+arg1    +block<(array<uint8>#):void> const implicit                +
+--------+----------------------------------------------------------+


|function-fio-fmap|

.. _function-_at_fio_c__c_fopen__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_implicit_hh_string:

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

.. _function-_at_fio_c__c_fprint__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: fprint(arg0: fio::FILE const? const implicit; arg1: string const implicit)

+--------+----------------------------------------------------------+
+argument+argument type                                             +
+========+==========================================================+
+arg0    + :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+--------+----------------------------------------------------------+
+arg1    +string const implicit                                     +
+--------+----------------------------------------------------------+


|function-fio-fprint|

.. _function-_at_fio_c__c_fread__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh___context:

.. das:function:: fread(arg0: fio::FILE const? const implicit)

fread returns string

+--------+----------------------------------------------------------+
+argument+argument type                                             +
+========+==========================================================+
+arg0    + :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+--------+----------------------------------------------------------+


|function-fio-fread|

.. _function-_at_fio_c__c_fstat__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_implicit_hh__hh_handle_hh_FStat__hh_const_hh___context__hh_const_hh___lineInfo:

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

.. _function-_at_fio_c__c_fwrite__hh_const_hh__hh_implicit_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: fwrite(arg0: fio::FILE const? const implicit; arg1: string const implicit)

+--------+----------------------------------------------------------+
+argument+argument type                                             +
+========+==========================================================+
+arg0    + :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+--------+----------------------------------------------------------+
+arg1    +string const implicit                                     +
+--------+----------------------------------------------------------+


|function-fio-fwrite|

.. _function-_at_fio_c__c_stat__hh_const_hh__hh_implicit_hh_string__hh_implicit_hh__hh_handle_hh_FStat:

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

.. _function-_at_fio_c__c_fload__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_auto_hh_BufType:

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

.. _function-_at_fio_c__c_fopen__hh_const_hh_string__hh_const_hh_string__hh_const_hh__hh_block_hh__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE_hh__rq_f_hh__c_void:

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

.. _function-_at_fio_c__c_fread__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_block_hh__hh_const_hh__hh_temporary_hh_string_hh__rq_data_hh__c__hh_auto:

.. das:function:: fread(f: file; blk: block<(data:string const#):auto> const)

fread returns auto

+--------+--------------------------------------+
+argument+argument type                         +
+========+======================================+
+f       + :ref:`file <alias-file>`             +
+--------+--------------------------------------+
+blk     +block<(data:string const#):auto> const+
+--------+--------------------------------------+


|function-fio-fread|

.. _function-_at_fio_c__c_fsave__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_auto_hh_BufType:

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

.. _function-_at_fio_c__c_fstat__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE:

.. das:function:: fstat(f: file)

fstat returns  :ref:`fio::FStat <handle-fio-FStat>` 

+--------+--------------------------+
+argument+argument type             +
+========+==========================+
+f       + :ref:`file <alias-file>` +
+--------+--------------------------+


|function-fio-fstat|

.. _function-_at_fio_c__c_fwrite__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_implicit_hh__hh_array_hh__hh_auto_hh_BufType:

.. das:function:: fwrite(f: file; buf: array<auto(BufType)> const implicit)

fwrite returns auto

+--------+-----------------------------------+
+argument+argument type                      +
+========+===================================+
+f       + :ref:`file <alias-file>`          +
+--------+-----------------------------------+
+buf     +array<auto(BufType)> const implicit+
+--------+-----------------------------------+


|function-fio-fwrite|

.. _function-_at_fio_c__c_fwrite__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_implicit_hh__hh_auto_hh_BufType:

.. das:function:: fwrite(f: file; buf: auto(BufType) const implicit)

fwrite returns auto

+--------+----------------------------+
+argument+argument type               +
+========+============================+
+f       + :ref:`file <alias-file>`   +
+--------+----------------------------+
+buf     +auto(BufType) const implicit+
+--------+----------------------------+


|function-fio-fwrite|

.. _function-_at_fio_c__c_stat__hh_const_hh_string:

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

  *  :ref:`base_name (arg0:string const implicit;arg1:__context const) : string <function-_at_fio_c__c_base_name__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 
  *  :ref:`dir_name (arg0:string const implicit;arg1:__context const) : string <function-_at_fio_c__c_dir_name__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context>` 

.. _function-_at_fio_c__c_base_name__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

.. das:function:: base_name(arg0: string const implicit)

base_name returns string

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+


|function-fio-base_name|

.. _function-_at_fio_c__c_dir_name__hh_const_hh__hh_implicit_hh_string__hh_const_hh___context:

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

  *  :ref:`mkdir (arg0:string const implicit) : bool <function-_at_fio_c__c_mkdir__hh_const_hh__hh_implicit_hh_string>` 
  *  :ref:`dir (path:string const;blk:block\<(filename:string const):void\> const) : auto <function-_at_fio_c__c_dir__hh_const_hh_string__hh_const_hh__hh_block_hh__hh_const_hh_string_hh__rq_filename_hh__c_void>` 

.. _function-_at_fio_c__c_mkdir__hh_const_hh__hh_implicit_hh_string:

.. das:function:: mkdir(arg0: string const implicit)

mkdir returns bool

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arg0    +string const implicit+
+--------+---------------------+


|function-fio-mkdir|

.. _function-_at_fio_c__c_dir__hh_const_hh_string__hh_const_hh__hh_block_hh__hh_const_hh_string_hh__rq_filename_hh__c_void:

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

  *  :ref:`sleep (arg0:uint const) : void <function-_at_fio_c__c_sleep__hh_const_hh_uint>` 

.. _function-_at_fio_c__c_sleep__hh_const_hh_uint:

.. das:function:: sleep(arg0: uint const)

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +uint const   +
+--------+-------------+


|function-fio-sleep|


