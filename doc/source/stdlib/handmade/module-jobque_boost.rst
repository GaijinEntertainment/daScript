The JOBQUE_BOOST module provides high-level job queue abstractions built on
the low-level ``jobque`` primitives: ``new_job`` / ``new_thread`` (which capture
a lambda and clone the context for the worker), ``with_wait_group`` / ``done``,
``parallel_for`` and the ``team_parallel_*`` family, and typed ``push`` /
``pop`` / ``gather`` over a ``Stream``. It requires ``jobque`` publicly, so the
builtins — ``with_job_que``, ``with_job_status``, ``Channel``, ``LockBox`` —
are visible through it as well.

See also :doc:`jobque` for the low-level job queue primitives.
See :ref:`tutorial_jobque` for a hands-on tutorial.

All functions and symbols are in "jobque_boost" module, use require to get access to it.

.. code-block:: das

    require daslib/jobque_boost

Example:

.. code-block:: das

    require daslib/jobque_boost

    [export]
    def main() {
        with_job_status(1) $(status) {
            new_thread() @() {
                print("from thread\n")
                status |> notify_and_release()
            }
            status |> join()
            print("thread done\n")
        }
    }
    // output:
    // from thread
    // thread done
