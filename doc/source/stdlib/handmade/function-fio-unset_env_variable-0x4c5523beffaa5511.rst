Removes an environment variable from the current process (`unsetenv`, or `_wputenv_s` with an empty value on Windows). Afterwards `has_env_variable` is false for it.
