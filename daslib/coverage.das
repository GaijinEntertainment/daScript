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
module coverage shared public ! inscope

//! Code coverage instrumentation macro.
//!
//! Automatically inserts coverage tracking calls at every block entry point,
//! function entry, and branch (if/else) decision point.
//! After execution, call `get_report` to produce output in LCOV format,
//! which can be converted to HTML via ``genhtml``.

require ast
require rtti
require daslib/ast_boost
require strings
require daslib/templates_boost
require fio


struct LineCov {
    //! Per-file coverage data storing line hit counts.
    lines : table<uint; uint>

    def insert(x : uint) {
        //! Increments the hit count for the given line number.
        unsafe(lines[x]) += 1u
    }
}

struct FuncCov {
    //! Per-function coverage entry storing line number and hit count.
    line : uint
    hits : uint
}

struct BranchCov {
    //! Per-branch coverage entry storing line, block id, branch id and hit count.
    line : uint
    block_num : uint
    branch_num : uint
    hits : uint
}

struct CoverageData {
    lineCov = LineCov() // Line coverage.
    funcCov : table<string; FuncCov> // Function name -> Function coverage.
    branchCov : table<uint; BranchCov> // branch_idx -> Branch coverage.
}

var coverageData = table<string; CoverageData>()

def private single_file_report(name : string) {
    var fnData : string;
    var fnFound = 0
    var fnHit = 0
    coverageData |> get(name) $(funcs) {
        for (fname, entry in keys(funcs.funcCov), values(funcs.funcCov)) {
            fnData += "FN:{entry.line |> int},{fname}\n"
        }
        for (fname, entry in keys(funcs.funcCov), values(funcs.funcCov)) {
            fnData += "FNDA:{entry.hits |> int},{fname}\n"
            fnFound += 1
            if (entry.hits > 0u) {
                fnHit += 1
            }
        }
    };
    var brData : string;
    var brFound = 0
    var brHit = 0
    coverageData |> get(name) $(branches) {
        for (br in values(branches.branchCov)) {
            var taken : string
            if (br.hits == 0u) {
                taken = "-"
            } else {
                taken = "{br.hits |> int}"
            }
            brData += "BRDA:{br.line |> int},{br.block_num |> int},{br.branch_num |> int},{taken}\n"
            brFound += 1
            if (br.hits > 0u) {
                brHit += 1
            }
        }
    };
    var data : string;
    coverageData |> get(name) $(x) {
        for (k, v in keys(x.lineCov.lines), values(x.lineCov.lines)) {
            data += "DA:{k |> int},{v |> int}\n"
        }
    };
    return "\nTN:\nSF:{name}\n{fnData}FNF:{fnFound}\nFNH:{fnHit}\n{brData}BRF:{brFound}\nBRH:{brHit}\n{data}end_of_record\n"
}

def public get_report(name : string = "") : string {
    //! Returns code coverage report in lcov format for the specified file, or all files if name is empty.
    if (name |> empty) {
        var res = ""

        for (k in keys(coverageData)) {
            res += single_file_report(k)
        }
        return res
    } else {
        return single_file_report(name);
    }
}

[export]
def public write_report(fname : string; name : string = "") {
    //! Writes coverage report in lcov format to the specified file.
    let report = get_report(name)
    fopen(fname, "wb") <| $(f) {
        if (f != null) {
            fwrite(f, report)
        }
    }
}

[export]
def public fill_report(var dest : string?; name : string = "") {
    //! Fills the string at dest with the coverage report. Used for cross-context data transfer.
    unsafe {
        *dest = get_report(name)
    }
}

def public register_lines(file : string; lines : array<uint>) {
    //! Registers lines as instrumented with 0 hit count (for DA:line,0 in LCOV).
    coverageData |> get_with_default(file) <| $(var cov) {
        for (line in lines) {
            cov.lineCov.lines |> insert(line, 0u)
        }
    }
}

def public register_functions(file : string; names : array<string>; lines : array<uint>) {
    //! Registers functions with 0 hit count for FN/FNDA records in LCOV.
    coverageData |> get_with_default(file) <| $(var funcs) {
        for (i in range(names |> length)) {
            if (!(funcs.funcCov |> key_exists(names[i]))) {
                funcs.funcCov |> insert(names[i], FuncCov(line = lines[i], hits = 0u))
            }
        }
    }
}

def public register_branches(file : string; blines : array<uint>; block_nums : array<uint>; branch_nums : array<uint>) {
    //! Registers branches with 0 hit count for BRDA records in LCOV.
    coverageData |> get_with_default(file) <| $(var branches) {
        for (i in range(blines |> length)) {
            branches.branchCov |> insert(i |> uint, BranchCov(line = blines[i], block_num = block_nums[i], branch_num = branch_nums[i], hits = 0u))
        }
    }
}

def public add_coverage(file : string; line : uint) {
    //! Records a coverage hit for the given file and line number.
    coverageData |> get_with_default(file) <| $(var cov) {
        cov.lineCov.insert(line)
    }
}

