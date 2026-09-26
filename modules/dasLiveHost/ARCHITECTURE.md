# Live host architecture

## 1. Capture response serialization {#capture-response-serialization}

`live_capture_flush` encodes the captured JSON tree once. It writes those bytes as
the saved capture and inserts the same bytes as `snapshot` in the transport response.
The surrounding response fields are encoded as JSON metadata before insertion, so
quoted IDs and other strings remain escaped. Reusing the tree avoids a second full
traversal and keeps the saved and transported snapshots identical.
