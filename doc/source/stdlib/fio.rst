
.. _stdlib_fio:

=========================
File input output library
=========================

.. das:module:: fio

The FIO module implements file input/output and filesystem operations.
It provides functions for reading and writing files (``fopen``, ``fread``, ``fwrite``),
directory management (``mkdir``, ``dir``), path manipulation (``join_path``,
``basename``, ``dirname``), and file metadata queries (``stat``, ``file_time``).

All functions and symbols are in "fio" module, use require to get access to it.

.. code-block:: das

    require fio

Example:

.. code-block:: das

    require fio

        [export]
        def main() {
            let fname = "_test_fio_tmp.txt"
            fopen(fname, "wb") $(f) {
                fwrite(f, "hello, daslang!")
            }
            fopen(fname, "rb") $(f) {
                let content = fread(f)
                print("{content}\n")
            }
            remove(fname)
        }
        // output:
        // hello, daslang!



++++++++++++
Type aliases
++++++++++++

.. _alias-file:

.. das:attribute:: file = FILE const?

Type alias for FILE const? used as the standard file handle parameter type across fio functions.


+++++++++
Constants
+++++++++

.. _global-fio-seek_set:

.. das:attribute:: seek_set = 0

Constant for fseek that positions the file pointer relative to the beginning of the file by the given offset.

.. _global-fio-seek_cur:

.. das:attribute:: seek_cur = 1

Constant for fseek that positions the file pointer relative to its current position by the given offset.

.. _global-fio-seek_end:

.. das:attribute:: seek_end = 2

Constant for fseek that positions the file pointer relative to the end of the file by the given offset.

.. _global-fio-df_magic:

.. das:attribute:: df_magic = 0x12345678

Magic number constant used to identify daslang binary file format.
df_magic:uint const



++++++++++
Structures
++++++++++

.. _struct-fio-df_header:

.. das:attribute:: df_header

Obsolete header structure used by fsave and fload for binary serialization with type identification.

:Fields: * **magic** : uint - Magic number constant used to identify and validate the file type.

         * **size** : int - Total size in bytes of the serialized data following this header.




++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-fio-FStat:

.. das:attribute:: FStat

.. _function-fio__dot__rq_size_FStat:

.. das:function:: FStat.size() : uint64

Returns the size of the file in bytes.



.. _function-fio__dot__rq_atime_FStat:

.. das:function:: FStat.atime() : clock

Returns the last access time of the file as a clock value.



.. _function-fio__dot__rq_ctime_FStat:

.. das:function:: FStat.ctime() : clock

Returns the creation time of the file as a clock value.



.. _function-fio__dot__rq_mtime_FStat:

.. das:function:: FStat.mtime() : clock

Returns the last modification time of the file as a clock value.



.. _function-fio__dot__rq_is_reg_FStat:

.. das:function:: FStat.is_reg() : bool

Returns true if the file status indicates a regular file.



.. _function-fio__dot__rq_is_dir_FStat:

.. das:function:: FStat.is_dir() : bool

Returns true if the file status indicates a directory.



:Properties: * **size** : uint64

             * **atime** :  :ref:`clock <handle-builtin-clock>`

             * **ctime** :  :ref:`clock <handle-builtin-clock>`

             * **mtime** :  :ref:`clock <handle-builtin-clock>`

             * **is_reg** : bool

             * **is_dir** : bool

:Fields: * **is_valid** : bool -  `stat` and `fstat` return file information in this structure.




+++++++++++++
Handled types
+++++++++++++

.. _handle-fio-FILE:

.. das:attribute:: FILE

Opaque handle wrapping the platform-specific C FILE type used by all low-level file I/O functions.


