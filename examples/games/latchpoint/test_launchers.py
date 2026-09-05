import json
import os
from pathlib import Path
import shutil
import shlex
import stat
import subprocess
import tempfile
import unittest


HERE = Path(os.environ.get("LATCHPOINT_LAUNCHER_SOURCE", Path(__file__).resolve().parent))


class LauncherTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="latchpoint launcher ")
        self.root = Path(self.temp.name) / "repo with spaces"
        self.latchpoint = self.root / "examples" / "games" / "latchpoint"
        self.latchpoint.mkdir(parents=True)

    def tearDown(self):
        self.temp.cleanup()

    def copy_launcher(self, name):
        destination = self.latchpoint / name
        shutil.copy2(HERE / name, destination)
        return destination

    @unittest.skipUnless(shutil.which("powershell") or shutil.which("pwsh"), "PowerShell is unavailable")
    def test_game_script_quotes_spaced_main_path_for_start_process(self):
        launcher = self.copy_launcher("start-game.ps1")
        runtime = self.root / "bin" / "Release" / "daslang-live.exe"
        runtime.parent.mkdir(parents=True)
        runtime.touch()
        capture = self.root / "start-process.json"
        shell = shutil.which("powershell") or shutil.which("pwsh")
        wrapper = """
function global:Invoke-RestMethod { throw 'offline' }
function global:Start-Process {
    param($FilePath, $ArgumentList, $WorkingDirectory, $WindowStyle,
          $RedirectStandardOutput, $RedirectStandardError)
    $ArgumentList | ConvertTo-Json | Set-Content -LiteralPath $env:LAUNCH_CAPTURE
}
. $env:LAUNCH_SCRIPT
"""
        env = os.environ | {"LAUNCH_CAPTURE": str(capture), "LAUNCH_SCRIPT": str(launcher)}
        subprocess.run([shell, "-NoProfile", "-Command", wrapper], check=True, env=env,
                       capture_output=True, text=True)
        arguments = json.loads(capture.read_text(encoding="utf-8-sig"))
        self.assertEqual(arguments[1], f'"{self.latchpoint / "main.das"}"')

    @unittest.skipUnless(shutil.which("powershell") or shutil.which("pwsh"), "PowerShell is unavailable")
    def test_powershell_server_forwards_absolute_config(self):
        launcher = self.copy_launcher("start-server.ps1")
        runtime = self.root / "bin" / "Release" / "daslang.exe"
        runtime.parent.mkdir(parents=True)
        runtime.touch()
        config = self.root / "config files" / "server.toml"
        config.parent.mkdir()
        config.touch()
        capture = self.root / "python-args.json"
        shell = shutil.which("powershell") or shutil.which("pwsh")
        wrapper = """
function global:python { $args | ConvertTo-Json | Set-Content -LiteralPath $env:LAUNCH_CAPTURE }
Set-Location -LiteralPath $env:CONFIG_PARENT
. $env:LAUNCH_SCRIPT -Config 'server.toml'
"""
        env = os.environ | {
            "LAUNCH_CAPTURE": str(capture),
            "LAUNCH_SCRIPT": str(launcher),
            "CONFIG_PARENT": str(config.parent),
        }
        subprocess.run([shell, "-NoProfile", "-Command", wrapper], check=True, env=env,
                       capture_output=True, text=True)
        arguments = json.loads(capture.read_text(encoding="utf-8-sig"))
        config_index = arguments.index("--config")
        self.assertEqual(Path(arguments[config_index + 1]), config)

    @unittest.skipUnless(shutil.which("bash"), "bash is unavailable")
    def test_shell_server_forwards_absolute_config(self):
        launcher = self.copy_launcher("start-server.sh")
        runtime = self.root / "bin" / "daslang"
        runtime.parent.mkdir(parents=True)
        runtime.touch()
        runtime.chmod(runtime.stat().st_mode | stat.S_IXUSR)
        config = self.root / "config files" / "server.toml"
        config.parent.mkdir()
        config.touch()
        stub_dir = self.root / "stubs"
        stub_dir.mkdir()
        capture = self.root / "python-args.txt"
        def bash_path(path):
            if os.name != "nt":
                return str(path)
            drive, tail = os.path.splitdrive(str(path))
            return f"/mnt/{drive[0].lower()}/{tail.lstrip('\\/').replace(os.sep, '/')}"

        python_stub = stub_dir / "python3"
        python_stub.write_text(
            f'#!/usr/bin/env bash\nprintf "%s\\n" "$@" > {shlex.quote(bash_path(capture))}\n',
            encoding="utf-8", newline="\n")
        python_stub.chmod(python_stub.stat().st_mode | stat.S_IXUSR)
        command = (
            f"export PATH={shlex.quote(bash_path(stub_dir))}:/usr/local/bin:/usr/bin:/bin; "
            f"cd -- {shlex.quote(bash_path(config.parent))} && "
            f"exec bash {shlex.quote(bash_path(launcher))} server.toml"
        )
        completed = subprocess.run(
            ["bash", "-c", command], capture_output=True, text=True)
        self.assertEqual(completed.returncode, 0, completed.stderr)
        arguments = capture.read_text(encoding="utf-8").splitlines()
        config_index = arguments.index("--config")
        self.assertEqual(arguments[config_index + 1], bash_path(config))


if __name__ == "__main__":
    unittest.main()
