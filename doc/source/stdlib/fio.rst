
.. _stdlib_fio:

=========================
File input output library
=========================

++++++++++
Structures
++++++++++

.. _struct-fio-df_header:

.. das:attribute:: df_header

df_header fields are

+-----+----+
+magic+uint+
+-----+----+
+size +int +
+-----+----+


to be documented


++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle_fio_FILE:

.. das:attribute:: FILE

to be documented


.. _handle_fio_FStat:

.. das:attribute:: FStat

FStat fields are

+--------+----+
+is_valid+bool+
+--------+----+


FStat properties are

+------+----------------------------------------------+
+atime + :ref:`builtin::clock <handle_builtin_clock>` +
+------+----------------------------------------------+
+size  +uint64                                        +
+------+----------------------------------------------+
+ctime + :ref:`builtin::clock <handle_builtin_clock>` +
+------+----------------------------------------------+
+is_dir+bool                                          +
+------+----------------------------------------------+
+mtime + :ref:`builtin::clock <handle_builtin_clock>` +
+------+----------------------------------------------+
+is_reg+bool                                          +
+------+----------------------------------------------+


to be documented


++++++++
Generics
++++++++

.. das:function:: dir ( path:string const; blk:block<(filename:string const):void> const )  : auto

dir returns auto



function arguments are

+----+-----------------------------------------+
+path+string const                             +
+----+-----------------------------------------+
+blk +block<(filename:string const):void> const+
+----+-----------------------------------------+



to be documented


.. das:function:: fload ( f:fio::FILE const? const; buf:auto(BufType) const )  : auto

fload returns auto



function arguments are

+---+-------------------------------------------------+
+f  + :ref:`fio::FILE <handle_fio_FILE>`  const? const+
+---+-------------------------------------------------+
+buf+auto(BufType) const                              +
+---+-------------------------------------------------+



to be documented


.. das:function:: fsave ( f:fio::FILE const? const; buf:auto(BufType) const )  : auto

fsave returns auto



function arguments are

+---+-------------------------------------------------+
+f  + :ref:`fio::FILE <handle_fio_FILE>`  const? const+
+---+-------------------------------------------------+
+buf+auto(BufType) const                              +
+---+-------------------------------------------------+



to be documented


+++++++++
Functions
+++++++++

.. das:function:: base_name ( arg0:string const )  : string

base_name returns string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+



to be documented


.. das:function:: builtin_dir ( arg0:string const; arg1:block<> const implicit ) 



function arguments are

+----+----------------------+
+arg0+string const          +
+----+----------------------+
+arg1+block<> const implicit+
+----+----------------------+



to be documented


.. das:function:: dir_name ( arg0:string const )  : string

dir_name returns string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+



to be documented


.. das:function:: fclose ( arg0:fio::FILE const? const implicit ) 



function arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle_fio_FILE>`  const? const implicit+
+----+----------------------------------------------------------+



to be documented


.. das:function:: feof ( arg0:fio::FILE const? const implicit )  : bool

feof returns bool



function arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle_fio_FILE>`  const? const implicit+
+----+----------------------------------------------------------+



to be documented


.. das:function:: fgets ( arg0:fio::FILE const? const implicit )  : string

fgets returns string



function arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle_fio_FILE>`  const? const implicit+
+----+----------------------------------------------------------+



to be documented


.. das:function:: fmap ( arg0:fio::FILE const? const implicit; arg1:block<(string const#):void> const implicit ) 



function arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle_fio_FILE>`  const? const implicit+
+----+----------------------------------------------------------+
+arg1+block<(string const#):void> const implicit                +
+----+----------------------------------------------------------+



to be documented


.. das:function:: fopen ( arg0:string const; arg1:string const )  : fio::FILE const? const

fopen returns  :ref:`fio::FILE <handle_fio_FILE>`  const? const



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+



to be documented


.. das:function:: fopen ( name:string const; mode:string const; blk:block<(f:fio::FILE const? const):void> const ) 



function arguments are

+----+-----------------------------------------------------------------------+
+name+string const                                                           +
+----+-----------------------------------------------------------------------+
+mode+string const                                                           +
+----+-----------------------------------------------------------------------+
+blk +block<(f: :ref:`fio::FILE <handle_fio_FILE>`  const? const):void> const+
+----+-----------------------------------------------------------------------+



to be documented


.. das:function:: fprint ( arg0:fio::FILE const? const implicit; arg1:string const ) 



function arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle_fio_FILE>`  const? const implicit+
+----+----------------------------------------------------------+
+arg1+string const                                              +
+----+----------------------------------------------------------+



to be documented


.. das:function:: fread ( arg0:fio::FILE const? const implicit )  : string

fread returns string



function arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle_fio_FILE>`  const? const implicit+
+----+----------------------------------------------------------+



to be documented


.. das:function:: fstat ( f:fio::FILE const? const )  : FStat

fstat returns  :ref:`fio::FStat <handle_fio_FStat>` 



function arguments are

+-+-------------------------------------------------+
+f+ :ref:`fio::FILE <handle_fio_FILE>`  const? const+
+-+-------------------------------------------------+



to be documented


.. das:function:: fstat ( arg0:fio::FILE const? const implicit; arg1:FStat implicit )  : bool

fstat returns bool



function arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle_fio_FILE>`  const? const implicit+
+----+----------------------------------------------------------+
+arg1+ :ref:`fio::FStat <handle_fio_FStat>`  implicit           +
+----+----------------------------------------------------------+



to be documented


.. das:function:: fstderr

fstderr returns  :ref:`fio::FILE <handle_fio_FILE>`  const? const




to be documented


.. das:function:: fstdin

fstdin returns  :ref:`fio::FILE <handle_fio_FILE>`  const? const




to be documented


.. das:function:: fstdout

fstdout returns  :ref:`fio::FILE <handle_fio_FILE>`  const? const




to be documented


.. das:function:: fwrite ( arg0:fio::FILE const? const implicit; arg1:string const ) 



function arguments are

+----+----------------------------------------------------------+
+arg0+ :ref:`fio::FILE <handle_fio_FILE>`  const? const implicit+
+----+----------------------------------------------------------+
+arg1+string const                                              +
+----+----------------------------------------------------------+



to be documented


.. das:function:: mkdir ( arg0:string const )  : bool

mkdir returns bool



function arguments are

+----+------------+
+arg0+string const+
+----+------------+



to be documented


.. das:function:: sleep ( arg0:uint const ) 



function arguments are

+----+----------+
+arg0+uint const+
+----+----------+



to be documented


.. das:function:: stat ( path:string const )  : FStat

stat returns  :ref:`fio::FStat <handle_fio_FStat>` 



function arguments are

+----+------------+
+path+string const+
+----+------------+



to be documented


.. das:function:: stat ( arg0:string const; arg1:FStat implicit )  : bool

stat returns bool



function arguments are

+----+-----------------------------------------------+
+arg0+string const                                   +
+----+-----------------------------------------------+
+arg1+ :ref:`fio::FStat <handle_fio_FStat>`  implicit+
+----+-----------------------------------------------+



to be documented



