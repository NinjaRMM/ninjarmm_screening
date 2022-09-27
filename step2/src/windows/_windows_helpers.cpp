#include <_windows_helpers.h>
#include <Windows.h>
#include <algorithm>

// Convert a wide Unicode string to an UTF8 string
std::string utf8_encode(const wchar_t* wstr)
{
	int len = wcslen(wstr);
    if (!len) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)len, NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)len, &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

void trimNullTerminator(std::wstring& strToTrim) {
    strToTrim = strToTrim.erase(strToTrim.length());
}

void trimWhiteSpace(std::wstring& str) {
	if (!str.empty()) {
		int whiteSpaceStart = str.find_last_not_of(L" \t");
		str.erase(whiteSpaceStart + 1);
		int whiteSpaceStartBeginning = str.find_first_not_of(L" \n\t");
		str.erase(str.begin(), str.end() - (str.length() - whiteSpaceStartBeginning));
	}
}

void removeTabulation(std::wstring& str) {
	str.erase(std::remove(str.begin(), str.end(), L'\t'), str.end());
}

bool wcharEqualsPredicate(wchar_t a, wchar_t b) {
	return a == b && a == ' ';
}

void condenseSpaces(std::wstring& str) {
	std::wstring::iterator new_end =
		std::unique(str.begin(), str.end(), wcharEqualsPredicate);
	str.erase(new_end, str.end());
}