+++++++++++++++++
File manipulation
+++++++++++++++++

  *  :ref:`fclose (file: FILE const?) <function-fio_fclose_FILE_const_q_>`
  *  :ref:`feof (file: FILE const?) : bool <function-fio_feof_FILE_const_q_>`
  *  :ref:`fflush (file: FILE const?) <function-fio_fflush_FILE_const_q_>`
  *  :ref:`fgets (file: FILE const?) : string <function-fio_fgets_FILE_const_q_>`
  *  :ref:`fload (file: file; size: int; blk: block\<(data:array\<uint8\>):void\>) <function-fio_fload_file_int_block_ls_data_c_array_ls_uint8_gr__c_void_gr_>`
  *  :ref:`fload (f: file; var buf: auto(BufType)) : auto <function-fio_fload_file_autoBufType_0x40>`
  *  :ref:`fmap (file: FILE const?; block: block\<(array\<uint8\>#):void\>) <function-fio_fmap_FILE_const_q__block_ls_array_ls_uint8_gr__hh__c_void_gr_>`
  *  :ref:`fopen (name: string; mode: string; blk: block\<(f:file):void\>) : auto <function-fio_fopen_string_string_block_ls_f_c_file_c_void_gr_>`
  *  :ref:`fopen (name: string; mode: string) : FILE const? <function-fio_fopen_string_string>`
  *  :ref:`fprint (file: FILE const?; text: string) <function-fio_fprint_FILE_const_q__string>`
  *  :ref:`fread (f: file; buf: auto(BufType)) : auto <function-fio_fread_file_autoBufType_0x66>`
  *  :ref:`fread (file: FILE const?) : string <function-fio_fread_FILE_const_q_>`
  *  :ref:`fread (f: file; buf: array\<auto(BufType)\>) : auto <function-fio_fread_file_array_ls_autoBufType_gr_>`
  *  :ref:`fread (f: file; blk: block\<(data:string#):auto\>) : auto <function-fio_fread_file_block_ls_data_c_string_hh__c_auto_gr_>`
  *  :ref:`fsave (f: file; buf: auto(BufType)) : auto <function-fio_fsave_file_autoBufType_0x53>`
  *  :ref:`fseek (file: FILE const?; offset: int64; mode: int) : int64 <function-fio_fseek_FILE_const_q__int64_int>`
  *  :ref:`fstat (file: FILE const?; stat: FStat) : bool <function-fio_fstat_FILE_const_q__FStat>`
  *  :ref:`fstat (f: file) : FStat <function-fio_fstat_file>`
  *  :ref:`fstderr () : FILE const? <function-fio_fstderr>`
  *  :ref:`fstdin () : FILE const? <function-fio_fstdin>`
  *  :ref:`fstdout () : FILE const? <function-fio_fstdout>`
  *  :ref:`ftell (file: FILE const?) : int64 <function-fio_ftell_FILE_const_q_>`
  *  :ref:`fwrite (f: file; buf: array\<auto(BufType)\>) : auto <function-fio_fwrite_file_array_ls_autoBufType_gr_>`
  *  :ref:`fwrite (file: FILE const?; text: string) <function-fio_fwrite_FILE_const_q__string>`
  *  :ref:`fwrite (f: file; buf: auto(BufType)) : auto <function-fio_fwrite_file_autoBufType_0x74>`
  *  :ref:`getchar () : int <function-fio_getchar>`
  *  :ref:`remove (name: string) : bool <function-fio_remove_string>`
  *  :ref:`rename (old_name: string; new_name: string) : bool <function-fio_rename_string_string>`
  *  :ref:`stat (path: string) : FStat <function-fio_stat_string>`
  *  :ref:`stat (file: string; stat: FStat) : bool <function-fio_stat_string_FStat>`

.. _function-fio_fclose_FILE_const_q_:

.. das:function:: fclose(file: FILE const?)

Closes the given FILE pointer and releases its associated resources, equivalent to C fclose.


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>`? implicit

.. _function-fio_feof_FILE_const_q_:

.. das:function:: feof(file: FILE const?) : bool

Returns true if the end-of-file indicator has been set on the given FILE pointer, equivalent to C feof.


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>`? implicit

.. _function-fio_fflush_FILE_const_q_:

.. das:function:: fflush(file: FILE const?)

Flushes any buffered output data for the given FILE pointer to the underlying file, equivalent to C fflush.


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>`? implicit

.. _function-fio_fgets_FILE_const_q_:

.. das:function:: fgets(file: FILE const?) : string

Reads and returns the next line as a string from the given FILE pointer, equivalent to C fgets.


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>`? implicit


fload
^^^^^

.. _function-fio_fload_file_int_block_ls_data_c_array_ls_uint8_gr__c_void_gr_:

