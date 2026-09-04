options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_tune_scope shared public

require dasllama/dasllama_lint public
require llvm/daslib/llvm_tune public  // the scope annotation; PUBLIC so the auto-policy infer pass and tune_status reach app roots
require dasllama/dasllama_version  // nolint:STYLE030 — version_of= below names DASLLAMA_RELEASE by string; the module must be in this compile

//! The scope reaches the AST before any covered module compiles - the scope apply refuses a covered
//! module that compiled ahead of it.
[tune_scope(name = "dasllama", tuner = "../harness/dasllama_tuner.das",
            covers = "dasllama_math_gen;dasllama_math;dasllama_math_default;dasllama_math_aarch64_neon;dasllama_kv_codec;dasllama_convert",
            version_of = "dasllama_version/DASLLAMA_RELEASE", version_key = "dasllama_version",
            defaults = "../performance/defaults")]
struct private DasllamaTuneScope {}
