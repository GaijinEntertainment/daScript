
.. _stdlib_fio:

=========================
File input output library
=========================

The FIO module implements file input/output and filesystem operations.
It provides functions for reading and writing files (``fopen``, ``fread``, ``fwrite``),
directory management (``mkdir``, ``dir``), path manipulation (``join_path``,
``basename``, ``dirname``), and file metadata queries (``stat``, ``file_time``).

All functions and symbols are in "fio" module, use require to get access to it. ::

    require fio

Example: ::

    require fio

        [export]
        def main() {
            let fname = "_test_fio_tmp.txt"
            fopen(fname, "wb") <| $(f) {
                fwrite(f, "hello, daScript!")
            }
            fopen(fname, "rb") <| $(f) {
                let content = fread(f)
                print("{content}\n")
            }
            remove(fname)
        }
        // output:
        // hello, daScript!

++++++++++++
Type aliases
++++++++++++

.. _alias-file:

.. das:attribute:: file = FILE const?

alias for the `FILE const?`; its there since most file functions expect exactly this type

+++++++++
Constants
+++++++++

.. _global-fio-seek_set:

.. das:attribute:: seek_set = 0

constant for `fseek` which sets the file pointer to the beginning of the file plus the offset.

.. _global-fio-seek_cur:

.. das:attribute:: seek_cur = 1

constant for `fseek` which sets the file pointer to the current position of the file plus the offset.

.. _global-fio-seek_end:

.. das:attribute:: seek_end = 2

constant for `fseek` which sets the file pointer to the end of the file plus the offset.

.. _global-fio-df_magic:

.. das:attribute:: df_magic = 0x12345678

obsolete. magic number for `binary_save` and `binary_load`.

++++++++++
Structures
++++++++++

.. _struct-fio-df_header:

.. das:attribute:: df_header

obsolete. header for the `fsave` and `fload` which internally use `binary_save` and `binary_load`.

:Fields: * **magic** : uint - magic bits, to identify the file type.

         * **size** : int - total size of the saved data (not including this header)


++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-fio-FStat:

.. das:attribute:: FStat

.. _function-fio__dot__rq_size_FStat_implicit:

.. das:function:: FStat implicit.size() : uint64

Returns the size of the file represented by the given FStat object.

.. _function-fio__dot__rq_atime_FStat_implicit:

.. das:function:: FStat implicit.atime() : clock

Returns the atime of the file represented by the given FStat object.

.. _function-fio__dot__rq_ctime_FStat_implicit:

.. das:function:: FStat implicit.ctime() : clock

Returns the ctime of the file represented by the given FStat object.

.. _function-fio__dot__rq_mtime_FStat_implicit:

.. das:function:: FStat implicit.mtime() : clock

Returns the mtime of the file represented by the given FStat object.

.. _function-fio__dot__rq_is_reg_FStat_implicit:

.. das:function:: FStat implicit.is_reg() : bool

Returns true if the file represented by the given FStat object is a regular file.

.. _function-fio__dot__rq_is_dir_FStat_implicit:

.. das:function:: FStat implicit.is_dir() : bool

Returns true if the file represented by the given FStat object is a directory.

:Properties: * **size** : uint64

             * **atime** :  :ref:`clock <handle-builtin-clock>` 

             * **ctime** :  :ref:`clock <handle-builtin-clock>` 

             * **mtime** :  :ref:`clock <handle-builtin-clock>` 

             * **is_reg** : bool

             * **is_dir** : bool

:Fields: * **is_valid** : bool - `stat` and `fstat` return file information in this structure.


+++++++++++++
Handled types
+++++++++++++

.. _handle-fio-FILE:

.. das:attribute:: FILE

Holds system specific `FILE` type.

