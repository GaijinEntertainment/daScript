
.. _stdlib_rtti:

================================
Runtime type information library
================================

++++++++++++
Type aliases
++++++++++++

.. das:attribute:: FileAccessPtr = smart_ptr<rtti::FileAccess>

to be documented

.. das:attribute:: ProgramFlags is a bitfield

- failToCompile
- unsafe
- isCompiling
- isSimulating
- isCompilingMacros
- needMacroModule

to be documented

.. das:attribute:: RttiValue is a variant type

it defines as follows ::

 tBool   : bool
 tInt    : int
 tUInt   : uint
 tInt64  : int64
 tUInt64 : uint64
 tFloat  : float
 tDouble : double
 tString : string
 nothing : any

to be documented

++++++++++++
Enumerations
++++++++++++

.. das:attribute:: CompilationError

to be documented

+-----------------------------------+-----+
+unspecified                        +0    +
+-----------------------------------+-----+
+mismatching_parentheses            +10001+
+-----------------------------------+-----+
+mismatching_curly_bracers          +10002+
+-----------------------------------+-----+
+string_constant_exceeds_file       +10003+
+-----------------------------------+-----+
+string_constant_exceeds_line       +10004+
+-----------------------------------+-----+
+unexpected_close_comment           +10005+
+-----------------------------------+-----+
+integer_constant_out_of_range      +10006+
+-----------------------------------+-----+
+comment_contains_eof               +10007+
+-----------------------------------+-----+
+invalid_escape_sequence            +10008+
+-----------------------------------+-----+
+invalid_line_directive             +10009+
+-----------------------------------+-----+
+syntax_error                       +20000+
+-----------------------------------+-----+
+invalid_type                       +30101+
+-----------------------------------+-----+
+invalid_return_type                +30102+
+-----------------------------------+-----+
+invalid_argument_type              +30103+
+-----------------------------------+-----+
+invalid_structure_field_type       +30104+
+-----------------------------------+-----+
+invalid_array_type                 +30105+
+-----------------------------------+-----+
+invalid_table_type                 +30106+
+-----------------------------------+-----+
+invalid_argument_count             +30107+
+-----------------------------------+-----+
+invalid_variable_type              +30108+
+-----------------------------------+-----+
+invalid_new_type                   +30109+
+-----------------------------------+-----+
+invalid_index_type                 +30110+
+-----------------------------------+-----+
+invalid_annotation                 +30111+
+-----------------------------------+-----+
+invalid_swizzle_mask               +30112+
+-----------------------------------+-----+
+invalid_initialization_type        +30113+
+-----------------------------------+-----+
+invalid_with_type                  +30114+
+-----------------------------------+-----+
+invalid_override                   +30115+
+-----------------------------------+-----+
+invalid_name                       +30116+
+-----------------------------------+-----+
+invalid_array_dimension            +30117+
+-----------------------------------+-----+
+invalid_iteration_source           +30118+
+-----------------------------------+-----+
+invalid_loop                       +30119+
+-----------------------------------+-----+
+invalid_label                      +30120+
+-----------------------------------+-----+
+invalid_enumeration                +30121+
+-----------------------------------+-----+
+invalid_option                     +30122+
+-----------------------------------+-----+
+invalid_member_function            +30123+
+-----------------------------------+-----+
+function_already_declared          +30201+
+-----------------------------------+-----+
+argument_already_declared          +30202+
+-----------------------------------+-----+
+local_variable_already_declared    +30203+
+-----------------------------------+-----+
+global_variable_already_declared   +30204+
+-----------------------------------+-----+
+structure_field_already_declared   +30205+
+-----------------------------------+-----+
+structure_already_declared         +30206+
+-----------------------------------+-----+
+structure_already_has_initializer  +30207+
+-----------------------------------+-----+
+enumeration_already_declared       +30208+
+-----------------------------------+-----+
+enumeration_value_already_declared +30209+
+-----------------------------------+-----+
+type_alias_already_declared        +30210+
+-----------------------------------+-----+
+field_already_initialized          +30211+
+-----------------------------------+-----+
+type_not_found                     +30301+
+-----------------------------------+-----+
+structure_not_found                +30302+
+-----------------------------------+-----+
+operator_not_found                 +30303+
+-----------------------------------+-----+
+function_not_found                 +30304+
+-----------------------------------+-----+
+variable_not_found                 +30305+
+-----------------------------------+-----+
+handle_not_found                   +30306+
+-----------------------------------+-----+
+annotation_not_found               +30307+
+-----------------------------------+-----+
+enumeration_not_found              +30308+
+-----------------------------------+-----+
+enumeration_value_not_found        +30309+
+-----------------------------------+-----+
+type_alias_not_found               +30310+
+-----------------------------------+-----+
+bitfield_not_found                 +30311+
+-----------------------------------+-----+
+cant_initialize                    +30401+
+-----------------------------------+-----+
+cant_dereference                   +30501+
+-----------------------------------+-----+
+cant_index                         +30502+
+-----------------------------------+-----+
+cant_get_field                     +30503+
+-----------------------------------+-----+
+cant_write_to_const                +30504+
+-----------------------------------+-----+
+cant_move_to_const                 +30505+
+-----------------------------------+-----+
+cant_write_to_non_reference        +30506+
+-----------------------------------+-----+
+cant_copy                          +30507+
+-----------------------------------+-----+
+cant_move                          +30508+
+-----------------------------------+-----+
+cant_pass_temporary                +30509+
+-----------------------------------+-----+
+condition_must_be_bool             +30601+
+-----------------------------------+-----+
+condition_must_be_static           +30602+
+-----------------------------------+-----+
+cant_pipe                          +30701+
+-----------------------------------+-----+
+invalid_block                      +30801+
+-----------------------------------+-----+
+return_or_break_in_finally         +30802+
+-----------------------------------+-----+
+module_not_found                   +30901+
+-----------------------------------+-----+
+module_already_has_a_name          +30902+
+-----------------------------------+-----+
+cant_new_handle                    +31001+
+-----------------------------------+-----+
+bad_delete                         +31002+
+-----------------------------------+-----+
+cant_infer_generic                 +31100+
+-----------------------------------+-----+
+cant_infer_missing_initializer     +31101+
+-----------------------------------+-----+
+cant_infer_mismatching_restrictions+31102+
+-----------------------------------+-----+
+invalid_cast                       +31200+
+-----------------------------------+-----+
+incompatible_cast                  +31201+
+-----------------------------------+-----+
+unsafe                             +31300+
+-----------------------------------+-----+
+index_out_of_range                 +31400+
+-----------------------------------+-----+
+expecting_return_value             +32101+
+-----------------------------------+-----+
+not_expecting_return_value         +32102+
+-----------------------------------+-----+
+invalid_return_semantics           +32103+
+-----------------------------------+-----+
+invalid_yield                      +32104+
+-----------------------------------+-----+
+typeinfo_reference                 +39901+
+-----------------------------------+-----+
+typeinfo_auto                      +39902+
+-----------------------------------+-----+
+typeinfo_undefined                 +39903+
+-----------------------------------+-----+
+typeinfo_dim                       +39904+
+-----------------------------------+-----+
+typeinfo_macro_error               +39905+
+-----------------------------------+-----+
+static_assert_failed               +40100+
+-----------------------------------+-----+
+run_failed                         +40101+
+-----------------------------------+-----+
+annotation_failed                  +40102+
+-----------------------------------+-----+
+concept_failed                     +40103+
+-----------------------------------+-----+
+not_all_paths_return_value         +40200+
+-----------------------------------+-----+
+assert_with_side_effects           +40201+
+-----------------------------------+-----+
+only_fast_aot_no_cpp_name          +40202+
+-----------------------------------+-----+
+aot_side_effects                   +40203+
+-----------------------------------+-----+
+no_global_heap                     +40204+
+-----------------------------------+-----+
+no_global_variables                +40205+
+-----------------------------------+-----+
+unused_function_argument           +40206+
+-----------------------------------+-----+
+unsafe_function                    +40207+
+-----------------------------------+-----+
+too_many_infer_passes              +41000+
+-----------------------------------+-----+
+missing_node                       +50100+
+-----------------------------------+-----+