.. das:function:: fload(file: file; size: int; blk: block<(data:array<uint8>):void>)

Obsolete; loads binary data from a file into the provided buffer or passes it as an array of uint8 to a block.


:Arguments: * **file** :  :ref:`file <alias-file>`

            * **size** : int

            * **blk** : block<(data:array<uint8>):void>

.. _function-fio_fload_file_autoBufType_0x40:

.. das:function:: fload(f: file; buf: auto(BufType)) : auto

----

.. _function-fio_fmap_FILE_const_q__block_ls_array_ls_uint8_gr__hh__c_void_gr_:

.. das:function:: fmap(file: FILE const?; block: block<(array<uint8>#):void>)

Memory-maps the contents of the given FILE pointer and provides the data as an array of uint8 inside the block.


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>`? implicit

            * **block** : block<(array<uint8>\ #):void> implicit


fopen
^^^^^

.. _function-fio_fopen_string_string_block_ls_f_c_file_c_void_gr_:

.. das:function:: fopen(name: string; mode: string; blk: block<(f:file):void>) : auto

Opens the file at the given path with the specified mode string, returning a FILE pointer or invoking a block with a file handle.


:Arguments: * **name** : string

            * **mode** : string

            * **blk** : block<(f: :ref:`file <alias-file>`):void>

.. _function-fio_fopen_string_string:

.. das:function:: fopen(name: string; mode: string) : FILE const?

----

.. _function-fio_fprint_FILE_const_q__string:

.. das:function:: fprint(file: FILE const?; text: string)

Writes the given text string to the specified FILE pointer, equivalent to print but targeting a file.


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>`? implicit

            * **text** : string implicit


fread
^^^^^

.. _function-fio_fread_file_autoBufType_0x66:

.. das:function:: fread(f: file; buf: auto(BufType)) : auto

Reads data from a file into a buffer, an array, or returns the full contents as a string, with block-based overloads available.


:Arguments: * **f** :  :ref:`file <alias-file>`

            * **buf** : auto(BufType) implicit

.. _function-fio_fread_FILE_const_q_:

.. das:function:: fread(file: FILE const?) : string

.. _function-fio_fread_file_array_ls_autoBufType_gr_:

.. das:function:: fread(f: file; buf: array<auto(BufType)>) : auto

.. _function-fio_fread_file_block_ls_data_c_string_hh__c_auto_gr_:

.. das:function:: fread(f: file; blk: block<(data:string#):auto>) : auto

----

.. _function-fio_fsave_file_autoBufType_0x53:

.. das:function:: fsave(f: file; buf: auto(BufType)) : auto

Obsolete; saves the provided buffer data to a file in binary format.


:Arguments: * **f** :  :ref:`file <alias-file>`

            * **buf** : auto(BufType)

.. _function-fio_fseek_FILE_const_q__int64_int:

.. das:function:: fseek(file: FILE const?; offset: int64; mode: int) : int64

Repositions the file pointer of the given FILE to the specified offset relative to the mode (seek_set, seek_cur, or seek_end) and returns the new position.


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>`? implicit

            * **offset** : int64

            * **mode** : int


fstat
^^^^^

.. _function-fio_fstat_FILE_const_q__FStat:

.. das:function:: fstat(file: FILE const?; stat: FStat) : bool

Retrieves file metadata such as size and timestamps into an FStat structure from a file handle, equivalent to C fstat.


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>`? implicit

            * **stat** :  :ref:`FStat <handle-fio-FStat>` implicit

.. _function-fio_fstat_file:

.. das:function:: fstat(f: file) : FStat

----

.. _function-fio_fstderr:

.. das:function:: fstderr() : FILE const?

Returns the FILE pointer corresponding to the standard error stream.


.. _function-fio_fstdin:

.. das:function:: fstdin() : FILE const?

Returns the FILE pointer corresponding to the standard input stream.


.. _function-fio_fstdout:

.. das:function:: fstdout() : FILE const?

Returns the FILE pointer corresponding to the standard output stream.


.. _function-fio_ftell_FILE_const_q_:

.. das:function:: ftell(file: FILE const?) : int64

Returns the current byte offset of the file pointer for the given FILE, equivalent to C ftell.


:Arguments: * **file** :  :ref:`FILE <handle-fio-FILE>`? implicit


fwrite
^^^^^^

.. _function-fio_fwrite_file_array_ls_autoBufType_gr_:

.. das:function:: fwrite(f: file; buf: array<auto(BufType)>) : auto

Writes a string, typed buffer, or array of data to the specified file handle.


:Arguments: * **f** :  :ref:`file <alias-file>`

            * **buf** : array<auto(BufType)> implicit

.. _function-fio_fwrite_FILE_const_q__string:

.. das:function:: fwrite(file: FILE const?; text: string)

.. _function-fio_fwrite_file_autoBufType_0x74:

.. das:function:: fwrite(f: file; buf: auto(BufType)) : auto

----

.. _function-fio_getchar:

.. das:function:: getchar() : int

Reads and returns the next character from standard input as an integer, equivalent to C getchar.


.. _function-fio_remove_string:

.. das:function:: remove(name: string) : bool

Deletes the file at the specified path and returns true if it was removed successfully.


:Arguments: * **name** : string implicit

.. _function-fio_rename_string_string:

.. das:function:: rename(old_name: string; new_name: string) : bool

Renames or moves a file from old_name to new_name and returns true on success.


:Arguments: * **old_name** : string implicit

            * **new_name** : string implicit


stat
^^^^

.. _function-fio_stat_string:

.. das:function:: stat(path: string) : FStat

Retrieves file metadata such as size and timestamps for the file at the given path, returning an FStat structure or populating one by reference.


:Arguments: * **path** : string

.. _function-fio_stat_string_FStat:

.. das:function:: stat(file: string; stat: FStat) : bool


+++++++++++++++++
Path manipulation
+++++++++++++++++

  *  :ref:`base_name (name: string) : string <function-fio_base_name_string>`
  *  :ref:`dir_name (name: string) : string <function-fio_dir_name_string>`
  *  :ref:`get_full_file_name (path: string) : string <function-fio_get_full_file_name_string>`

.. _function-fio_base_name_string:

.. das:function:: base_name(name: string) : string

Extracts and returns the final component of a file path, equivalent to POSIX basename.


:Arguments: * **name** : string implicit

.. _function-fio_dir_name_string:

.. das:function:: dir_name(name: string) : string

Extracts and returns the directory component of a file path, equivalent to POSIX dirname.


:Arguments: * **name** : string implicit

.. _function-fio_get_full_file_name_string:

.. das:function:: get_full_file_name(path: string) : string

Returns the fully resolved and normalized absolute path for the given file path string.


:Arguments: * **path** : string implicit


++++++++++++++++++++++
Directory manipulation
++++++++++++++++++++++

  *  :ref:`chdir (path: string) : bool <function-fio_chdir_string>`
  *  :ref:`dir (path: string; blk: block\<(filename:string):void\>) : auto <function-fio_dir_string_block_ls_filename_c_string_c_void_gr_>`
  *  :ref:`getcwd () : string <function-fio_getcwd>`
  *  :ref:`mkdir (path: string) : bool <function-fio_mkdir_string>`
  *  :ref:`mkdir_rec (path: string) : bool <function-fio_mkdir_rec_string>`

.. _function-fio_chdir_string:

.. das:function:: chdir(path: string) : bool

Changes the current working directory to the specified path and returns true on success.


:Arguments: * **path** : string implicit

.. _function-fio_dir_string_block_ls_filename_c_string_c_void_gr_:

.. das:function:: dir(path: string; blk: block<(filename:string):void>) : auto

Iterates over all entries in the directory at the given path, invoking the block with each filename.


:Arguments: * **path** : string

            * **blk** : block<(filename:string):void>

.. _function-fio_getcwd:

.. das:function:: getcwd() : string

Returns the absolute path of the current working directory as a string.


.. _function-fio_mkdir_string:

.. das:function:: mkdir(path: string) : bool

Creates a single directory at the specified path and returns true if it was created successfully.


:Arguments: * **path** : string implicit

.. _function-fio_mkdir_rec_string:

.. das:function:: mkdir_rec(path: string) : bool

Recursively creates the directory at the specified path along with any missing parent directories, returning true on success.


:Arguments: * **path** : string


++++++++++++++++++++
OS specific routines
++++++++++++++++++++

  *  :ref:`exit (exitCode: int) <function-fio_exit_int>`
  *  :ref:`get_env_variable (var: string) : string <function-fio_get_env_variable_string>`
  *  :ref:`has_env_variable (var: string) : bool <function-fio_has_env_variable_string>`
  *  :ref:`popen (command: string; scope: block\<(FILE const?):void\>) : int <function-fio_popen_string_block_ls_FILE_const_q__c_void_gr_>`
  *  :ref:`popen_binary (command: string; scope: block\<(FILE const?):void\>) : int <function-fio_popen_binary_string_block_ls_FILE_const_q__c_void_gr_>`
  *  :ref:`sanitize_command_line (var: string) : string <function-fio_sanitize_command_line_string>`
  *  :ref:`sleep (msec: uint) <function-fio_sleep_uint>`

.. _function-fio_exit_int:

.. das:function:: exit(exitCode: int)

.. warning::
  This is unsafe operation.

Terminates the program immediately with the specified integer exit code, equivalent to C exit.


:Arguments: * **exitCode** : int

.. _function-fio_get_env_variable_string:

.. das:function:: get_env_variable(var: string) : string

Returns the string value of the environment variable with the given name, or an empty string if undefined.


:Arguments: * **var** : string implicit

.. _function-fio_has_env_variable_string:

.. das:function:: has_env_variable(var: string) : bool

Returns true if an environment variable with the given name is defined in the current process environment.


:Arguments: * **var** : string implicit

.. _function-fio_popen_string_block_ls_FILE_const_q__c_void_gr_:

.. das:function:: popen(command: string; scope: block<(FILE const?):void>) : int

.. warning::
  This is unsafe operation.

Opens a pipe to the given shell command, provides the resulting FILE pointer to the block, and returns the process exit code.


:Arguments: * **command** : string implicit

            * **scope** : block<( :ref:`FILE <handle-fio-FILE>`?):void> implicit

.. _function-fio_popen_binary_string_block_ls_FILE_const_q__c_void_gr_:

.. das:function:: popen_binary(command: string; scope: block<(FILE const?):void>) : int

.. warning::
  This is unsafe operation.

Opens a pipe to the given shell command in binary mode, provides the resulting FILE pointer to the block, and returns the process exit code.


:Arguments: * **command** : string implicit

            * **scope** : block<( :ref:`FILE <handle-fio-FILE>`?):void> implicit

.. _function-fio_sanitize_command_line_string:

.. das:function:: sanitize_command_line(var: string) : string

Escapes and sanitizes a command-line argument string to prevent shell injection.


:Arguments: * **var** : string implicit

.. _function-fio_sleep_uint:

.. das:function:: sleep(msec: uint)

Suspends execution of the current thread for the specified number of milliseconds.


:Arguments: * **msec** : uint


+++++++++++++++
Dynamic modules
+++++++++++++++

  *  :ref:`register_dynamic_module (path: string; name: string) : void? <function-fio_register_dynamic_module_string_string>`
  *  :ref:`register_dynamic_module (path: string; name: string; on_error: int) : void? <function-fio_register_dynamic_module_string_string_int>`
  *  :ref:`register_native_path (mod_name: string; src: string; dst: string) <function-fio_register_native_path_string_string_string>`


register_dynamic_module
^^^^^^^^^^^^^^^^^^^^^^^

.. _function-fio_register_dynamic_module_string_string:

.. das:function:: register_dynamic_module(path: string; name: string) : void?

Loads a shared library from the given path and registers it as a daslang module under the specified name, making it available for require.


:Arguments: * **path** : string implicit

            * **name** : string implicit

.. _function-fio_register_dynamic_module_string_string_int:

.. das:function:: register_dynamic_module(path: string; name: string; on_error: int) : void?

----

.. _function-fio_register_native_path_string_string_string:

.. das:function:: register_native_path(mod_name: string; src: string; dst: string)

Registers a path prefix mapping for a module, redirecting file resolution from the src prefix to the dst prefix.


:Arguments: * **mod_name** : string implicit

            * **src** : string implicit

            * **dst** : string implicit