+++++++++++++++++
File manipulation
+++++++++++++++++

  *  :ref:`remove (name: string implicit) : bool <function-fio_remove_string_implicit>` 
  *  :ref:`rename (old_name: string implicit; new_name: string implicit) : bool <function-fio_rename_string_implicit_string_implicit>` 
  *  :ref:`fopen (name: string implicit; mode: string implicit) : FILE const? <function-fio_fopen_string_implicit_string_implicit>` 
  *  :ref:`fclose (file: FILE const? implicit) <function-fio_fclose_FILE_const_q__implicit>` 
  *  :ref:`fflush (file: FILE const? implicit) <function-fio_fflush_FILE_const_q__implicit>` 
  *  :ref:`fprint (file: FILE const? implicit; text: string implicit) <function-fio_fprint_FILE_const_q__implicit_string_implicit>` 
  *  :ref:`fread (file: FILE const? implicit) : string <function-fio_fread_FILE_const_q__implicit>` 
  *  :ref:`fmap (file: FILE const? implicit; block: block\<(array\<uint8\>#):void\>) <function-fio_fmap_FILE_const_q__implicit_block_ls_array_ls_uint8_gr__hh__c_void_gr_>` 
  *  :ref:`fgets (file: FILE const? implicit) : string <function-fio_fgets_FILE_const_q__implicit>` 
  *  :ref:`fwrite (file: FILE const? implicit; text: string implicit) <function-fio_fwrite_FILE_const_q__implicit_string_implicit>` 
  *  :ref:`feof (file: FILE const? implicit) : bool <function-fio_feof_FILE_const_q__implicit>` 
  *  :ref:`fseek (file: FILE const? implicit; offset: int64; mode: int) : int64 <function-fio_fseek_FILE_const_q__implicit_int64_int>` 
  *  :ref:`ftell (file: FILE const? implicit) : int64 <function-fio_ftell_FILE_const_q__implicit>` 
  *  :ref:`fstat (file: FILE const? implicit; stat: FStat implicit) : bool <function-fio_fstat_FILE_const_q__implicit_FStat_implicit>` 
  *  :ref:`stat (file: string implicit; stat: FStat implicit) : bool <function-fio_stat_string_implicit_FStat_implicit>` 
  *  :ref:`fstdin () : FILE const? <function-fio_fstdin>` 
  *  :ref:`fstdout () : FILE const? <function-fio_fstdout>` 
  *  :ref:`fstderr () : FILE const? <function-fio_fstderr>` 
  *  :ref:`getchar () : int <function-fio_getchar>` 
  *  :ref:`fload (file: file; size: int; blk: block\<(data:array\<uint8\>):void\>) <function-fio_fload_file_int_block_ls_data_c_array_ls_uint8_gr__c_void_gr_>` 
  *  :ref:`fopen (name: string; mode: string; blk: block\<(f:file):void\>) : auto <function-fio_fopen_string_string_block_ls_f_c_file_c_void_gr_>` 
  *  :ref:`stat (path: string) : FStat <function-fio_stat_string>` 
  *  :ref:`fstat (f: file) : FStat <function-fio_fstat_file>` 
  *  :ref:`fread (f: file; blk: block\<(data:string#):auto\>) : auto <function-fio_fread_file_block_ls_data_c_string_hh__c_auto_gr_>` 
  *  :ref:`fload (f: file; var buf: auto(BufType)) : auto <function-fio_fload_file_autoBufType>` 
  *  :ref:`fsave (f: file; buf: auto(BufType)) : auto <function-fio_fsave_file_autoBufType>` 
  *  :ref:`fread (f: file; buf: auto(BufType) implicit) : auto <function-fio_fread_file_autoBufType_implicit>` 
  *  :ref:`fread (f: file; buf: array\<auto(BufType)\>) : auto <function-fio_fread_file_array_ls_autoBufType_gr_>` 
  *  :ref:`fwrite (f: file; buf: auto(BufType) implicit) : auto <function-fio_fwrite_file_autoBufType_implicit>` 
  *  :ref:`fwrite (f: file; buf: array\<auto(BufType)\>) : auto <function-fio_fwrite_file_array_ls_autoBufType_gr_>` 

.. _function-fio_remove_string_implicit:

.. das:function:: remove(name: string implicit) : bool

deletes file specified by name

:Arguments: * **name** : string implicit

.. _function-fio_rename_string_implicit_string_implicit:

.. das:function:: rename(old_name: string implicit; new_name: string implicit) : bool

Renames or moves a file from the source path to the destination path. Returns ``true`` on success.
:Arguments: * **old_name** : string implicit

            * **new_name** : string implicit

.. _function-fio_fopen_string_implicit_string_implicit:

.. das:function:: fopen(name: string implicit; mode: string implicit) : FILE const?

equivalent to C `fopen`. Opens file in different modes.

:Arguments: * **name** : string implicit

            * **mode** : string implicit

.. _function-fio_fclose_FILE_const_q__implicit:

.. das:function:: fclose(file: FILE const? implicit)

