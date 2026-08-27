options gen2

module csv_bake shared private

require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost
require daslib/fio
require daslib/strings_boost
require daslib/strings_convert
require strings

//! Compile-time CSV baking. `bake_curve("file.csv", N)` reads a two-column
//! calibration file while the calling module COMPILES, resamples it onto N
//! evenly spaced points, and splices the result in as an integer literal.
//!
//! The program that results carries the table and nothing that built it - no
//! CSV text, no parser, no division to invert the curve, no file I/O.
//!
//! `curve_lo(path)` / `curve_hi(path)` bake the first and last x, so the caller
//! maps a reading onto the table without repeating those numbers by hand.

struct private CalPoint {
    x : int
    y : int
}

// Calibration files are written by instruments and by people, so tolerate blank
// lines, `#` comments and a header row rather than demanding a clean file.
// An unreadable or unusable file comes back empty; each macro reports that.
def private read_points(path : string) : array<CalPoint> {
    var points : array<CalPoint>
    var lines <- split(fio::fread(path), "\n")
    for (line in lines) {
        let trimmed = strip(line)
        if (empty(trimmed) || starts_with(trimmed, "#")) {
            continue
        }
        let cells <- split(trimmed, ",")
        if (length(cells) != 2) {
            continue
        }
        let px = try_to_int(strip(cells[0]))
        let py = try_to_int(strip(cells[1]))
        if (px |> is_ok && py |> is_ok) {
            points |> push(CalPoint(x = px ?? 0, y = py ?? 0))
        }
    }
    // the file may arrive in any order; every read below walks it low to high
    sort(points) $(a, b) => a.x < b.x
    return <- points
}

// Piecewise-linear read of the calibration curve, clamped past either end.
// Integer math throughout: this runs in the compiler, but staying exact means
// the baked table is identical on every host that builds the program.
def private sample_at(points : array<CalPoint>; x : int) : int {
    let last = length(points) - 1
    if (x <= points[0].x) {
        return points[0].y
    }
    if (x >= points[last].x) {
        return points[last].y
    }
    for (i in range(last)) {
        let lo = points[i]
        let hi = points[i + 1]
        if (x >= lo.x && x <= hi.x) {
            let span = hi.x - lo.x
            return span == 0 ? lo.y : lo.y + ((hi.y - lo.y) * (x - lo.x)) / span
        }
    }
    return points[last].y
}

// resolved against the CALLING file, so an example moves without its data breaking
def private csv_path(var expr : ExprCallMacro?) : string {
    let rel = string((expr.arguments[0] as ExprConstString).value)
    return path_join(dir_name(string(expr.at.fileInfo.name)), rel)
}

[call_macro(name="bake_curve")]
class BakeCurve : AstCallMacro {
    //! ``bake_curve(csv_path, count)`` - the resampled table, as an integer literal.
    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : ExpressionPtr {
        macro_verify(expr.arguments |> length == 2, prog, expr.at,
            "bake_curve expects (csv path : string literal, count : int literal)")
        macro_verify(expr.arguments[0] is ExprConstString, prog, expr.at,
            "bake_curve: path must be a string literal, got {describe(expr.arguments[0])}")
        macro_verify(expr.arguments[1] is ExprConstInt, prog, expr.at,
            "bake_curve: count must be an integer literal, got {describe(expr.arguments[1])}")
        macro_verify(expr.at.fileInfo != null, prog, expr.at,
            "bake_curve: the call site has no file to resolve the path against")
        let count = (expr.arguments[1] as ExprConstInt).value
        macro_verify(count > 1, prog, expr.at, "bake_curve: count must be greater than 1, got {count}")
        let path = csv_path(expr)
        let points <- read_points(path)
        macro_verify(length(points) >= 2, prog, expr.at,
            "bake_curve: {path} holds {length(points)} usable rows, need at least 2")

        // Resample onto an even grid. This is the whole trick: an evenly spaced
        // table is indexed directly at run time, so the search and the
        // interpolation a raw calibration file would need never ship.
        let lo = points[0].x
        let hi = points[length(points) - 1].x
        // a caller maps readings with (x - lo) / (hi - lo); a file where every
        // row shares one x would turn that into a divide by zero at run time
        macro_verify(hi > lo, prog, expr.at,
            "bake_curve: {path} spans no x range (all {length(points)} rows at x = {lo})")
        var curve : array<int>
        curve |> reserve(count)
        for (i in range(count)) {
            curve |> push(sample_at(points, lo + ((hi - lo) * i) / (count - 1)))
        }
        return <- qmacro($v(curve))
    }
}

[call_macro(name="curve_lo")]
class CurveLo : AstCallMacro {
    //! ``curve_lo(csv_path)`` - lowest x in the file.
    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : ExpressionPtr {
        macro_verify(expr.arguments |> length == 1, prog, expr.at, "curve_lo expects (csv path : string literal)")
        macro_verify(expr.arguments[0] is ExprConstString, prog, expr.at,
            "curve_lo: path must be a string literal, got {describe(expr.arguments[0])}")
        macro_verify(expr.at.fileInfo != null, prog, expr.at,
            "curve_lo: the call site has no file to resolve the path against")
        let path = csv_path(expr)
        let points <- read_points(path)
        macro_verify(length(points) >= 2, prog, expr.at, "curve_lo: {path} holds too few usable rows")
        let bound = points[0].x
        return <- qmacro($v(bound))
    }
}

[call_macro(name="curve_hi")]
class CurveHi : AstCallMacro {
    //! ``curve_hi(csv_path)`` - highest x in the file.
    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : ExpressionPtr {
        macro_verify(expr.arguments |> length == 1, prog, expr.at, "curve_hi expects (csv path : string literal)")
        macro_verify(expr.arguments[0] is ExprConstString, prog, expr.at,
            "curve_hi: path must be a string literal, got {describe(expr.arguments[0])}")
        macro_verify(expr.at.fileInfo != null, prog, expr.at,
            "curve_hi: the call site has no file to resolve the path against")
        let path = csv_path(expr)
        let points <- read_points(path)
        macro_verify(length(points) >= 2, prog, expr.at, "curve_hi: {path} holds too few usable rows")
        let bound = points[length(points) - 1].x
        return <- qmacro($v(bound))
    }
}
