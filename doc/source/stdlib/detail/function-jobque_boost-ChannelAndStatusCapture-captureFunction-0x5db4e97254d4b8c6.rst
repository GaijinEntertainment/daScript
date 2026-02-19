Implementation details for the capture macro.
Note: generators are skipped because their ``finally`` section is called on every
iteration, not just once at destruction. This means ``Channel``, ``JobStatus``, and
``LockBox`` captures inside generators do NOT get automatic release checks.
Users must manually call ``release`` on captured jobque objects inside generators.
