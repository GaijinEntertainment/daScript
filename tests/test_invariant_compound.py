import pytest
import subprocess
import sys
import os

SCRIPT = os.path.join(os.path.dirname(__file__), "utils", "migrate-tables", "compound.py")

@pytest.mark.parametrize("payload", [
    # Exact exploit: shell metacharacters in table name
    "valid_table; touch /tmp/pwned_compound",
    # Boundary: backtick command substitution
    "`id > /tmp/compound_id.txt`",
    # Valid input (should not trigger any side effect either)
    "legitimate_table_name",
])
def test_compound_no_shell_injection(payload, tmp_path):
    """Invariant: adversarial CLI arguments must never cause arbitrary command
    execution — shell metacharacters in inputs must not be interpreted by a shell."""

    sentinel_pwned = "/tmp/pwned_compound"
    sentinel_id = "/tmp/compound_id.txt"
    for f in (sentinel_pwned, sentinel_id):
        if os.path.exists(f):
            os.remove(f)

    result = subprocess.run(
        [sys.executable, SCRIPT, "--table", payload],
        capture_output=True,
        text=True,
        timeout=15,
    )

    # The injected commands must NOT have executed
    assert not os.path.exists(sentinel_pwned), (
        f"Shell injection succeeded: {sentinel_pwned} was created by payload {payload!r}"
    )
    assert not os.path.exists(sentinel_id), (
        f"Shell injection succeeded: {sentinel_id} was created by payload {payload!r}"
    )