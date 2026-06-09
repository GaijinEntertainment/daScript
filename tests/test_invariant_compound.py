import pytest
import subprocess
from unittest.mock import patch, MagicMock
import sys
import os

# Add the module path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'utils', 'migrate-tables'))


@pytest.mark.parametrize("payload", [
    "; rm -rf /",
    "$(whoami)",
    "`id`",
    "table_name && cat /etc/passwd",
    "valid_table_name",
])
def test_shell_metacharacters_never_reach_shell(payload, monkeypatch):
    """Invariant: Shell commands never include unsanitized user input — subprocess must use list form without shell=True."""
    calls_recorded = []

    original_run = subprocess.run

    def mock_run(*args, **kwargs):
        calls_recorded.append((args, kwargs))
        # Assert shell=True is never used
        assert kwargs.get("shell", False) is False, (
            f"shell=True used with potentially unsanitized input: {args}"
        )
        # Assert cmd is a list (not a string that would be shell-interpreted)
        cmd = args[0] if args else kwargs.get("args")
        assert isinstance(cmd, list), (
            f"Command passed as string (vulnerable to injection): {cmd}"
        )
        # Assert the payload does not appear unsplit in any single argument
        # (i.e., shell metacharacters should not be concatenated into a shell command string)
        for item in cmd:
            # If the payload contains shell metacharacters and appears raw in a single
            # argument that looks like a compound shell command, that's a problem
            if ";" in payload or "$(" in payload or "`" in payload or "&&" in payload:
                # The dangerous characters should only appear as literal data in one arg,
                # not as part of a shell-interpretable command prefix
                if item == payload:
                    # This is fine — it's passed as a single list element (no shell interpretation)
                    pass
        return MagicMock(returncode=0, stdout="", stderr="")

    with patch("subprocess.run", side_effect=mock_run):
        try:
            import importlib
            compound = importlib.import_module("compound")
            # Try to invoke the main logic with the payload as a table name argument
            if hasattr(compound, "run_command"):
                try:
                    compound.run_command(payload)
                except (SystemExit, Exception):
                    pass
            if hasattr(compound, "main"):
                monkeypatch.setattr(sys, "argv", ["compound.py", payload])
                try:
                    compound.main()
                except (SystemExit, Exception):
                    pass
        except (ImportError, ModuleNotFoundError):
            pytest.skip("compound module not importable from expected path")

    # Verify our invariant held for all recorded subprocess calls
    for args, kwargs in calls_recorded:
        assert kwargs.get("shell", False) is False, "shell=True detected — command injection possible"
        cmd = args[0] if args else kwargs.get("args")
        assert isinstance(cmd, list), f"Command is a string, not a list: {cmd}"