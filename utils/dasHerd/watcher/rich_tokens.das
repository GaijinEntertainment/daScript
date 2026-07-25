options gen2
options gc
options persistent_heap
options no_unused_function_arguments = false

module rich_tokens

require imgui/imgui_harness

//! The token color language (note 20): one color per token TYPE, shared by
//! every surface. A SHA is the same blue in the PR table, the History table,
//! the changelist header, and a card; a path renders dir/sep/name in the same
//! three tints everywhere. Surfaces must not restate these literals.

def token_sha_color(alpha : float = 1.0f) : float4 {
    return float4(0.36f, 0.76f, 0.96f, alpha)
}

def token_branch_color(alpha : float = 1.0f) : float4 {
    return float4(0.29f, 0.84f, 0.74f, alpha)
}

def token_pid_color(alpha : float = 1.0f) : float4 {
    return float4(0.76f, 0.54f, 0.95f, alpha)
}

def token_path_dir_color(alpha : float = 1.0f) : float4 {
    return float4(0.43f, 0.64f, 0.82f, alpha)
}

def token_path_sep_color(alpha : float = 1.0f) : float4 {
    return float4(0.78f, 0.55f, 0.30f, alpha)
}

def token_path_name_color(alpha : float = 1.0f) : float4 {
    return float4(0.90f, 0.91f, 0.93f, alpha)
}

def token_warning_color(alpha : float = 1.0f) : float4 {
    return float4(0.97f, 0.76f, 0.28f, alpha)
}

def token_error_color(alpha : float = 1.0f) : float4 {
    return float4(1.0f, 0.35f, 0.30f, alpha)
}

def token_added_color(alpha : float = 1.0f) : float4 {
    return float4(0.42f, 0.82f, 0.42f, alpha)
}

def token_deleted_color(alpha : float = 1.0f) : float4 {
    return float4(0.37f, 0.65f, 0.96f, alpha)
}

def token_title_color(alpha : float = 1.0f) : float4 {
    return float4(0.91f, 0.93f, 0.96f, alpha)
}

def token_muted_color(alpha : float = 1.0f) : float4 {
    return float4(0.64f, 0.70f, 0.80f, alpha)
}

def token_state_color(state : string; alpha : float = 1.0f) : float4 {
    //! Session/worktree lifecycle words: running green, exited muted,
    //! closed dim, anything conflicted/attention amber.
    if (state == "running" || state == "interactive") return token_added_color(alpha)
    if (state == "exited") return token_muted_color(alpha)
    if (state == "closed") return token_muted_color(alpha * 0.7f)
    return token_warning_color(alpha)
}
