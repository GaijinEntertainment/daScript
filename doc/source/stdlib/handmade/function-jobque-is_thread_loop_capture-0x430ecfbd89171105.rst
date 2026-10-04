Returns whether an address lies inside this thread's active repeated-lambda
capture. Internal release hooks use this to defer captured-handle checks until
the final step; nested captures keep their ordinary release checks.