def public add_func_coverage(file : string; name : string) {
    //! Records a coverage hit for the given file and function name.
    coverageData |> get_with_default(file) <| $(var funcs) {
        funcs.funcCov |> modify(name) <| $(value) {
            var res = value
            res.hits += 1u
            return res
        }
    }
}

def public add_branch_coverage(file : string; branch_idx : uint) {
    //! Records a coverage hit for the given file and branch index.
    coverageData |> get_with_default(file) <| $(var branches) {
        branches.branchCov |> modify(uint(branch_idx)) <| $(value) {
            var res = value
            res.hits += 1u
            return res
        }
    }
}

// compile-time data for function and branch registration
struct RegisterData {
    allFunctions : table<string; uint>
    allBranches : array<tuple<uint;uint;uint>>
    
}

var allFunctions = table<string; table<string; uint>>(); //! file -> funcname -> line (compile time)
var allBranches = table<string; array<tuple<uint;uint;uint>>>(); //! file -> [(line, block_num, branch_num)] (compile time)
var branchBlockCounters = table<string; uint>(); //! file -> next block_num counter (compile time)

class NoCoverageAnnotation : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        return true
    }
}

[_macro]
def private setup_coverage {
    if (is_compiling_macros_in_module("coverage")) {
        add_new_function_annotation("no_coverage", new NoCoverageAnnotation())
    }
}

class CoverageMacro : AstVisitor {
    //! AST visitor that instruments expression blocks, function entries, and branches with coverage tracking calls.
    astChanged : bool = false

    @do_not_delete func : Function?
    def override preVisitFunction(var fun : FunctionPtr) {
        //! Stores a pointer to the current function being visited and records function info for coverage.
        func = get_ptr(fun)
        if (fun.flags.generated || fun.flags.init || fun.flags.builtIn) {
            return
        }
        if (fun.at.fileInfo == null) {
            return
        }
        let file = string(fun.at.fileInfo.name)
        let fname = string(fun.name)
        let line = fun.at.line
        allFunctions |> get_with_default(file) <| $(var funcs) {
            if (!(funcs |> key_exists(fname))) {
                funcs |> insert(fname, line)
            }
        }
    }

    def override canVisitFunction(fun : Function?) : bool {
        if (fun.name |> string() |> starts_with("builtin") ||
            fun.flags.init) {
            return false
        }
        for (ann in fun.annotations) {
            if (ann.annotation.name != "marker") {
                continue
            }
            for (arg in ann.arguments) {
                if (arg.name == "no_coverage") {
                    return false;
                }
            }
        }
        return true
    }

    def override preVisitFunctionBody(fun : FunctionPtr; expr : ExpressionPtr) {
        //! Inserts function coverage call at the start of each function body.
        if (fun.flags.generated || fun.flags.init || fun.flags.builtIn) {
            return
        }
        if (fun.at.fileInfo == null) {
            return
        }
        if (!(expr is ExprBlock)) {
            return
        }
        let file = string(fun.at.fileInfo.name)
        let fname = string(fun.name)
        var blk = unsafe(reinterpret<ExprBlock?> expr)
        var inscope prev : array<ExpressionPtr>
        for (ex in blk.list) {
            prev.emplace(ex)
        }
        blk.list |> clear()
        blk.list |> emplace_new <| qmacro(_::add_func_coverage($v(file), $v(fname)))
        for (el in range(prev |> length)) {
            blk.list |> emplace <| prev[el]
        }
        astChanged = true
        func.not_inferred()
    }

    def override visitExprIfThenElse(var ifte : smart_ptr<ExprIfThenElse>) : ExpressionPtr {
        //! Instruments if/else branches with branch coverage tracking calls.
        // if (ifte.isStatic) {
        //     return <- ifte
        // }
        if (ifte.at.fileInfo == null) {
            // assert?
            return <- ifte
        }
        let file = string(ifte.at.fileInfo.name)
        let line = ifte.at.line
        var block_num : uint
        branchBlockCounters |> get_with_default(file) <| $(var counter) {
            block_num = counter
            counter += 1u
        }
        var true_idx : uint
        var false_idx : uint
        allBranches |> get_with_default(file) <| $(var branches) {
            true_idx = uint(branches |> length)
            branches |> push(tuple(line, block_num, 0u))
            false_idx = uint(branches |> length)
            branches |> push(tuple(line, block_num, 1u))
        }
        // instrument true branch
        if (ifte.if_true != null && ifte.if_true is ExprBlock) {
            var trueBlk = unsafe(reinterpret<ExprBlock?> ifte.if_true)
            var inscope prevTrue : array<ExpressionPtr>
            for (ex in trueBlk.list) {
                prevTrue.emplace(ex)
            }
            trueBlk.list |> clear()
            trueBlk.list |> emplace_new <| qmacro(_::add_branch_coverage($v(file), $v(true_idx)))
            for (el in range(prevTrue |> length)) {
                trueBlk.list |> emplace <| prevTrue[el]
            }
        }
        // instrument false branch
        if (ifte.if_false != null && ifte.if_false is ExprBlock) {
            var falseBlk = unsafe(reinterpret<ExprBlock?> ifte.if_false)
            var inscope prevFalse : array<ExpressionPtr>
            for (ex in falseBlk.list) {
                prevFalse.emplace(ex)
            }
            falseBlk.list |> clear()
            falseBlk.list |> emplace_new <| qmacro(_::add_branch_coverage($v(file), $v(false_idx)))
            for (el in range(prevFalse |> length)) {
                falseBlk.list |> emplace <| prevFalse[el]
            }
        } elif (ifte.if_false == null) {
            // no else branch — create one with just the branch coverage call
            var inscope elseBlk <- new ExprBlock(at = ifte.at)
            elseBlk.list |> emplace_new <| qmacro(_::add_branch_coverage($v(file), $v(false_idx)))
            move(ifte.if_false) <| elseBlk
        }
        astChanged = true
        func.not_inferred()
        return <- ifte
    }

