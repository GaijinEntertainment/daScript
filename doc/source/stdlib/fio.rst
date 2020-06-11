
.. _stdlib_fio:

=========================
File input output library
=========================

+++++++++++
TypeAliases
+++++++++++

.. _alias-file:

.. das:attribute:: file = file

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

.. das:function:: dir (path:string const; blk:block<(filename:string const):void> const )  : auto

dir returns auto

arguments are

+----+-----------------------------------------+
+path+string const                             +
+----+-----------------------------------------+
+blk +block<(filename:string const):void> const+
+----+-----------------------------------------+


function|fio|dir to be documented

.. das:function:: fload (f:file; buf:auto(BufType) const )  : auto

fload returns auto

arguments are

+---+--------------------------+
+f  + :ref:`file <alias-file>` +
+---+--------------------------+
+buf+auto(BufType) const       +
+---+--------------------------+


function|fio|fload to be documented

.. das:function:: fsave (f:file; buf:auto(BufType) const )  : auto

fsave returns auto

arguments are

+---+--------------------------+
+f  + :ref:`file <alias-file>` +
+---+--------------------------+
+buf+auto(BufType) const       +
+---+--------------------------+


function|fio|fsave to be documented

.. das:function:: base_name (arg0:string const )  : string

base_name returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|fio|base_name to be documented

.. das:function:: builtin_dir (arg0:string const; arg1:block<> const implicit ) 

arguments are

+----+----------------------+
+arg0+string const          +
+----+----------------------+
+arg1+block<> const implicit+
+----+----------------------+


function|fio|builtin_dir to be documented

.. das:function:: dir_name (arg0:string const )  : string

dir_name returns string

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|fio|dir_name to be documented

.. das:function:: fclose (arg0:fio::FILE const? const implicit ) 

arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+----+----------------------------------------------------------+


function|fio|fclose to be documented

.. das:function:: feof (arg0:fio::FILE const? const implicit )  : bool

feof returns bool

arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+----+----------------------------------------------------------+


function|fio|feof to be documented

.. das:function:: fgets (arg0:fio::FILE const? const implicit )  : string

fgets returns string

arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+----+----------------------------------------------------------+


function|fio|fgets to be documented

.. das:function:: fmap (arg0:fio::FILE const? const implicit; arg1:block<(string const#):void> const implicit ) 

arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+----+----------------------------------------------------------+
+arg1+block<(string const#):void> const implicit                +
+----+----------------------------------------------------------+


function|fio|fmap to be documented

.. das:function:: fopen (arg0:string const; arg1:string const )  : fio::FILE const? const

fopen returns  :ref:`fio::FILE <handle-fio-FILE>`  const? const

arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+


function|fio|fopen to be documented

.. das:function:: fopen (name:string const; mode:string const; blk:block<(f:fio::FILE const? const):void> const ) 

arguments are

+----+------------------------------------------------+
+name+string const                                    +
+----+------------------------------------------------+
+mode+string const                                    +
+----+------------------------------------------------+
+blk +block<(f: :ref:`file <alias-file>` ):void> const+
+----+------------------------------------------------+


function|fio|fopen to be documented

.. das:function:: fprint (arg0:fio::FILE const? const implicit; arg1:string const ) 

arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+----+----------------------------------------------------------+
+arg1+string const                                              +
+----+----------------------------------------------------------+


function|fio|fprint to be documented

.. das:function:: fread (arg0:fio::FILE const? const implicit )  : string

fread returns string

arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+----+----------------------------------------------------------+


function|fio|fread to be documented

.. das:function:: fstat (f:file )  : FStat

fstat returns  :ref:`fio::FStat <handle-fio-FStat>` 

arguments are

+-+--------------------------+
+f+ :ref:`file <alias-file>` +
+-+--------------------------+


function|fio|fstat to be documented

.. das:function:: fstat (arg0:fio::FILE const? const implicit; arg1:FStat implicit )  : bool

fstat returns bool

arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+----+----------------------------------------------------------+
+arg1+ :ref:`fio::FStat <handle-fio-FStat>`  implicit           +
+----+----------------------------------------------------------+


function|fio|fstat to be documented

.. das:function:: fstderr ( )  : fio::FILE const? const

fstderr returns  :ref:`fio::FILE <handle-fio-FILE>`  const? const

arguments are

+


function|fio|fstderr to be documented

.. das:function:: fstdin ( )  : fio::FILE const? const

fstdin returns  :ref:`fio::FILE <handle-fio-FILE>`  const? const

arguments are

+


function|fio|fstdin to be documented

.. das:function:: fstdout ( )  : fio::FILE const? const

fstdout returns  :ref:`fio::FILE <handle-fio-FILE>`  const? const

arguments are

+


function|fio|fstdout to be documented

.. das:function:: fwrite (arg0:fio::FILE const? const implicit; arg1:string const ) 

arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle-fio-FILE>`  const? const implicit+
+----+----------------------------------------------------------+
+arg1+string const                                              +
+----+----------------------------------------------------------+


function|fio|fwrite to be documented

.. das:function:: mkdir (arg0:string const )  : bool

mkdir returns bool

arguments are

+----+------------+
+arg0+string const+
+----+------------+


function|fio|mkdir to be documented

.. das:function:: sleep (arg0:uint const ) 

arguments are

+----+----------+
+arg0+uint const+
+----+----------+


function|fio|sleep to be documented

.. das:function:: stat (path:string const )  : FStat

stat returns  :ref:`fio::FStat <handle-fio-FStat>` 

arguments are

+----+------------+
+path+string const+
+----+------------+


function|fio|stat to be documented

.. das:function:: stat (arg0:string const; arg1:FStat implicit )  : bool

stat returns bool

arguments are

+----+-----------------------------------------------+
+arg0+string const                                   +
+----+-----------------------------------------------+
+arg1+ :ref:`fio::FStat <handle-fio-FStat>`  implicit+
+----+-----------------------------------------------+


function|fio|stat to be documented


