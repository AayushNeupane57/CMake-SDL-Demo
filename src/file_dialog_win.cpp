#include <windows.h>
#include <ShlObj.h>
#include <atlstr.h>
#include <string>

#include "file_dialog.h"

std::string FileDialog::openFile() {
	std::string filePath = "";
	IFileDialog *pfd = NULL;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
	if (SUCCEEDED(hr)) {
		hr = pfd->Show(NULL);
		if (SUCCEEDED(hr)) {
			IShellItem* res = NULL;
			hr = pfd->GetResult(&res);
			if (hr == S_OK) {
				LPWSTR path = NULL;
				res->GetDisplayName(SIGDN_FILESYSPATH, &path);
				filePath = CW2A(path);
			}
			res->Release();
		}
	}
	pfd->Release();
	return filePath;
}