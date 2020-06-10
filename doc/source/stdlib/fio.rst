
.. _stdlib_fio:

=========================
File input output library
=========================

++++++++++
Structures
++++++++++

.. das:attribute:: df_header

it defines as follows ::

  magic : uint
  size  : int

to be documented


++++++++++++++++++
Handled structures
++++++++++++++++++

.. das:attribute:: FILE

to be documented


.. das:attribute:: FStat

it defines as follows ::

  is_valid : bool

it has the following properties ::

  atime  : $::clock
  size   : uint64
  ctime  : $::clock
  is_dir : bool
  mtime  : $::clock
  is_reg : bool

to be documented


++++++++
Generics
++++++++

.. das:function:: dir(path;blk)

dir returns ::

 auto



function arguments are

+----+-----------------------------------------+
+path+string const                             +
+----+-----------------------------------------+
+blk +block<(filename:string const):void> const+
+----+-----------------------------------------+



to be documented


.. das:function:: fload(f;buf)

fload returns ::

 auto



function arguments are

+---+--------------------------+
+f  +fio::FILE const? const    +
+---+--------------------------+
+buf+auto(BufType) const -const+
+---+--------------------------+



to be documented


.. das:function:: fsave(f;buf)

fsave returns ::

 auto



function arguments are

+---+----------------------+
+f  +fio::FILE const? const+
+---+----------------------+
+buf+auto(BufType) const   +
+---+----------------------+



to be documented


+++++++++
Functions
+++++++++

.. das:function:: base_name(arg0;arg1)

base_name returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: builtin_dir(arg0;arg1;arg2)



function arguments are

+----+----------------------+
+arg0+string const          +
+----+----------------------+
+arg1+block<> const implicit+
+----+----------------------+
+arg2+ const                +
+----+----------------------+



to be documented


.. das:function:: dir_name(arg0;arg1)

dir_name returns ::

 string



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: fclose(arg0;arg1)



function arguments are

+----+-------------------------------+
+arg0+fio::FILE const? const implicit+
+----+-------------------------------+
+arg1+ const                         +
+----+-------------------------------+



to be documented


.. das:function:: feof(arg0)

feof returns ::

 bool



function arguments are

+----+-------------------------------+
+arg0+fio::FILE const? const implicit+
+----+-------------------------------+



to be documented


.. das:function:: fgets(arg0;arg1)

fgets returns ::

 string



function arguments are

+----+-------------------------------+
+arg0+fio::FILE const? const implicit+
+----+-------------------------------+
+arg1+ const                         +
+----+-------------------------------+



to be documented


.. das:function:: fmap(arg0;arg1;arg2)



function arguments are

+----+------------------------------------------+
+arg0+fio::FILE const? const implicit           +
+----+------------------------------------------+
+arg1+block<(string const#):void> const implicit+
+----+------------------------------------------+
+arg2+ const                                    +
+----+------------------------------------------+



to be documented


.. das:function:: fopen(arg0;arg1)

fopen returns ::

 fio::FILE const? const



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+string const+
+----+------------+



to be documented


.. das:function:: fopen(name;mode;blk)



function arguments are

+----+--------------------------------------------+
+name+string const                                +
+----+--------------------------------------------+
+mode+string const                                +
+----+--------------------------------------------+
+blk +block<(f:fio::FILE const? const):void> const+
+----+--------------------------------------------+



to be documented


.. das:function:: fprint(arg0;arg1;arg2)



function arguments are

+----+-------------------------------+
+arg0+fio::FILE const? const implicit+
+----+-------------------------------+
+arg1+string const                   +
+----+-------------------------------+
+arg2+ const                         +
+----+-------------------------------+



to be documented


.. das:function:: fread(arg0;arg1)

fread returns ::

 string



function arguments are

+----+-------------------------------+
+arg0+fio::FILE const? const implicit+
+----+-------------------------------+
+arg1+ const                         +
+----+-------------------------------+



to be documented


.. das:function:: fstat(f)

fstat returns ::

 fio::FStat



function arguments are

+-+----------------------+
+f+fio::FILE const? const+
+-+----------------------+



to be documented


.. das:function:: fstat(arg0;arg1)

fstat returns ::

 bool



function arguments are

+----+-------------------------------+
+arg0+fio::FILE const? const implicit+
+----+-------------------------------+
+arg1+fio::FStat implicit            +
+----+-------------------------------+



to be documented


.. das:function:: fstderr

fstderr returns ::

 fio::FILE const? const




to be documented


.. das:function:: fstdin

fstdin returns ::

 fio::FILE const? const




to be documented


.. das:function:: fstdout

fstdout returns ::

 fio::FILE const? const




to be documented


.. das:function:: fwrite(arg0;arg1;arg2)



function arguments are

+----+-------------------------------+
+arg0+fio::FILE const? const implicit+
+----+-------------------------------+
+arg1+string const                   +
+----+-------------------------------+
+arg2+ const                         +
+----+-------------------------------+



to be documented


.. das:function:: mkdir(arg0)

mkdir returns ::

 bool



function arguments are

+----+------------+
+arg0+string const+
+----+------------+



to be documented


.. das:function:: sleep(arg0)



function arguments are

+----+----------+
+arg0+uint const+
+----+----------+



to be documented


.. das:function:: stat(path)

stat returns ::

 fio::FStat



function arguments are

+----+------------+
+path+string const+
+----+------------+



to be documented


.. das:function:: stat(arg0;arg1)

stat returns ::

 bool



function arguments are

+----+-------------------+
+arg0+string const       +
+----+-------------------+
+arg1+fio::FStat implicit+
+----+-------------------+



to be documented