.. das:attribute:: ConstMatters

to be documented

+---+-+
+no +0+
+---+-+
+yes+1+
+---+-+


.. das:attribute:: RefMatters

to be documented

+---+-+
+no +0+
+---+-+
+yes+1+
+---+-+


.. das:attribute:: TemporaryMatters

to be documented

+---+-+
+no +0+
+---+-+
+yes+1+
+---+-+


.. das:attribute:: Type

to be documented

+--------------+--+
+none          +0 +
+--------------+--+
+autoinfer     +1 +
+--------------+--+
+alias         +2 +
+--------------+--+
+fakeContext   +3 +
+--------------+--+
+fakeLineInfo  +4 +
+--------------+--+
+anyArgument   +5 +
+--------------+--+
+tVoid         +6 +
+--------------+--+
+tBool         +7 +
+--------------+--+
+tInt64        +12+
+--------------+--+
+tUInt64       +13+
+--------------+--+
+tInt          +14+
+--------------+--+
+tInt2         +15+
+--------------+--+
+tInt3         +16+
+--------------+--+
+tInt4         +17+
+--------------+--+
+tUInt         +18+
+--------------+--+
+tUInt2        +19+
+--------------+--+
+tUInt3        +20+
+--------------+--+
+tUInt4        +21+
+--------------+--+
+tFloat        +22+
+--------------+--+
+tFloat2       +23+
+--------------+--+
+tFloat3       +24+
+--------------+--+
+tFloat4       +25+
+--------------+--+
+tDouble       +26+
+--------------+--+
+tRange        +27+
+--------------+--+
+tURange       +28+
+--------------+--+
+tString       +29+
+--------------+--+
+tStructure    +30+
+--------------+--+
+tHandle       +31+
+--------------+--+
+tEnumeration  +32+
+--------------+--+
+tPointer      +36+
+--------------+--+
+tFunction     +37+
+--------------+--+
+tLambda       +38+
+--------------+--+
+tIterator     +39+
+--------------+--+
+tArray        +40+
+--------------+--+
+tTable        +41+
+--------------+--+
+tBlock        +42+
+--------------+--+
+tInt8         +8 +
+--------------+--+
+tUInt8        +9 +
+--------------+--+
+tInt16        +10+
+--------------+--+
+tUInt16       +11+
+--------------+--+
+tTuple        +43+
+--------------+--+
+tEnumeration8 +33+
+--------------+--+
+tEnumeration16+34+
+--------------+--+
+tVariant      +44+
+--------------+--+
+tBitfield     +35+
+--------------+--+


