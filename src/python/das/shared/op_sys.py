import subprocess
import os
from os import path


class RunCmdError(Exception):

    MAX_OUTPUT_LEN = 5000

    def __init__(self, cmd, stdout, stderr, exit_code):
        super(RunCmdError, self).__init__(
            'Command: {cmd}\n'
            'Exit code: {exit_code}\n'
            'Stdout:\n{stdout}\n'
            'Stderr:\n{stderr}'.format(
                cmd=cmd,
                exit_code=exit_code,
                stdout=stdout[-self.MAX_OUTPUT_LEN:],
                stderr=stderr[-self.MAX_OUTPUT_LEN:]))


def make_dirs(dpath):
    MAX_TRIES = 100
    for _ in range(MAX_TRIES):
        try:
            os.makedirs(dpath)
        except FileExistsError:
            pass
        if path.isdir(dpath):
            break
        sleep(0.1)
    else:
        raise BinderError(f'Cannot create directory: {dpath}')

def write_to_file(fpath, content):
    make_dirs(path.dirname(fpath))
    with open(fpath, 'w') as f:
        f.write(content)

def run_exec(cmd, raise_on_error=True):
    result = subprocess.run(cmd, shell=False, capture_output=True)
    stdout = result.stdout.decode()
    stderr = result.stderr.decode()
    exit_code = result.returncode

    if raise_on_error and exit_code != 0:
        raise RunCmdError(cmd=cmd, stdout=stdout, stderr=stderr,
            exit_code=exit_code)

    return stdout, stderr, exit_code

def full_path(p):
    return path.realpath(path.abspath(p))
