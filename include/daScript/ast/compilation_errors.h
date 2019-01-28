#pragma once

namespace das
{
    enum class CompilationError : int
    {
        unspecified

// lexer errors

    ,   mismatching_parentheses                 =   10001
    ,   mismatching_curly_bracers               =   10002
    ,   string_constant_exceeds_file            =   10003
    ,   string_constant_exceeds_line            =   10004

// parser errors

    ,   syntax_error                            =   20000

// semantic erros

    ,   invalid_type                            =   30101       //  int & a[], int&&, int&*
    ,   invalid_return_type                     =   30102       //  func():blah&
    ,   invalid_argument_type                   =   30103       //  func(a:boxed&)
    ,   invalid_structure_field_type            =   30104       //  struct a:void
    ,   invalid_array_type                      =   30105       //  array<int&>
    ,   invalid_table_type                      =   30106       //  table<wtf,int> table<string&,int> table<int,string&>
    ,   invalid_argument_count                  =   30107       //  assert(), assert(blah,....)
    ,   invalid_variable_type                   =   30108       //  a:void
    ,   invalid_new_type                        =   30109       //  new int&, new int
    ,   invalid_index_type                      =   30110       //  a[wtf]
    ,   invalid_annotation                      =   30111       //  [wtf] a
    ,   invalid_swizzle_mask                    =   30112       //  vec.xxxxx or vec.xAz or vec.wtf

    ,   function_already_declared               =   30201       //  func x .... func x
    ,   argument_already_declared               =   30202       //  func(...,a,...,a,....)
    ,   local_variable_already_declared         =   30203       //  let(...,x,...,x,...)
    ,   global_variable_already_declared        =   30204       //  let(...,x,...,x,...)
    ,   structure_field_already_declared        =   30205       //  struct ... x ... x
    ,   structure_already_declared              =   30206       //  ... struct x ... struct x ...
    ,   structure_already_has_initializer       =   30207       //  struct Foo x = 5; def Foo() ...

    ,   type_not_found                          =   30301       //  a:wtf
    ,   structure_not_found                     =   30302       //  new wtf
    ,   operator_not_found                      =   30303       //  1 + 1.0
    ,   function_not_found                      =   30304       //  wtf(x)
    ,   variable_not_found                      =   30305       //  wtf
    ,   handle_not_found                        =   30306       //  external type has colliding name
    ,   annotation_not_found                    =   30307       //  [wtf] struct or [wtf] function

    ,   cant_initialize                         =   30401       //  block type declaration, default values

    ,   cant_dereference                        =   30501
    ,   cant_index                              =   30502       //  wtf[a]
    ,   cant_get_field                          =   30503       //  wtf.field
    ,   cant_write_to_const                     =   30504       //  const int & a = 5
    ,   cant_move_to_const                      =   30505       //  const array<int> & a <- b
    ,   cant_write_to_non_reference             =   30506       //  1 = blah
    ,   cant_copy                               =   30507       //  a = array<int>(x), expecting <-
    ,   cant_move                               =   30508       //  int a; a <- 1

    ,   condition_must_be_bool                  =   30601       //  if ( 10 ) ...

    ,   cant_pipe                               =   30701       //  wtf <| arg

    ,   invalid_block                           =   30801       //  fn({ return; }), fn ({ break; })

    ,   module_not_found                        =   30901       //  require wtf

    ,   cant_new_handle                         =   31001       //  new Handle

    ,   cant_infer_generic                      =   31100       // TEMPORARY, REMOVE ONCE GENERICS ARE SUPPORTED
    ,   cant_infer_missing_initializer          =   31101       //  let x = 5
    ,   cant_infer_mismatching_restrictions     =   31102       //  let x : auto [5] = int[4][3]

    ,   expecting_return_value                  =   32101       // def blah:int without return
    ,   not_expecting_return_value              =   32102       // def blah:void ... return 12
    ,   invalid_return_semantics                =   32103       // return <- required

    ,   sizeof_reference                        =   39901       //  sizeof(type int&)
    ,   sizeof_auto                             =   39902       //  sizeof(type auto)
    ,   typename_auto                           =   39903       //  typename(type auto)

// logic errors

    ,   static_assert_failed                    =   40100       // static_assert(false)

	,	too_many_infer_passes					=	41000
    };
}
