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
#undef NDEBUG
#include "shell.cpp"

#include <assert.h>

static void check_filter(LPCWSTR in, LPCWSTR out)
{
    WCHAR buf[1000];
    assert(in != NULL);
    assert(wcslen(in) < 1000);
    wcscpy(buf, in);
    if (filter_url(buf)) {
        assert(out != NULL);
        assert(wcscmp(buf, out) == 0);
    } else {
        assert(out == NULL);
    }
}

int main(int argc, char **argv)
{
    check_filter(L"c:\\", NULL);
    check_filter(L"file:///c:/", L"c:\\");
    check_filter(L"file://test%20", L"test ");
    check_filter(L"file://test%2", NULL);
    check_filter(L"file://a%g0", NULL);
    check_filter(L"file://a%E0", L"a\xe0");
    check_filter(L"file:///b+c", L"b+c");
    check_filter(L"file://a\u4141", L"a\u4141");

    return 0;
}