++++++++++++++++++
Handled structures
++++++++++++++++++

.. das:attribute:: Annotation

it defines as follows ::

  _module : rtti::Module?
  cppName : $::das_string
  name    : $::das_string

it has the following properties ::

  isTypeAnnotation           : bool
  isBasicStructureAnnotation : bool

to be documented


.. das:attribute:: AnnotationArgument

it defines as follows ::

  fValue    : float
  iValue    : int
  name      : $::das_string
  sValue    : $::das_string
  bValue    : bool
  basicType : rtti::Type

to be documented


.. das:attribute:: AnnotationDeclaration

it defines as follows ::

  annotation : smart_ptr<rtti::Annotation>
  arguments  : rtti::AnnotationArgumentList

to be documented


.. das:attribute:: BasicStructureAnnotation

it defines as follows ::

  cppName : $::das_string
  name    : $::das_string

it has the following properties ::

  fieldCount : int

to be documented


.. das:attribute:: EnumInfo

it defines as follows ::

  count : uint
  name  : string
  hash  : uint

to be documented


.. das:attribute:: EnumValueInfo

it defines as follows ::

  value : int64
  name  : string

to be documented


.. das:attribute:: Error

it defines as follows ::

  fixme : $::das_string
  at    : rtti::LineInfo
  what  : $::das_string
  extra : $::das_string
  cerr  : rtti::CompilationError

