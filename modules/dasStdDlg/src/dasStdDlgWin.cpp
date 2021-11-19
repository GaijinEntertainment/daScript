#include "daScript/misc/platform.h"

#include "dasStdDlg.h"

#include <windows.h>
#include <Shobjidl.h>
#include <Shlwapi.h>
#include <codecvt>

namespace das {

	wstring string_to_wstring ( const string& utf8String ) {
		wstring_convert<codecvt_utf8<wchar_t>> convert;
		return convert.from_bytes(utf8String);
	}

	string wstring_to_string ( const wstring & wideString ) {
		wstring_convert<codecvt_utf8<wchar_t>> convert;
		return convert.to_bytes(wideString);
	}

	vector<string> split ( const char * str, const char * delim );

	HWND GetHWND() {
		return 0;
	}

	void StdDlgInit() {
	}

	bool GetOkCancelFromUser(const char * caption, const char * body) {
		wstring wcaption = string_to_wstring(caption ? caption : "");
		wstring wbody = string_to_wstring(body ? body : "");
		return MessageBoxW ( GetHWND(), wbody.c_str(), wcaption.c_str(), MB_OKCANCEL) == IDOK;
	}

	bool GetOkFromUser(const char * caption, const char * body) {
		wstring wcaption = string_to_wstring(caption ? caption : "");
		wstring wbody = string_to_wstring(body ? body : "");
		MessageBoxW ( GetHWND(), wbody.c_str(), wcaption.c_str(), MB_OK);
		return true;
	}

	void ParseFilter(const char* filter, wstring & filterStr, vector<wstring> & extensions) {
		if (!filter) return;
		vector<string> exts = split(filter, "|");
		for (size_t i = 0; i < exts.size(); ++i) {
			extensions.push_back(string_to_wstring(exts[i]));
			filterStr.append(string_to_wstring(string("*.") + exts[i]));
			if (i < (exts.size() - 1)) {
				filterStr.append(L";");
			}
		}
	}

	string GetSaveFileFromUser ( const char * initialFileName , const char * initialPath, const char * filter ) {
		IFileSaveDialog * dialog = nullptr;
		if (FAILED(CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog)))) return string();
		wstring filterStr;
		vector<wstring> extensions;
		ParseFilter(filter, filterStr, extensions);
		if (extensions.size() == 0 || filterStr.empty()) return string();
		COMDLG_FILTERSPEC filterSpec;
		filterSpec.pszName = L"File type(s)";
		filterSpec.pszSpec = filterStr.c_str();
		if (FAILED(dialog->SetFileTypes(1, &filterSpec))) return string();
		if (FAILED(dialog->SetDefaultExtension(extensions[0].c_str()))) return string();
		if (initialPath && *initialPath) {
			IShellItem * folder = nullptr;
			wstring path = string_to_wstring(initialPath);
			if (PathIsDirectoryW(path.c_str())) {
				if (SUCCEEDED(SHCreateItemFromParsingName(path.c_str(), NULL, IID_PPV_ARGS(&folder)))) {
					dialog->SetDefaultFolder(folder);
				}
			}
		}
		if (initialFileName) {
			wstring fileName = string_to_wstring(initialFileName);
			dialog->SetFileName(fileName.c_str());
		}
		dialog->Show(GetHWND());
		IShellItem * shellItem = nullptr;
		if (FAILED(dialog->GetResult(&shellItem))) return string();
		PWSTR filePath = NULL;
		if (FAILED(shellItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath))) return string();
		wstring result = filePath;
		CoTaskMemFree(filePath);
		return wstring_to_string(result);
	}

	string GetOpenFileFromUser ( const char * initialPath, const char * filter ) {
		IFileOpenDialog * dialog = nullptr;
		if (FAILED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog)))) return string();
		wstring filterStr;
		vector<wstring> extensions;
		ParseFilter(filter, filterStr, extensions);
		if (extensions.size() == 0 || filterStr.empty()) return string();
		COMDLG_FILTERSPEC filterSpec[] = {
			{ L"File type(s)" , filterStr.c_str()},
			{ L"All", L"*.*" },
		};
		if (FAILED(dialog->SetFileTypes(2, filterSpec))) return string();
		if (FAILED(dialog->SetDefaultExtension(extensions[0].c_str()))) return string();
		if (initialPath && *initialPath) {
			IShellItem * folder = nullptr;
			wstring path = string_to_wstring(initialPath);
			if (PathIsDirectoryW(path.c_str())) {
				if (SUCCEEDED(SHCreateItemFromParsingName(path.c_str(), NULL, IID_PPV_ARGS(&folder)))) {
					dialog->SetDefaultFolder(folder);
				}
			} else if (PathFileExistsW(path.c_str())) {
				wstring fileName = PathFindFileNameW(path.c_str());
				if (path.length() <= MAX_PATH &&
					PathRemoveFileSpecW(&path[0]) &&
					SUCCEEDED(SHCreateItemFromParsingName(path.c_str(), NULL, IID_PPV_ARGS(&folder)))) {
						dialog->SetFolder(folder);
						if (!fileName.empty()) {
							dialog->SetFileName(fileName.c_str());
						}
				}
			}
		}
		dialog->Show(GetHWND());
		IShellItem * shellItem = nullptr;
		if (FAILED(dialog->GetResult(&shellItem))) return string();
		PWSTR filePath = NULL;
		if (FAILED(shellItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath))) return string();
		wstring result = filePath;
		CoTaskMemFree(filePath);
		return wstring_to_string(result);
	}
}
