
.. _stdlib_ast_cursor:

==========================
Position-based AST queries
==========================

.. das:module:: ast_cursor

Position-based AST queries. Given a file, line, and column, finds all expression nodes at that cursor position with innermost-first ordering and enclosing function context. Used for implementing IDE features like goto-definition, type-of, and find-references.



++++++++++
Structures
++++++++++

.. _struct-ast_cursor-CursorHit:

.. das:attribute:: CursorHit

:Fields: * **expr** :  :ref:`Expression <handle-ast-Expression>`? - Raw pointer to the expression node (valid while the program lives).

         * **func** :  :ref:`Function <handle-ast-Function>`? - Enclosing function, or null for expressions in global scope.

         * **name** : string - Extracted name for named nodes (variable name, call name, field name, operator).

         * **rtti** : string - Expression RTTI type string: "ExprVar", "ExprCall", "ExprField", etc.




+++++++
Classes
+++++++

.. _struct-ast_cursor-CursorVisitor:

.. das:attribute:: CursorVisitor : AstVisitor

class CursorVisitor

.. _function-ast_cursor_CursorVisitor_rq_preVisitFunction_CursorVisitor_FunctionPtr_0x6b:

.. das:function:: CursorVisitor.preVisitFunction(fun: FunctionPtr)

def preVisitFunction : function<(var self:ast::AstVisitor;fun:smart_ptr<ast::Function> aka FunctionPtr const):void>

:Arguments: * **fun** :  :ref:`FunctionPtr <alias-FunctionPtr>`

.. _function-ast_cursor_CursorVisitor_rq_visitFunction_CursorVisitor_FunctionPtr_0x6f:

.. das:function:: CursorVisitor.visitFunction(fun: FunctionPtr) : FunctionPtr

def visitFunction : function<(var self:ast::AstVisitor;fun:smart_ptr<ast::Function> aka FunctionPtr const):smart_ptr<ast::Function> aka FunctionPtr>

:Arguments: * **fun** :  :ref:`FunctionPtr <alias-FunctionPtr>`

.. _function-ast_cursor_CursorVisitor_rq_preVisitExpression_CursorVisitor_ExpressionPtr_0x74:

.. das:function:: CursorVisitor.preVisitExpression(expr: ExpressionPtr)

def preVisitExpression : function<(var self:ast::AstVisitor;expr:smart_ptr<ast::Expression> aka ExpressionPtr const):void>

:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>`

.. _function-ast_cursor_CursorVisitor_string_int_int_0x64:

.. das:function:: CursorVisitor(file: string; line: int; col: int) : CursorVisitor

def CursorVisitor (file: string; line: int; col: int) : CursorVisitor

:Arguments: * **file** : string

            * **line** : int

            * **col** : int


++++++++++++++
Cursor queries
++++++++++++++

  *  :ref:`cursor_inside (line: int; col: int; at: LineInfo) : bool <function-ast_cursor_cursor_inside_int_int_LineInfo>`
  *  :ref:`find_at_cursor (program: smart_ptr\<Program\>; file: string; line: int; col: int) : array\<CursorHit\> <function-ast_cursor_find_at_cursor_smart_ptr_ls_Program_gr__string_int_int>`
  *  :ref:`find_at_cursor_in_function (func: FunctionPtr; file: string; line: int; col: int) : array\<CursorHit\> <function-ast_cursor_find_at_cursor_in_function_FunctionPtr_string_int_int>`

.. _function-ast_cursor_cursor_inside_int_int_LineInfo:

.. das:function:: cursor_inside(line: int; col: int; at: LineInfo) : bool

def cursor_inside (line: int; col: int; at: LineInfo) : bool

:Arguments: * **line** : int

            * **col** : int

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>`

.. _function-ast_cursor_find_at_cursor_smart_ptr_ls_Program_gr__string_int_int:

.. das:function:: find_at_cursor(program: smart_ptr<Program>; file: string; line: int; col: int) : array<CursorHit>

def find_at_cursor (program: smart_ptr<Program>; file: string; line: int; col: int) : array<CursorHit>

:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>`>

            * **file** : string

            * **line** : int

            * **col** : int

.. _function-ast_cursor_find_at_cursor_in_function_FunctionPtr_string_int_int:

.. das:function:: find_at_cursor_in_function(func: FunctionPtr; file: string; line: int; col: int) : array<CursorHit>

def find_at_cursor_in_function (func: FunctionPtr; file: string; line: int; col: int) : array<CursorHit>

:Arguments: * **func** :  :ref:`FunctionPtr <alias-FunctionPtr>`

            * **file** : string

            * **line** : int

            * **col** : int


+++++++++++++++++
Result inspection
+++++++++++++++++

  *  :ref:`describe (hit: CursorHit) : string <function-ast_cursor_describe_CursorHit>`

.. _function-ast_cursor_describe_CursorHit:

.. das:function:: describe(hit: CursorHit) : string

def describe (hit: CursorHit) : string

:Arguments: * **hit** :  :ref:`CursorHit <struct-ast_cursor-CursorHit>`


