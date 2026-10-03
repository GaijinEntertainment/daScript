#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include <limits>

TEST_CASE("wide annotation integer option lookup") {
    das::AnnotationArgumentList args;
    args.emplace_back("wide", uint64_t(9007199254740991ULL));
    args.emplace_back("signed", int64_t(4102444800LL));
    args.emplace_back("legacy", -1);
    args.emplace_back("negative", int64_t(-1));
    args.emplace_back("wrong", std::string("text"));
    args.emplace_back("duplicate", std::string("text"));
    args.emplace_back("duplicate", uint64_t(4102444800ULL));
    CHECK(args.getUInt64Option("wide", 7) == 9007199254740991ULL);
    CHECK(args.getUInt64Option("signed", 7) == 4102444800ULL);
    CHECK(args.getUInt64Option("legacy", 7) == std::numeric_limits<uint64_t>::max());
    CHECK(args.getUInt64Option("negative", 7) == std::numeric_limits<uint64_t>::max());
    CHECK(args.getUInt64Option("missing", 7) == 7);
    CHECK(args.getUInt64Option("wrong", 7) == 7);
    CHECK(args.getUInt64OptionEx("missing", "wide", 7) == 9007199254740991ULL);
    CHECK(args.getUInt64OptionEx("wrong", "signed", 7) == 4102444800ULL);
    CHECK(args.getUInt64OptionEx("signed", "wide", 7) == 4102444800ULL);
    CHECK(args.getUInt64OptionEx("missing", "wrong", 7) == 7);
    CHECK(args.getUInt64Option("duplicate", 7) == 4102444800ULL);
    CHECK(args.getUInt64OptionEx("duplicate", "wide", 7) == 4102444800ULL);
}

TEST_CASE("wide annotation copies retain numeric payload") {
    das::AnnotationArgument source("wide", std::numeric_limits<uint64_t>::max());
    das::AnnotationArgument copied(source);
    CHECK(copied.ulValue == source.ulValue);
    copied = copied;
    CHECK(copied.ulValue == std::numeric_limits<uint64_t>::max());
    copied = das::AnnotationArgument("signed", int64_t(-9007199254740991LL));
    CHECK(copied.lValue == -9007199254740991LL);
}

TEST_CASE("wide annotation printers preserve values and type suffixes") {
    das::AnnotationArgumentList args;
    args.emplace_back("low", std::numeric_limits<int64_t>::min());
    args.emplace_back("high", std::numeric_limits<uint64_t>::max());
    das::Variable variable;
    das::TypeDecl argumentType(das::Type::tInt, das::LineInfo());
    das::TypeDecl resultType(das::Type::tVoid, das::LineInfo());
    variable.name = "value";
    variable.type = &argumentType;
    variable.annotation = args;
    das::Function function;
    function.name = "wide";
    function.arguments.push_back(&variable);
    function.result = &resultType;
    das::TextWriter printedFunction;
    printedFunction << function;
    CHECK(printedFunction.str().find("low=-9223372036854775808l") != das::string::npos);
    CHECK(printedFunction.str().find("high=18446744073709551615ul") != das::string::npos);
    das::ExprLet declaration;
    declaration.variables.push_back(&variable);
    das::TextWriter printedVariable;
    printedVariable << declaration;
    CHECK(printedVariable.str().find("low=-9223372036854775808l") != das::string::npos);
    CHECK(printedVariable.str().find("high=18446744073709551615ul") != das::string::npos);
    das::Annotation annotation("wide");
    das::AnnotationDeclaration applied;
    applied.annotation = &annotation;
    applied.arguments = args;
    das::TextWriter printedAnnotation;
    annotation.log(printedAnnotation, applied);
    CHECK(printedAnnotation.str().find("-9223372036854775808l") != das::string::npos);
    CHECK(printedAnnotation.str().find("18446744073709551615ul") != das::string::npos);
}