equivalent to C `fclose`. Closes file.

:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

.. _function-fio_fflush_FILE_const_q__implicit:

.. das:function:: fflush(file: FILE const? implicit)

equivalent to C `fflush`. Flushes FILE buffers.

:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

.. _function-fio_fprint_FILE_const_q__implicit_string_implicit:

.. das:function:: fprint(file: FILE const? implicit; text: string implicit)

same as `print` but outputs to file.

:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

            * **text** : string implicit

.. _function-fio_fread_FILE_const_q__implicit:

.. das:function:: fread(file: FILE const? implicit) : string

reads data from file.

:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

.. _function-fio_fmap_FILE_const_q__implicit_block_ls_array_ls_uint8_gr__hh__c_void_gr_:

.. das:function:: fmap(file: FILE const? implicit; block: block<(array<uint8>#):void>)

create map view of file, i.e. maps file contents to memory. Data is available as array<uint8> inside the block.

:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

            * **block** : block<(array<uint8>#):void> implicit

.. _function-fio_fgets_FILE_const_q__implicit:

.. das:function:: fgets(file: FILE const? implicit) : string

equivalent to C `fgets`. Reads and returns new string from the line.

:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

.. _function-fio_fwrite_FILE_const_q__implicit_string_implicit:

.. das:function:: fwrite(file: FILE const? implicit; text: string implicit)

writes data fo file.

:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

            * **text** : string implicit

.. _function-fio_feof_FILE_const_q__implicit:

.. das:function:: feof(file: FILE const? implicit) : bool

equivalent to C `feof`. Returns true if end of file has been reached.

:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

.. _function-fio_fseek_FILE_const_q__implicit_int64_int:

.. das:function:: fseek(file: FILE const? implicit; offset: int64; mode: int) : int64

equivalent to C `fseek`. Rewinds position of the current FILE pointer.

:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

            * **offset** : int64

            * **mode** : int

.. _function-fio_ftell_FILE_const_q__implicit:

.. das:function:: ftell(file: FILE const? implicit) : int64

equivalent to C `ftell`. Returns current FILE pointer position.

:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

.. _function-fio_fstat_FILE_const_q__implicit_FStat_implicit:

.. das:function:: fstat(file: FILE const? implicit; stat: FStat implicit) : bool

equivalent to C `fstat`. Returns information about file, such as file size, timestamp, etc.

:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>` ? implicit

            * **stat** :  :ref:`FStat <handle-fio-FStat>`  implicit

.. _function-fio_stat_string_implicit_FStat_implicit:

.. das:function:: stat(file: string implicit; stat: FStat implicit) : bool

same as fstat, but file is specified by file name.

:Arguments: * **file** : string implicit

            * **stat** :  :ref:`FStat <handle-fio-FStat>`  implicit

.. _function-fio_fstdin:

.. das:function:: fstdin() : FILE const?

returns FILE pointer to standard input.

.. _function-fio_fstdout:

.. das:function:: fstdout() : FILE const?

returns FILE pointer to standard output.

.. _function-fio_fstderr:

.. das:function:: fstderr() : FILE const?

returns FILE pointer to standard error.

.. _function-fio_getchar:

.. das:function:: getchar() : int

equivalent to C `getchar`. Reads and returns next character from standard input.

.. _function-fio_fload_file_int_block_ls_data_c_array_ls_uint8_gr__c_void_gr_:

.. das:function:: fload(file: file; size: int; blk: block<(data:array<uint8>):void>)

obsolete. saves data to file.

:Arguments: * **file** :  :ref:`file <alias-file>` 

            * **size** : int

            * **blk** : block<(data:array<uint8>):void>

.. _function-fio_fopen_string_string_block_ls_f_c_file_c_void_gr_:

.. das:function:: fopen(name: string; mode: string; blk: block<(f:file):void>) : auto

equivalent to C `fopen`. Opens file in different modes.

:Arguments: * **name** : string

            * **mode** : string

            * **blk** : block<(f: :ref:`file <alias-file>` ):void>

.. _function-fio_stat_string:

.. das:function:: stat(path: string) : FStat

same as fstat, but file is specified by file name.

:Arguments: * **path** : string

.. _function-fio_fstat_file:

.. das:function:: fstat(f: file) : FStat

equivalent to C `fstat`. Returns information about file, such as file size, timestamp, etc.

:Arguments: * **f** :  :ref:`file <alias-file>` 

.. _function-fio_fread_file_block_ls_data_c_string_hh__c_auto_gr_:

.. das:function:: fread(f: file; blk: block<(data:string#):auto>) : auto

reads data from file.

:Arguments: * **f** :  :ref:`file <alias-file>` 

            * **blk** : block<(data:string#):auto>

.. _function-fio_fload_file_autoBufType:

.. das:function:: fload(f: file; buf: auto(BufType)) : auto

obsolete. saves data to file.

:Arguments: * **f** :  :ref:`file <alias-file>` 

            * **buf** : auto(BufType)

.. _function-fio_fsave_file_autoBufType:

.. das:function:: fsave(f: file; buf: auto(BufType)) : auto

obsolete. loads data from file.

:Arguments: * **f** :  :ref:`file <alias-file>` 

            * **buf** : auto(BufType)

.. _function-fio_fread_file_autoBufType_implicit:

.. das:function:: fread(f: file; buf: auto(BufType) implicit) : auto

reads data from file.

:Arguments: * **f** :  :ref:`file <alias-file>` 

            * **buf** : auto(BufType) implicit

.. _function-fio_fread_file_array_ls_autoBufType_gr_:

.. das:function:: fread(f: file; buf: array<auto(BufType)>) : auto

reads data from file.

:Arguments: * **f** :  :ref:`file <alias-file>` 

            * **buf** : array<auto(BufType)> implicit

.. _function-fio_fwrite_file_autoBufType_implicit:

.. das:function:: fwrite(f: file; buf: auto(BufType) implicit) : auto

writes data fo file.

:Arguments: * **f** :  :ref:`file <alias-file>` 

            * **buf** : auto(BufType) implicit

.. _function-fio_fwrite_file_array_ls_autoBufType_gr_:

.. das:function:: fwrite(f: file; buf: array<auto(BufType)>) : auto

writes data fo file.

:Arguments: * **f** :  :ref:`file <alias-file>` 

            * **buf** : array<auto(BufType)> implicit

+++++++++++++++++
Path manipulation
+++++++++++++++++

  *  :ref:`dir_name (name: string implicit) : string <function-fio_dir_name_string_implicit>` 
  *  :ref:`base_name (name: string implicit) : string <function-fio_base_name_string_implicit>` 
  *  :ref:`get_full_file_name (path: string implicit) : string <function-fio_get_full_file_name_string_implicit>` 

.. _function-fio_dir_name_string_implicit:

.. das:function:: dir_name(name: string implicit) : string

equivalent to linux `dirname`. Splits path and returns the component preceding the final '/'.  Trailing '/' characters are not counted as part of the pathname.

:Arguments: * **name** : string implicit

.. _function-fio_base_name_string_implicit:

.. das:function:: base_name(name: string implicit) : string

equivalent to linux `basename`. Splits path and returns the string up to, but not including, the final '/'.

:Arguments: * **name** : string implicit

.. _function-fio_get_full_file_name_string_implicit:

.. das:function:: get_full_file_name(path: string implicit) : string

returns full name of the file in normalized form.

:Arguments: * **path** : string implicit

++++++++++++++++++++++
Directory manipulation
++++++++++++++++++++++

  *  :ref:`mkdir (path: string implicit) : bool <function-fio_mkdir_string_implicit>` 
  *  :ref:`chdir (path: string implicit) : bool <function-fio_chdir_string_implicit>` 
  *  :ref:`getcwd () : string <function-fio_getcwd>` 
  *  :ref:`dir (path: string; blk: block\<(filename:string):void\>) : auto <function-fio_dir_string_block_ls_filename_c_string_c_void_gr_>` 

.. _function-fio_mkdir_string_implicit:

.. das:function:: mkdir(path: string implicit) : bool

Creates a directory at the specified path. Returns ``true`` if the directory was created successfully.
:Arguments: * **path** : string implicit

.. _function-fio_chdir_string_implicit:

.. das:function:: chdir(path: string implicit) : bool

changes current directory.

:Arguments: * **path** : string implicit

.. _function-fio_getcwd:

.. das:function:: getcwd() : string

returns current working directory.

.. _function-fio_dir_string_block_ls_filename_c_string_c_void_gr_:

.. das:function:: dir(path: string; blk: block<(filename:string):void>) : auto

iterates through all files in the specified `path`.

:Arguments: * **path** : string

            * **blk** : block<(filename:string):void>

++++++++++++++++++++
OS specific routines
++++++++++++++++++++

  *  :ref:`sleep (msec: uint) <function-fio_sleep_uint>` 
  *  :ref:`exit (exitCode: int) <function-fio_exit_int>` 
  *  :ref:`popen (command: string implicit; scope: block\<(FILE const?):void\>) : int <function-fio_popen_string_implicit_block_ls_FILE_const_q__c_void_gr_>` 
  *  :ref:`popen_binary (command: string implicit; scope: block\<(FILE const?):void\>) : int <function-fio_popen_binary_string_implicit_block_ls_FILE_const_q__c_void_gr_>` 
  *  :ref:`get_env_variable (var: string implicit) : string <function-fio_get_env_variable_string_implicit>` 
  *  :ref:`has_env_variable (var: string implicit) : bool <function-fio_has_env_variable_string_implicit>` 
  *  :ref:`sanitize_command_line (var: string implicit) : string <function-fio_sanitize_command_line_string_implicit>` 

.. _function-fio_sleep_uint:

.. das:function:: sleep(msec: uint)

sleeps for specified number of milliseconds.

:Arguments: * **msec** : uint

.. _function-fio_exit_int:

.. das:function:: exit(exitCode: int)

.. warning:: 
  This is unsafe operation.

equivalent to C `exit`. Terminates program.

:Arguments: * **exitCode** : int

.. _function-fio_popen_string_implicit_block_ls_FILE_const_q__c_void_gr_:

.. das:function:: popen(command: string implicit; scope: block<(FILE const?):void>) : int

.. warning:: 
  This is unsafe operation.

equivalent to linux `popen`. Opens pipe to command.

:Arguments: * **command** : string implicit

            * **scope** : block<( :ref:`FILE <handle-fio-FILE>` ?):void> implicit

.. _function-fio_popen_binary_string_implicit_block_ls_FILE_const_q__c_void_gr_:

.. das:function:: popen_binary(command: string implicit; scope: block<(FILE const?):void>) : int

.. warning:: 
  This is unsafe operation.

opens pipe to command and returns FILE pointer to it, in binary mode.

:Arguments: * **command** : string implicit

            * **scope** : block<( :ref:`FILE <handle-fio-FILE>` ?):void> implicit

.. _function-fio_get_env_variable_string_implicit:

.. das:function:: get_env_variable(var: string implicit) : string

returns value of the environment variable.

:Arguments: * **var** : string implicit

.. _function-fio_has_env_variable_string_implicit:

.. das:function:: has_env_variable(var: string implicit) : bool

returns true if the environment variable is defined.

:Arguments: * **var** : string implicit

.. _function-fio_sanitize_command_line_string_implicit:

.. das:function:: sanitize_command_line(var: string implicit) : string

sanitizes command line arguments.

:Arguments: * **var** : string implicit

+++++++++++++
Uncategorized
+++++++++++++

.. _function-fio_register_dynamic_module_string_implicit_string_implicit_int:

.. das:function:: register_dynamic_module(path: string implicit; name: string implicit; on_error: int) : void?

Registers a dynamic (shared library) module by path and name. Loads the module and makes it available for `require`. The `on_error` callback receives error code if loading fails.
:Arguments: * **path** : string implicit

            * **name** : string implicit

            * **on_error** : int

.. _function-fio_register_dynamic_module_string_implicit_string_implicit:

.. das:function:: register_dynamic_module(path: string implicit; name: string implicit) : void?

Registers a dynamic (shared library) module by path and name. Loads the module and makes it available for `require`.
:Arguments: * **path** : string implicit

            * **name** : string implicit

.. _function-fio_register_native_path_string_implicit_string_implicit_string_implicit:

.. das:function:: register_native_path(mod_name: string implicit; src: string implicit; dst: string implicit)

Registers a native path mapping for a module. Maps `src` path prefix to `dst` when resolving module file paths.
:Arguments: * **mod_name** : string implicit

            * **src** : string implicit

            * **dst** : string implicit

.. _function-fio_mkdir_rec_string:

.. das:function:: mkdir_rec(path: string) : bool

Recursively creates directory and all its parents if they do not exist. Returns true if the directory already exists or was created successfully, false otherwise.

:Arguments: * **path** : string


