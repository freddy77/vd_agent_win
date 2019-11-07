/*
   Copyright (C) 2019 Red Hat, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "shell.h"

#include <stdio.h>
#include <exdisp.h>
#include <comutil.h>

static bool filter_url(LPWSTR str)
{
    // check and strip "file:///?"
    WCHAR *src = str;
    if (wcsncmp(str, L"file:///", 8) == 0) {
        src += 8;
    } else if (wcsncmp(str, L"file://", 7) == 0) {
        src += 7;
    } else {
        return false;
    }

    WCHAR *dst = str;
    while (*src) {
        WCHAR c = *src++, save_char;
        switch (c) {
        // convert Unix to DOS path separators
        case L'/':
            c = L'\\';
            break;
        // hexadecimal encoding
        case L'%':
            if (!iswxdigit(src[0]) || !iswxdigit(src[1])) {
                return false;
            }
            save_char = src[2];
            src[2] = 0;
            c = (WCHAR) wcstol(src, NULL, 16);
            src[2] = save_char;
            src += 2;
            break;
        // NOTE: do not convert + to space, paths like "C++"
        // remain the same
        default:
            break;
        }
        *dst++ = c;
    }
    *dst = 0;

    return true;
}

bool open_shell_directory(LPCWSTR directory)
{
    HRESULT result;

    IShellWindows *shellWindows = nullptr;
    result = CoCreateInstance(
        CLSID_ShellWindows,
        NULL,
        CLSCTX_ALL,
        IID_IShellWindows,
        (void**)&shellWindows
    );
    if (FAILED(result)) {
        return false;
    }

    long windowsCount = 0;
    result = shellWindows->get_Count(&windowsCount);
    if (FAILED(result)) {
        shellWindows->Release();
        return false;
    }

    bool found = false;
    for (long i = 0; i < windowsCount; i++) {
        IDispatch *disp = nullptr;
        _variant_t va(i, VT_I4);

        result = shellWindows->Item(va, &disp);
        if (FAILED(result)) {
            continue;
        }

        IWebBrowser2 *pBrowser2 = nullptr;
        result = disp->QueryInterface(IID_IWebBrowserApp, (void**)&pBrowser2);

        if (SUCCEEDED(result)) {
            BSTR url;
            pBrowser2->get_LocationURL(&url);

            if (filter_url(url) && lstrcmpiW(url, directory) == 0) {
                found = true;
            }
            SysFreeString(url);
            pBrowser2->Release();
        }

        disp->Release();
    }
    shellWindows->Release();

    if (!found) {
        ShellExecuteW(NULL, L"open", directory, NULL, NULL, SW_SHOWNORMAL);
    }

    return true;
}