to be documented


.. das:attribute:: FileAccess

to be documented


.. das:attribute:: FileInfo

it defines as follows ::

  tabSize      : int
  name         : $::das_string
  sourceLength : uint

it has the following properties ::

  source : string const

to be documented


.. das:attribute:: FuncInfo

it defines as follows ::

  stackSize : uint
  result    : rtti::TypeInfo?
  count     : uint
  cppName   : string
  name      : string
  hash      : uint
  flags     : uint

to be documented


.. das:attribute:: LineInfo

it defines as follows ::

  last_column : uint
  line        : uint
  last_line   : uint
  column      : uint
  fileInfo    : rtti::FileInfo?

to be documented


.. das:attribute:: Module

it defines as follows ::

  name : $::das_string

to be documented


.. das:attribute:: Program

it defines as follows ::

  flags  : bitfield<failToCompile;unsafe;isCompiling;isSimulating;isCompilingMacros;needMacroModule>
  errors : rtti::dasvector`Error

to be documented


.. das:attribute:: StructInfo

it defines as follows ::

  size        : uint
  initializer : int
  count       : uint
  name        : string
  hash        : uint

to be documented


.. das:attribute:: TypeAnnotation

it defines as follows ::

  cppName          : $::das_string
  annotationModule : rtti::Module?
  name             : $::das_string

to be documented


.. das:attribute:: TypeInfo

it defines as follows ::

  argTypes   : rtti::TypeInfo??
  secondType : rtti::TypeInfo?
  dimSize    : uint
  enumType   : rtti::EnumInfo?
  hash       : uint
  argNames   : string?
  argCount   : uint
  basicType  : rtti::Type
  firstType  : rtti::TypeInfo?
  structType : rtti::StructInfo?

it has the following properties ::

  isConst    : bool
  annotation : rtti::TypeAnnotation?
  isRawPod   : bool
  isRef      : bool
  isImplicit : bool
  isRefType  : bool
  isPod      : bool
  isTemp     : bool
  isRefValue : bool
  canCopy    : bool

to be documented


.. das:attribute:: VarInfo

it defines as follows ::

  argTypes             : rtti::TypeInfo??
  value                : any
  secondType           : rtti::TypeInfo?
  dimSize              : uint
  enumType             : rtti::EnumInfo?
  name                 : string
  hash                 : uint
  argNames             : string?
  argCount             : uint
  sValue               : string
  offset               : uint
  basicType            : rtti::Type
  firstType            : rtti::TypeInfo?
  annotation_arguments : rtti::AnnotationArguments const? const
  structType           : rtti::StructInfo?

it has the following properties ::

  isConst    : bool
  annotation : rtti::TypeAnnotation?
  isRawPod   : bool
  isRef      : bool
  isImplicit : bool
  isRefType  : bool
  isPod      : bool
  isTemp     : bool
  isRefValue : bool
  canCopy    : bool

to be documented


++++++++
Generics
++++++++

.. das:function:: class_info(cl)

class_info returns ::

 rtti::StructInfo const?



function arguments are

+--+----------+
+cl+auto const+
+--+----------+



to be documented


+++++++++
Functions
+++++++++

.. das:function:: RttiValue_nothing

RttiValue_nothing returns ::

 variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>




to be documented


.. das:function:: arg_names(info)

arg_names returns ::

 iterator<string>



function arguments are

+----+--------------------+
+info+rtti::TypeInfo const+
+----+--------------------+



to be documented


.. das:function:: arg_names(info)

arg_names returns ::

 iterator<string>



function arguments are

+----+-------------------+
+info+rtti::VarInfo const+
+----+-------------------+



to be documented


.. das:function:: arg_types(info)

arg_types returns ::

 iterator<rtti::TypeInfo const&>



function arguments are

+----+--------------------+
+info+rtti::TypeInfo const+
+----+--------------------+



to be documented


.. das:function:: arg_types(info)

arg_types returns ::

 iterator<rtti::TypeInfo const&>



function arguments are

+----+-------------------+
+info+rtti::VarInfo const+
+----+-------------------+



to be documented


.. das:function:: basic_struct_for_each_field(arg0;arg1;arg2)



function arguments are

+----+--------------------------------------------------------------------+
+arg0+rtti::BasicStructureAnnotation const implicit                       +
+----+--------------------------------------------------------------------+
+arg1+block<(string;string;rtti::TypeInfo const;uint):void> const implicit+
+----+--------------------------------------------------------------------+
+arg2+ const                                                              +
+----+--------------------------------------------------------------------+



to be documented


.. das:function:: builtin_is_same_type(arg0;arg1;arg2;arg3;arg4;arg5)

builtin_is_same_type returns ::

 bool



function arguments are

+----+------------------------------------+
+arg0+rtti::TypeInfo const? const implicit+
+----+------------------------------------+
+arg1+rtti::TypeInfo const? const implicit+
+----+------------------------------------+
+arg2+rtti::RefMatters const              +
+----+------------------------------------+
+arg3+rtti::ConstMatters const            +
+----+------------------------------------+
+arg4+rtti::TemporaryMatters const        +
+----+------------------------------------+
+arg5+bool const                          +
+----+------------------------------------+



to be documented


.. das:function:: compile(arg0;arg1;arg2;arg3)



function arguments are

+----+------------------------------------------------------------------------------+
+arg0+string const                                                                  +
+----+------------------------------------------------------------------------------+
+arg1+string const                                                                  +
+----+------------------------------------------------------------------------------+
+arg2+block<(bool;smart_ptr<rtti::Program>;$::das_string const):void> const implicit+
+----+------------------------------------------------------------------------------+
+arg3+ const                                                                        +
+----+------------------------------------------------------------------------------+



to be documented


.. das:function:: compile_file(arg0;arg1;arg2;arg3)



function arguments are

+----+------------------------------------------------------------------------------+
+arg0+string const                                                                  +
+----+------------------------------------------------------------------------------+
+arg1+smart_ptr<rtti::FileAccess> const implicit                                    +
+----+------------------------------------------------------------------------------+
+arg2+block<(bool;smart_ptr<rtti::Program>;$::das_string const):void> const implicit+
+----+------------------------------------------------------------------------------+
+arg3+ const                                                                        +
+----+------------------------------------------------------------------------------+



to be documented


.. das:function:: context_for_each_function(blk)



function arguments are

+---+---------------------------------------------+
+blk+block<(info:rtti::FuncInfo const):void> const+
+---+---------------------------------------------+



to be documented


.. das:function:: context_for_each_variable(blk)



function arguments are

+---+--------------------------------------------+
+blk+block<(info:rtti::VarInfo const):void> const+
+---+--------------------------------------------+



to be documented


.. das:function:: each_dim(info)

each_dim returns ::

 iterator<int>



function arguments are

+----+--------------------+
+info+rtti::TypeInfo const+
+----+--------------------+



to be documented


.. das:function:: each_dim(info)

each_dim returns ::

 iterator<int>



function arguments are

+----+-------------------+
+info+rtti::VarInfo const+
+----+-------------------+



to be documented


.. das:function:: get_annotation_argument_value(arg0;arg1)

get_annotation_argument_value returns ::

 variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>



function arguments are

+----+---------------------------------------+
+arg0+rtti::AnnotationArgument const implicit+
+----+---------------------------------------+
+arg1+ const                                 +
+----+---------------------------------------+



to be documented


.. das:function:: get_das_type_name(arg0;arg1)

get_das_type_name returns ::

 string



function arguments are

+----+----------------+
+arg0+rtti::Type const+
+----+----------------+
+arg1+ const          +
+----+----------------+



to be documented


.. das:function:: get_dim(arg0;arg1;arg2)

get_dim returns ::

 int



function arguments are

+----+-----------------------------+
+arg0+rtti::TypeInfo const implicit+
+----+-----------------------------+
+arg1+int const                    +
+----+-----------------------------+
+arg2+ const                       +
+----+-----------------------------+



to be documented


.. das:function:: get_dim(arg0;arg1;arg2)

get_dim returns ::

 int



function arguments are

+----+----------------------------+
+arg0+rtti::VarInfo const implicit+
+----+----------------------------+
+arg1+int const                   +
+----+----------------------------+
+arg2+ const                      +
+----+----------------------------+



to be documented


.. das:function:: get_function_info(arg0)

get_function_info returns ::

 rtti::FuncInfo const&



function arguments are

+----+---+
+arg0+int+
+----+---+



to be documented


.. das:function:: get_module(arg0)

get_module returns ::

 rtti::Module?



function arguments are

+----+------------+
+arg0+string const+
+----+------------+



to be documented


.. das:function:: get_this_module(arg0)

get_this_module returns ::

 rtti::Module?



function arguments are

+----+---------------------------------------+
+arg0+smart_ptr<rtti::Program> const implicit+
+----+---------------------------------------+



to be documented


.. das:function:: get_total_functions(arg0)

get_total_functions returns ::

 int



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: get_total_variables(arg0)

get_total_variables returns ::

 int



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: get_variable_info(arg0)

get_variable_info returns ::

 rtti::VarInfo const&



function arguments are

+----+---+
+arg0+int+
+----+---+



to be documented


.. das:function:: get_variable_value(arg0)

get_variable_value returns ::

 variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>



function arguments are

+----+----------------------------+
+arg0+rtti::VarInfo const implicit+
+----+----------------------------+



to be documented


.. das:function:: is_compatible_cast(a;b)

is_compatible_cast returns ::

 bool



function arguments are

+-+----------------------+
+a+rtti::StructInfo const+
+-+----------------------+
+b+rtti::StructInfo const+
+-+----------------------+



to be documented


.. das:function:: is_compatible_cast(arg0;arg1)

is_compatible_cast returns ::

 bool



function arguments are

+----+--------------------------------------+
+arg0+rtti::StructInfo const? const implicit+
+----+--------------------------------------+
+arg1+rtti::StructInfo const? const implicit+
+----+--------------------------------------+



to be documented


.. das:function:: is_same_type(a;b;refMatters;constMatters;temporaryMatters;topLevel)

is_same_type returns ::

 bool



function arguments are

+----------------+----------------------------+
+a               +rtti::TypeInfo const        +
+----------------+----------------------------+
+b               +rtti::TypeInfo const        +
+----------------+----------------------------+
+refMatters      +rtti::RefMatters const      +
+----------------+----------------------------+
+constMatters    +rtti::ConstMatters const    +
+----------------+----------------------------+
+temporaryMatters+rtti::TemporaryMatters const+
+----------------+----------------------------+
+topLevel        +bool const                  +
+----------------+----------------------------+



to be documented


.. das:function:: make_file_access(arg0;arg1)

make_file_access returns ::

 smart_ptr<rtti::FileAccess>



function arguments are

+----+------------+
+arg0+string const+
+----+------------+
+arg1+ const      +
+----+------------+



to be documented


.. das:function:: module_for_each_annotation(arg0;arg1;arg2)



function arguments are

+----+---------------------------------------------------+
+arg0+rtti::Module? const implicit                       +
+----+---------------------------------------------------+
+arg1+block<(rtti::Annotation const):void> const implicit+
+----+---------------------------------------------------+
+arg2+ const                                             +
+----+---------------------------------------------------+



to be documented


.. das:function:: module_for_each_enumeration(arg0;arg1;arg2)



function arguments are

+----+-------------------------------------------------+
+arg0+rtti::Module? const implicit                     +
+----+-------------------------------------------------+
+arg1+block<(rtti::EnumInfo const):void> const implicit+
+----+-------------------------------------------------+
+arg2+ const                                           +
+----+-------------------------------------------------+



to be documented


.. das:function:: module_for_each_function(arg0;arg1;arg2)



function arguments are

+----+-------------------------------------------------+
+arg0+rtti::Module? const implicit                     +
+----+-------------------------------------------------+
+arg1+block<(rtti::FuncInfo const):void> const implicit+
+----+-------------------------------------------------+
+arg2+ const                                           +
+----+-------------------------------------------------+



to be documented


.. das:function:: module_for_each_generic(arg0;arg1;arg2)



function arguments are

+----+-------------------------------------------------+
+arg0+rtti::Module? const implicit                     +
+----+-------------------------------------------------+
+arg1+block<(rtti::FuncInfo const):void> const implicit+
+----+-------------------------------------------------+
+arg2+ const                                           +
+----+-------------------------------------------------+



to be documented


.. das:function:: module_for_each_global(arg0;arg1;arg2)



function arguments are

+----+------------------------------------------------+
+arg0+rtti::Module? const implicit                    +
+----+------------------------------------------------+
+arg1+block<(rtti::VarInfo const):void> const implicit+
+----+------------------------------------------------+
+arg2+ const                                          +
+----+------------------------------------------------+



to be documented


.. das:function:: module_for_each_structure(arg0;arg1;arg2)



function arguments are

+----+---------------------------------------------------+
+arg0+rtti::Module? const implicit                       +
+----+---------------------------------------------------+
+arg1+block<(rtti::StructInfo const):void> const implicit+
+----+---------------------------------------------------+
+arg2+ const                                             +
+----+---------------------------------------------------+



to be documented


.. das:function:: program_for_each_module(arg0;arg1;arg2)



function arguments are

+----+------------------------------------------+
+arg0+smart_ptr<rtti::Program> const implicit   +
+----+------------------------------------------+
+arg1+block<(rtti::Module?):void> const implicit+
+----+------------------------------------------+
+arg2+ const                                    +
+----+------------------------------------------+



to be documented


.. das:function:: program_for_each_registered_module(arg0;arg1)



function arguments are

+----+------------------------------------------+
+arg0+block<(rtti::Module?):void> const implicit+
+----+------------------------------------------+
+arg1+ const                                    +
+----+------------------------------------------+



to be documented


.. das:function:: rtti_builtin_structure_for_each_annotation(arg0;arg1;arg2)



function arguments are

+----+-------------------------------+
+arg0+rtti::StructInfo const implicit+
+----+-------------------------------+
+arg1+block<> const implicit         +
+----+-------------------------------+
+arg2+ const                         +
+----+-------------------------------+



to be documented


.. das:function:: set_file_source(arg0;arg1;arg2;arg3)

set_file_source returns ::

 bool



function arguments are

+----+------------------------------------------+
+arg0+smart_ptr<rtti::FileAccess> const implicit+
+----+------------------------------------------+
+arg1+string const                              +
+----+------------------------------------------+
+arg2+string const                              +
+----+------------------------------------------+
+arg3+ const                                    +
+----+------------------------------------------+



to be documented


.. das:function:: structure_for_each_annotation(st;subexpr)



function arguments are

+-------+-----------------------------------------------------------------------------------+
+st     +rtti::StructInfo const                                                             +
+-------+-----------------------------------------------------------------------------------+
+subexpr+block<(ann:rtti::Annotation const;args:rtti::AnnotationArguments const):void> const+
+-------+-----------------------------------------------------------------------------------+



to be documented



