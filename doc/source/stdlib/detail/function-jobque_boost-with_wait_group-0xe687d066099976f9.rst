Creates a wait group with the given notification count and auto-joins after the block.
Each unit of work should call ``notify_and_release`` (or ``done``) when complete.
The block returns only after all notifications have been received.
