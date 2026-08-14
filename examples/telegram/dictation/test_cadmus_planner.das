options gen2
options no_unused_function_arguments = false

require dastest/testing_boost
require cadmus_planner
require daslib/defer
require daslib/json_boost

[test]
def test_parse_cadmus_retrieval_plan(t : T?) {   // nolint:STYLE038 - flat parse-then-assert list, one plan shape per block
    var plan = CadmusRetrievalPlan()
    defer() { delete plan.Queries }
    var error = ""
    t |> success(parse_cadmus_retrieval_plan(
        "```json\n\{\"action\":\"search_messages\",\"answer_language\":\"ru\",\"queries\":[\"рыбал\",\"fishing\"],\"sender\":\"Greg Cohen\",\"limit\":8}\n```",
        plan, error), error)
    t |> equal(plan.Action, "search_messages")
    t |> equal(plan.Query, "рыбал")
    t |> equal(length(plan.Queries), 2)
    t |> equal(plan.Queries[1], "fishing")
    t |> equal(plan.Sender, "Greg Cohen")
    t |> equal(plan.AnswerLanguage, "ru")
    t |> equal(plan.Limit, 8)

    error = ""
    t |> success(parse_cadmus_retrieval_plan(
        "Plan:\n\{\"action\":\"search_web\",\"answer_language\":\"en\",\"query\":\"Brave pricing\"}\nDone.",
        plan, error), error)
    t |> equal(plan.Action, "search_web")
    t |> equal(plan.Query, "Brave pricing")
    t |> equal(length(plan.Queries), 1)

    error = ""
    t |> success(parse_cadmus_retrieval_plan(
        "\{\"action\":\"answer\",\"answer_language\":\"ru\"}", plan, error), error)
    t |> equal(plan.Action, "answer")

    error = ""
    t |> success(parse_cadmus_retrieval_plan(
        "\{\"action\":\"get_message_context\",\"answer_language\":\"en\",\"message_id\":42,\"before\":3,\"after\":7}",
        plan, error), error)
    t |> equal(plan.MessageId, 42l)
    t |> equal(plan.Before, 3)
    t |> equal(plan.After, 7)
    let arguments = cadmus_retrieval_arguments_json(plan)
    var arguments_error = ""
    var arguments_json = read_json(arguments, arguments_error)
    t |> success(arguments_json != null, arguments_error)
    if (arguments_json != null) {
        t |> equal(arguments_json?["action"] ?? "", "get_message_context")
        t |> equal(arguments_json?["answer_language"] ?? "", "en")
        t |> equal(arguments_json?["message_id"] ?? 0l, 42l)
        t |> equal(arguments_json?["before"] ?? 0, 3)
        t |> equal(arguments_json?["after"] ?? 0, 7)
        unsafe {
            delete arguments_json
        }
    }

    error = ""
    t |> success(!parse_cadmus_retrieval_plan(
        "\{\"action\":\"search_messages\",\"answer_language\":\"ru\",\"query\":\"\"}", plan, error))
    t |> success(!empty(error))

    error = ""
    t |> success(!parse_cadmus_retrieval_plan(
        "\{\"action\":\"answer\"}", plan, error))
    t |> success(!empty(error))

    error = ""
    t |> success(!parse_cadmus_retrieval_plan(
        "\{\"action\":\"invent_tool\",\"query\":\"x\"}", plan, error))
    t |> success(!empty(error))

    error = ""
    t |> success(!parse_cadmus_retrieval_plan(
        "\{\"action\":\"get_message_context\",\"answer_language\":\"ru\",\"message_id\":\"\"}",
        plan, error))
    t |> success(!empty(error))

    error = ""
    t |> success(!parse_cadmus_retrieval_plan(
        "\{\"action\":\"list_messages\",\"answer_language\":\"ru\",\"before_message_id\":\"not-a-number\"}",
        plan, error))
    t |> success(!empty(error))

    error = ""
    t |> success(parse_cadmus_retrieval_plan(
        "\{\"action\":\"get_message_context\",\"answer_language\":\"ru\",\"message_id\":\"63042\"}",
        plan, error), error)
    t |> equal(plan.MessageId, 63042l)

    // A bare integer tokenizes as _longint, but a fraction or exponent lands in _number; both are
    // valid ids from the planner and both must parse.
    error = ""
    t |> success(parse_cadmus_retrieval_plan(
        "\{\"action\":\"get_message_context\",\"answer_language\":\"ru\",\"message_id\":63042}",
        plan, error), error)
    t |> equal(plan.MessageId, 63042l)

    error = ""
    t |> success(parse_cadmus_retrieval_plan(
        "\{\"action\":\"get_message_context\",\"answer_language\":\"ru\",\"message_id\":63042.0}",
        plan, error), error)
    t |> equal(plan.MessageId, 63042l)

    // A fractional id is not an integer that happened to be encoded as a JSON number; truncating it
    // would anchor retrieval on a different row than the planner named.
    error = ""
    t |> success(!parse_cadmus_retrieval_plan(
        "\{\"action\":\"get_message_context\",\"answer_language\":\"ru\",\"message_id\":63042.5}",
        plan, error))
    t |> success(!empty(error))

    error = ""
    t |> success(!parse_cadmus_retrieval_plan("not json", plan, error))
    t |> success(!empty(error))
}
