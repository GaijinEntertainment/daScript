Capture macro for channel and job status objects in lambda captures.
This macro implements capturing of the `jobque::Channel` and `jobque::JobStatus` types.
When captured reference counts are increased. When lambda is destroyed, reference counts are decreased.
