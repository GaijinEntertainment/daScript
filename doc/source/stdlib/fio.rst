
.. _stdlib_fio:

=========================
File input output library
=========================

The FIO module exposes C++ FILE * API, file mapping,  directory and file stat manipulation routines to Daslang.

All functions and symbols are in "fio" module, use require to get access to it. ::

    require fio


++++++++++++
Type aliases
++++++++++++

.. _alias-file:

.. das:attribute:: file = FILE const?

// stub


+++++++++
Constants
+++++++++

.. _global-fio-seek_set:

.. das:attribute:: seek_set = 0

// stub


.. _global-fio-seek_cur:

.. das:attribute:: seek_cur = 1

// stub


.. _global-fio-seek_end:

.. das:attribute:: seek_end = 2

// stub


.. _global-fio-df_magic:

.. das:attribute:: df_magic = 0x12345678

// stub


++++++++++
Structures
++++++++++

.. _struct-fio-df_header:

.. das:attribute:: df_header

:Fields: * **magic** : uint

         * **size** : int

// stub


++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-fio-FStat:

.. das:attribute:: FStat

.. _function-_at_fio_c__c__dot__rq_size_CIH_ls_fio_c__c_FStat_gr_:

.. das:function:: FStat implicit.size() : uint64

// stub


.. _function-_at_fio_c__c__dot__rq_atime_CIH_ls_fio_c__c_FStat_gr_:

.. das:function:: FStat implicit.atime() : clock

// stub


.. _function-_at_fio_c__c__dot__rq_ctime_CIH_ls_fio_c__c_FStat_gr_:

.. das:function:: FStat implicit.ctime() : clock

// stub


.. _function-_at_fio_c__c__dot__rq_mtime_CIH_ls_fio_c__c_FStat_gr_:

.. das:function:: FStat implicit.mtime() : clock

// stub


.. _function-_at_fio_c__c__dot__rq_is_reg_CIH_ls_fio_c__c_FStat_gr_:

.. das:function:: FStat implicit.is_reg() : bool

// stub


.. _function-_at_fio_c__c__dot__rq_is_dir_CIH_ls_fio_c__c_FStat_gr_:

.. das:function:: FStat implicit.is_dir() : bool

// stub


:Properties: * **size** : uint64

             * **atime** :  :ref:`clock <handle-builtin-clock>` 

             * **ctime** :  :ref:`clock <handle-builtin-clock>` 

             * **mtime** :  :ref:`clock <handle-builtin-clock>` 

             * **is_reg** : bool

             * **is_dir** : bool

:Fields: * **is_valid** : bool

// stub


+++++++++++++
Handled types
+++++++++++++

.. _handle-fio-FILE:

.. das:attribute:: FILE

// stub