    def override visitExprBlock(var blk : smart_ptr<ExprBlock>) : ExpressionPtr {
        //! Instruments expression blocks with coverage tracking calls.
        if (blk.at.fileInfo == null) {
            return <- blk
        }
        astChanged = true
        func.not_inferred()
        var inscope prev : array<ExpressionPtr>;
        for (ex in blk.list) {
            prev.emplace(ex)
        }
        blk.list |> clear();
        for (el in range(prev |> length)) {
            // Todo: add linter phase to check fileInfo is never null.
            if (prev[el].at.fileInfo != null) {
                let file = string(prev[el].at.fileInfo.name)
                let line = prev[el].at.line
                coverageData |> get_with_default(file) <| $(var lines) {
                    lines.lineCov.insert(line)
                }
                blk.list |> emplace_new <| qmacro(_::add_coverage($v(file), $v(line)));
                blk.list |> emplace <| prev[el]
            }
        }
        return <- blk
    }

    def override preVisitExprAssert ( var expr : smart_ptr<ExprAssert> ) {
        expr.isVerify = true
        astChanged = true
        func.not_inferred()
    }

}

//! Infer pass macro that applies coverage instrumentation to the program.
[infer_macro]
class CoveragePass : AstPassMacro {
    //! Pass macro that instruments modules with code coverage.
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        //! Applies the coverage instrumentation visitor to the program.
        if (prog.getThisModule.moduleFlags.isModule) {
            return false
        }
        if (mod.name == "coverage" || (prog._options |> find_arg("no_coverage") ?as tBool ?? false)) {
            return false
        }
        var hasCoverage = false
        prog.get_ptr() |> for_each_module() <| $(m) {
            if (m.name == "coverage") {
                hasCoverage = true
            }
        }
        if (!hasCoverage) {
            return false
        }
        if (mod |> find_unique_function("__coverage_init") != null) {
            return false
        }
        var astVisitor = new CoverageMacro()
        var inscope astVisitorAdapter <- make_visitor(*astVisitor)
        visit(prog, astVisitorAdapter)
        var result = astVisitor.astChanged
        if (result) {
            // generate [init] function that registers all instrumented lines with 0 hits
            var inscope initFn <- qmacro_function("__coverage_init") <| $() {
                pass
            }
            var blk = unsafe(reinterpret<ExprBlock?> initFn.body)
            blk.list |> clear()
            for (file, values in keys(coverageData), values(coverageData)) {
                var lineArr : array<uint>
                for (line in keys(values.lineCov.lines)) {
                    lineArr |> push(line)
                }
                blk.list |> emplace_new <| qmacro(_::register_lines($v(string(file)), $v(lineArr)))
            }
            // register functions
            for (file in keys(allFunctions)) {
                allFunctions |> get(string(file)) $(funcs) {
                    var nameArr : array<string>
                    var lineArr : array<uint>
                    for (fname, fline in keys(funcs), values(funcs)) {
                        nameArr |> push(string(fname))
                        lineArr |> push(fline)
                    }
                    blk.list |> emplace_new <| qmacro(_::register_functions($v(string(file)), $v(nameArr), $v(lineArr)))
                }
            }
            // register branches
            for (file in keys(allBranches)) {
                allBranches |> get(string(file)) $(branches) {
                    var blines : array<uint>
                    var block_nums : array<uint>
                    var branch_nums : array<uint>
                    var branch_idx : array<uint>
                    for (i in range(branches |> length())) {
                        blines |> push(branches[i]._0)
                        block_nums |> push(branches[i]._1)
                        branch_nums |> push(branches[i]._2)
                        branch_idx |> push(i |> uint)
                    }
                    blk.list |> emplace_new <| qmacro(_::register_branches($v(string(file)), $v(blines), $v(block_nums), $v(branch_nums)))
                }
            }
            initFn.flags |= FunctionFlags.generated | FunctionFlags.init | FunctionFlags.privateFunction
            mod |> add_function(initFn)
        }
        unsafe {
            delete astVisitor
        }
        return result
    }
}
