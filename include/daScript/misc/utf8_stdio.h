#pragma once

#include "platform.h"
#include "daScript/simulate/simulate.h"
#include <string>

#if !defined(DAS_NO_FILEIO)

#include <stdio.h>

namespace das {

      FILE * fopen_utf8(const char * name, const char * mode, bool share_deny_write = false);
      FILE * popen_utf8(const char * command);
      const char * dirname_utf8(const char * path, Context * context); // allocation in context->stringHeap
      std::string dirname_utf8(const char * path);
      const char * basename_utf8(const char * path, Context * context); // allocation in context->stringHeap
      std::string basename_utf8(const char * path);

#if defined(_MSC_VER)
      bool stat_utf8(const char * name, struct _stati64 * buffer);
#else
      bool stat_utf8(const char * name, struct stat * buffer);
#endif

      typedef void (* DirCallback)(const char * file_name, void * user_pointer);
      int dir_utf8(const char * path, DirCallback callback, void * user_pointer = nullptr);
      int mkdir_utf8(const char * name);

      bool unlink_file_utf8(const char * file_name);
      bool rename_file_utf8(const char * file_from, const char * file_dest);
}

#endif

namespace das {
      // normalizeFileName ->
      const char * get_full_path_name_utf8(const char * path, Context * context); // allocation in context->stringHeap
      std::string get_full_path_name_utf8(const char * path);
}