+++++++++++++++++
File manipulation
+++++++++++++++++

  *  :ref:`remove (name: string implicit) : bool <function-_at_fio_c__c_remove_CIs>` 
  *  :ref:`rename (old_name: string implicit; new_name: string implicit) : bool <function-_at_fio_c__c_rename_CIs_CIs>` 
  *  :ref:`fopen (name: string implicit; mode: string implicit) : FILE const? <function-_at_fio_c__c_fopen_CIs_CIs>` 
  *  :ref:`fclose (file: FILE const? implicit) <function-_at_fio_c__c_fclose_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__C_c_C_l>` 
  *  :ref:`fflush (file: FILE const? implicit) <function-_at_fio_c__c_fflush_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__C_c_C_l>` 
  *  :ref:`fprint (file: FILE const? implicit; text: string implicit) <function-_at_fio_c__c_fprint_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CIs_C_c_C_l>` 
  *  :ref:`fread (file: FILE const? implicit) : string <function-_at_fio_c__c_fread_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__C_c_C_l>` 
  *  :ref:`fmap (file: FILE const? implicit; block: block\<(var arg0:array\<uint8\>#):void\> implicit) <function-_at_fio_c__c_fmap_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CI0_ls__hh_1_ls_u8_gr_A_gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`fgets (file: FILE const? implicit) : string <function-_at_fio_c__c_fgets_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__C_c_C_l>` 
  *  :ref:`fwrite (file: FILE const? implicit; text: string implicit) <function-_at_fio_c__c_fwrite_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CIs_C_c_C_l>` 
  *  :ref:`feof (file: FILE const? implicit) : bool <function-_at_fio_c__c_feof_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm_>` 
  *  :ref:`fseek (file: FILE const? implicit; offset: int64; mode: int) : int64 <function-_at_fio_c__c_fseek_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__Ci64_Ci_C_c_C_l>` 
  *  :ref:`ftell (file: FILE const? implicit) : int64 <function-_at_fio_c__c_ftell_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__C_c_C_l>` 
  *  :ref:`fstat (file: FILE const? implicit; stat: FStat implicit) : bool <function-_at_fio_c__c_fstat_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__IH_ls_fio_c__c_FStat_gr__C_c_C_l>` 
  *  :ref:`stat (file: string implicit; stat: FStat implicit) : bool <function-_at_fio_c__c_stat_CIs_IH_ls_fio_c__c_FStat_gr_>` 
  *  :ref:`fstdin () : FILE const? <function-_at_fio_c__c_fstdin>` 
  *  :ref:`fstdout () : FILE const? <function-_at_fio_c__c_fstdout>` 
  *  :ref:`fstderr () : FILE const? <function-_at_fio_c__c_fstderr>` 
  *  :ref:`getchar () : int <function-_at_fio_c__c_getchar>` 
  *  :ref:`fload (file: file; size: int; blk: block\<(data:array\<uint8\> const):void\>) <function-_at_fio_c__c_fload_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__Ci_CN_ls_data_gr_0_ls_C1_ls_u8_gr_A_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`fopen (name: string; mode: string; blk: block\<(f:FILE const? const):void\>) : auto <function-_at_fio_c__c_fopen_Cs_Cs_CN_ls_f_gr_0_ls_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`stat (path: string) : FStat <function-_at_fio_c__c_stat_Cs>` 
  *  :ref:`fstat (f: file) : FStat <function-_at_fio_c__c_fstat_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm_>` 
  *  :ref:`fread (f: file; blk: block\<(data:string const#):auto\>) : auto <function-_at_fio_c__c_fread_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CN_ls_data_gr_0_ls_C_hh_s_gr_1_ls__dot__gr__builtin_>` 
  *  :ref:`fload (f: file; var buf: auto(BufType)) : auto <function-_at_fio_c__c_fload_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CY_ls_BufType_gr__dot_>` 
  *  :ref:`fsave (f: file; buf: auto(BufType)) : auto <function-_at_fio_c__c_fsave_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CY_ls_BufType_gr__dot_>` 
  *  :ref:`fread (f: file; buf: auto(BufType) implicit) : auto <function-_at_fio_c__c_fread_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CIY_ls_BufType_gr__dot_>` 
  *  :ref:`fread (f: file; buf: array\<auto(BufType)\> implicit) : auto <function-_at_fio_c__c_fread_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CI1_ls_Y_ls_BufType_gr__dot__gr_A>` 
  *  :ref:`fwrite (f: file; buf: auto(BufType) implicit) : auto <function-_at_fio_c__c_fwrite_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CIY_ls_BufType_gr__dot_>` 
  *  :ref:`fwrite (f: file; buf: array\<auto(BufType)\> implicit) : auto <function-_at_fio_c__c_fwrite_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CI1_ls_Y_ls_BufType_gr__dot__gr_A>` 

.. _function-_at_fio_c__c_remove_CIs:

.. das:function:: remove(name: string implicit) : bool

// stub


:Arguments: * **name** : string implicit

.. _function-_at_fio_c__c_rename_CIs_CIs:

.. das:function:: rename(old_name: string implicit; new_name: string implicit) : bool

// stub


:Arguments: * **old_name** : string implicit

            * **new_name** : string implicit

.. _function-_at_fio_c__c_fopen_CIs_CIs:

.. das:function:: fopen(name: string implicit; mode: string implicit) : FILE const?

// stub


:Arguments: * **name** : string implicit

            * **mode** : string implicit

.. _function-_at_fio_c__c_fclose_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__C_c_C_l:

.. das:function:: fclose(file: FILE const? implicit)

// stub


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

.. _function-_at_fio_c__c_fflush_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__C_c_C_l:

.. das:function:: fflush(file: FILE const? implicit)

// stub


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

.. _function-_at_fio_c__c_fprint_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CIs_C_c_C_l:

.. das:function:: fprint(file: FILE const? implicit; text: string implicit)

// stub


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

            * **text** : string implicit

.. _function-_at_fio_c__c_fread_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__C_c_C_l:

.. das:function:: fread(file: FILE const? implicit) : string

// stub


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

.. _function-_at_fio_c__c_fmap_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CI0_ls__hh_1_ls_u8_gr_A_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: fmap(file: FILE const? implicit; block: block<(var arg0:array<uint8>#):void> implicit)

// stub


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

            * **block** : block<(array<uint8>#):void> implicit

.. _function-_at_fio_c__c_fgets_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__C_c_C_l:

.. das:function:: fgets(file: FILE const? implicit) : string

// stub


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

.. _function-_at_fio_c__c_fwrite_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CIs_C_c_C_l:

.. das:function:: fwrite(file: FILE const? implicit; text: string implicit)

// stub


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

            * **text** : string implicit

.. _function-_at_fio_c__c_feof_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm_:

.. das:function:: feof(file: FILE const? implicit) : bool

// stub


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

.. _function-_at_fio_c__c_fseek_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__Ci64_Ci_C_c_C_l:

.. das:function:: fseek(file: FILE const? implicit; offset: int64; mode: int) : int64

// stub


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

            * **offset** : int64

            * **mode** : int

.. _function-_at_fio_c__c_ftell_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__C_c_C_l:

.. das:function:: ftell(file: FILE const? implicit) : int64

// stub


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

.. _function-_at_fio_c__c_fstat_CI1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__IH_ls_fio_c__c_FStat_gr__C_c_C_l:

.. das:function:: fstat(file: FILE const? implicit; stat: FStat implicit) : bool

// stub


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

            * **stat** :  :ref:`FStat <handle-fio-FStat>`  implicit

.. _function-_at_fio_c__c_stat_CIs_IH_ls_fio_c__c_FStat_gr_:

.. das:function:: stat(file: string implicit; stat: FStat implicit) : bool

// stub


:Arguments: * **file** : string implicit

            * **stat** :  :ref:`FStat <handle-fio-FStat>`  implicit

.. _function-_at_fio_c__c_fstdin:

.. das:function:: fstdin() : FILE const?

// stub


.. _function-_at_fio_c__c_fstdout:

.. das:function:: fstdout() : FILE const?

// stub


.. _function-_at_fio_c__c_fstderr:

.. das:function:: fstderr() : FILE const?

// stub


.. _function-_at_fio_c__c_getchar:

.. das:function:: getchar() : int

// stub


.. _function-_at_fio_c__c_fload_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__Ci_CN_ls_data_gr_0_ls_C1_ls_u8_gr_A_gr_1_ls_v_gr__builtin_:

.. das:function:: fload(file: file; size: int; blk: block<(data:array<uint8> const):void>)

// stub


:Arguments: * **file** :  :ref:`file <alias-file>` 

            * **size** : int

            * **blk** : block<(data:array<uint8>):void>

.. _function-_at_fio_c__c_fopen_Cs_Cs_CN_ls_f_gr_0_ls_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__gr_1_ls_v_gr__builtin_:

.. das:function:: fopen(name: string; mode: string; blk: block<(f:FILE const? const):void>) : auto

// stub


:Arguments: * **name** : string

            * **mode** : string

            * **blk** : block<(f: :ref:`file <alias-file>` ):void>

.. _function-_at_fio_c__c_stat_Cs:

.. das:function:: stat(path: string) : FStat

// stub


:Arguments: * **path** : string

.. _function-_at_fio_c__c_fstat_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm_:

.. das:function:: fstat(f: file) : FStat

// stub


:Arguments: * **f** :  :ref:`file <alias-file>` 

.. _function-_at_fio_c__c_fread_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CN_ls_data_gr_0_ls_C_hh_s_gr_1_ls__dot__gr__builtin_:

.. das:function:: fread(f: file; blk: block<(data:string const#):auto>) : auto

// stub


:Arguments: * **f** :  :ref:`file <alias-file>` 

            * **blk** : block<(data:string#):auto>

.. _function-_at_fio_c__c_fload_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CY_ls_BufType_gr__dot_:

.. das:function:: fload(f: file; buf: auto(BufType)) : auto

// stub


:Arguments: * **f** :  :ref:`file <alias-file>` 

            * **buf** : auto(BufType)

.. _function-_at_fio_c__c_fsave_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CY_ls_BufType_gr__dot_:

.. das:function:: fsave(f: file; buf: auto(BufType)) : auto

// stub


:Arguments: * **f** :  :ref:`file <alias-file>` 

            * **buf** : auto(BufType)

.. _function-_at_fio_c__c_fread_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CIY_ls_BufType_gr__dot_:

.. das:function:: fread(f: file; buf: auto(BufType) implicit) : auto

// stub


:Arguments: * **f** :  :ref:`file <alias-file>` 

            * **buf** : auto(BufType) implicit

.. _function-_at_fio_c__c_fread_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CI1_ls_Y_ls_BufType_gr__dot__gr_A:

.. das:function:: fread(f: file; buf: array<auto(BufType)> implicit) : auto

// stub


:Arguments: * **f** :  :ref:`file <alias-file>` 

            * **buf** : array<auto(BufType)> implicit

.. _function-_at_fio_c__c_fwrite_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CIY_ls_BufType_gr__dot_:

.. das:function:: fwrite(f: file; buf: auto(BufType) implicit) : auto

// stub


:Arguments: * **f** :  :ref:`file <alias-file>` 

            * **buf** : auto(BufType) implicit

.. _function-_at_fio_c__c_fwrite_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__CI1_ls_Y_ls_BufType_gr__dot__gr_A:

.. das:function:: fwrite(f: file; buf: array<auto(BufType)> implicit) : auto

// stub


:Arguments: * **f** :  :ref:`file <alias-file>` 

            * **buf** : array<auto(BufType)> implicit

+++++++++++++++++
Path manipulation
+++++++++++++++++

  *  :ref:`dir_name (name: string implicit) : string <function-_at_fio_c__c_dir_name_CIs_C_c_C_l>` 
  *  :ref:`base_name (name: string implicit) : string <function-_at_fio_c__c_base_name_CIs_C_c_C_l>` 
  *  :ref:`get_full_file_name (path: string implicit) : string <function-_at_fio_c__c_get_full_file_name_CIs_C_c_C_l>` 

.. _function-_at_fio_c__c_dir_name_CIs_C_c_C_l:

.. das:function:: dir_name(name: string implicit) : string

// stub


:Arguments: * **name** : string implicit

.. _function-_at_fio_c__c_base_name_CIs_C_c_C_l:

.. das:function:: base_name(name: string implicit) : string

// stub


:Arguments: * **name** : string implicit

.. _function-_at_fio_c__c_get_full_file_name_CIs_C_c_C_l:

.. das:function:: get_full_file_name(path: string implicit) : string

// stub


:Arguments: * **path** : string implicit

++++++++++++++++++++++
Directory manipulation
++++++++++++++++++++++

  *  :ref:`mkdir (path: string implicit) : bool <function-_at_fio_c__c_mkdir_CIs>` 
  *  :ref:`chdir (path: string implicit) : bool <function-_at_fio_c__c_chdir_CIs>` 
  *  :ref:`getcwd () : string <function-_at_fio_c__c_getcwd_C_c_C_l>` 
  *  :ref:`dir (path: string; blk: block\<(filename:string const):void\>) : auto <function-_at_fio_c__c_dir_Cs_CN_ls_filename_gr_0_ls_Cs_gr_1_ls_v_gr__builtin_>` 

.. _function-_at_fio_c__c_mkdir_CIs:

.. das:function:: mkdir(path: string implicit) : bool

// stub


:Arguments: * **path** : string implicit

.. _function-_at_fio_c__c_chdir_CIs:

.. das:function:: chdir(path: string implicit) : bool

// stub


:Arguments: * **path** : string implicit

.. _function-_at_fio_c__c_getcwd_C_c_C_l:

.. das:function:: getcwd() : string

// stub


.. _function-_at_fio_c__c_dir_Cs_CN_ls_filename_gr_0_ls_Cs_gr_1_ls_v_gr__builtin_:

.. das:function:: dir(path: string; blk: block<(filename:string const):void>) : auto

// stub


:Arguments: * **path** : string

            * **blk** : block<(filename:string):void>

++++++++++++++++++++
OS specific routines
++++++++++++++++++++

  *  :ref:`sleep (msec: uint) <function-_at_fio_c__c_sleep_Cu>` 
  *  :ref:`exit (exitCode: int) <function-_at_fio_c__c_exit_Ci>` 
  *  :ref:`popen (command: string implicit; scope: block\<(arg0:FILE const? const):void\> implicit) : int <function-_at_fio_c__c_popen_CIs_CI0_ls_C1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`popen_binary (command: string implicit; scope: block\<(arg0:FILE const? const):void\> implicit) : int <function-_at_fio_c__c_popen_binary_CIs_CI0_ls_C1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l>` 
  *  :ref:`get_env_variable (var: string implicit) : string <function-_at_fio_c__c_get_env_variable_CIs_C_c_C_l>` 
  *  :ref:`sanitize_command_line (var: string implicit) : string <function-_at_fio_c__c_sanitize_command_line_CIs_C_c_C_l>` 

.. _function-_at_fio_c__c_sleep_Cu:

.. das:function:: sleep(msec: uint)

// stub


:Arguments: * **msec** : uint

.. _function-_at_fio_c__c_exit_Ci:

.. das:function:: exit(exitCode: int)

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **exitCode** : int

.. _function-_at_fio_c__c_popen_CIs_CI0_ls_C1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: popen(command: string implicit; scope: block<(arg0:FILE const? const):void> implicit) : int

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **command** : string implicit

            * **scope** : block<( :ref:`FILE <handle-fio-FILE>` ?):void> implicit

.. _function-_at_fio_c__c_popen_binary_CIs_CI0_ls_C1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: popen_binary(command: string implicit; scope: block<(arg0:FILE const? const):void> implicit) : int

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **command** : string implicit

            * **scope** : block<( :ref:`FILE <handle-fio-FILE>` ?):void> implicit

.. _function-_at_fio_c__c_get_env_variable_CIs_C_c_C_l:

.. das:function:: get_env_variable(var: string implicit) : string

// stub


:Arguments: * **var** : string implicit

.. _function-_at_fio_c__c_sanitize_command_line_CIs_C_c_C_l:

.. das:function:: sanitize_command_line(var: string implicit) : string

// stub


:Arguments: * **var** : string implicit


