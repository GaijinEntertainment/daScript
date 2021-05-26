#pragma once


namespace das {
    // shared impl
	bool GetOkCancelFromUser(const char * caption, const char * body);
    bool GetOkFromUser(const char * caption, const char * body);

    // C++ impl
	string GetSaveFileFromUser ( const char * initialFileName , const char * initialPath, const char * filter );
	string GetOpenFileFromUser ( const char * initialPath, const char * filter );

    // and das bindings
    class Context;
    char * GetSaveFileDlg ( const char * initialFileName , const char * initialPath, const char * filter, Context * ctx );
}