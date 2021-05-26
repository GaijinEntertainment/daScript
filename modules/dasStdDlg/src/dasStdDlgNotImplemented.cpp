#include "daScript/misc/platform.h"

#include "dasStdDlg.h"

namespace das {

	bool GetOkCancelFromUser(const char *, const char *) {
	    return false;
	}

	bool GetOkFromUser(const char *, const char *) {
        return false;
	}

	string GetSaveFileFromUser ( const char *, const char *, const char * ) {
        return "";
	}

	string GetOpenFileFromUser ( const char *, const char * ) {
        return "";
	}
}
