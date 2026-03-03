Opens an XML file, invokes *blk* with the parsed document and a
success flag, then automatically frees the document. The caller
never needs ``unsafe { delete ... }``.
