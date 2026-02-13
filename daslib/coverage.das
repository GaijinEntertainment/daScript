/*
 * Code coverage macro for daslang.
 * To make it work you should add coverage module:
 * `require daslib/coverage`
 * After executing tests/anything else add call to
 * `get_report(filename) : string`
 * to get coverage report. To create html from it use:
 * `genhtml cov.lcov -o coverage-report  --synthesize-missing --ignore-errors source`
 */
options gen2
module coverage shared public

//! Code coverage instrumentation macro.
//!
//! Automatically inserts coverage tracking calls at every block entry point.
//! After execution, call `get_report` to produce output in LCOV format,
//! which can be converted to HTML via ``genhtml``.

require ast
require rtti
require daslib/ast_boost
require daslib/templates_boost
require daslib/defer
require daslib/macro_boost


struct FileCov {
    lines : table<uint; uint>

    def insert(x : uint) {
        unsafe(lines[x]) += 1u
    }
}

var coverageData = new table<string; FileCov>();
var checkData = new table<string; table<uint>>();

def private single_file_report(name : string) {
    var data : string;
    (*coverageData) |> get(name) <| $(x) {
        for (k, v in keys(x.lines), values(x.lines)) {
            data += "DA:{k |> int},{v |> int}\n"
        }
    };
    return "\nTN:\nSF:{name}\n{data}end_of_record\n" // lcov format
}

def public get_report(name : string = "") : string {
    if (name |> empty) {
        var res = ""

        for (k in keys(*coverageData)) {
            res += single_file_report(k)
        }
        return res
    } else {
        return single_file_report(name);
    }
}

def public add_coverage(file : string; line : uint) {
    (*coverageData) |> get_with_default(file) <| $(var cov) { cov.lines |> modify(line) <| $(x : uint&) => x + 1u; }
}

class CoverageMacro : AstVisitor {
    astChanged : bool = false

    @do_not_delete func : Function?
    def override preVisitFunction(var fun : FunctionPtr) {
        func = get_ptr(fun)
    }

    def override visitExprBlock(var blk : smart_ptr<ExprBlock>) : ExpressionPtr {
        if ((*checkData)[string(blk.at.fileInfo.name)] |> key_exists(blk.at.line)) {
            return <- blk;
        }
        (*checkData)[string(blk.at.fileInfo.name)] |> insert(blk.at.line);
        astChanged = true
        func.not_inferred()
        var inscope prev : array<ExpressionPtr>;
        for (ex in blk.list) {
            prev.emplace(ex)
        }
        blk.list |> clear();
        for (el in range(prev |> length())) {
            blk.list |> emplace_new <| qmacro(_::add_coverage($v(string(prev[el].at.fileInfo.name)), $v(prev[el].at.line)));
            blk.list |> emplace <| prev[el]
        }
        return <- blk
    }
}

[infer_macro]
class CoveragePass : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        var astVisitor = new CoverageMacro()
        var inscope astVisitorAdapter <- make_visitor(*astVisitor)
        visit(prog, astVisitorAdapter)
        var result = astVisitor.astChanged
        unsafe {
            delete astVisitor
        }
        return result
    }
}
