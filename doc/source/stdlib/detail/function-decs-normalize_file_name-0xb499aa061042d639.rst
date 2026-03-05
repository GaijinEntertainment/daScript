Strips directory path, keeping only filename and extension.
This ensures the same canonical name regardless of working directory,
which is critical for AOT hash stability.
