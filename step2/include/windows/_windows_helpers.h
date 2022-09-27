#pragma once
#include <string>
std::string utf8_encode(const wchar_t* wstr);
/*
void trimNullTerminator(std::wstring& strToTrim);

void trimWhiteSpace(std::wstring& str);

void removeTabulation(std::wstring& str);

void condenseSpaces(std::wstring& str);
*/

#include <OAIdl.h>
using namespace std;
class DeleteVariantHelper
{
public:
	DeleteVariantHelper() = delete;
	DeleteVariantHelper(VARIANT* ptr) : _ptr(ptr)
	{}
	~DeleteVariantHelper()
	{
		if (_ptr)
			VariantClear(_ptr);

	}
private:
	VARIANT* _ptr;


};
