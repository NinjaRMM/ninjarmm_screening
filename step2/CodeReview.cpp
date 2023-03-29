// Copyright (c) 2023. Alexandre Horst
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
//  associated documentation files (the "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
//  of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following
//   conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial
// portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
// OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//

/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/


//Review: Use of undeclared identifier 'std::wstring'
#include <string>
//Review: Use of undeclared identifier 'std::map'
#include <map>
//Review: Use of undeclared identifier 'std::cout'
#include <iostream>
//Review: Use of undeclared Windows headers
#include <Windows.h>
//Review: Use of wscapi.h and iwscapi.h'
#include <wscapi.h>
#include <iwscapi.h>

//Review: The struct name AVSoftware could be simplified to AVSoftware since it's already clear that it's a third-party software from the context.
struct AVSoftware {
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, AVSoftware> &par_avs_software_map) {
    HRESULT hr = S_OK;
    IWscProduct *PtrProduct = nullptr;
    IWSCProductList *PtrProductList = nullptr;
    BSTR PtrVal = nullptr;
    LONG ProductCount = 0;
    WSC_SECURITY_PRODUCT_STATE ProductState;
    WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

    //Review: All variables could be condensed in one structure AVSoftware temporary variable avs.
    AVSoftware avs;

    //Review: Use nullptr instead of NULL
    hr = CoCreateInstance(__uuidof(WSCProductList), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList),
                          reinterpret_cast<LPVOID *>(&PtrProductList));
    if (FAILED(hr)) {
        //Review: The error messages should use `std::cerr` to show the error messages on the console.
        std::cerr << "Failed to create WSCProductList object." << std::endl;
        return false;
    }

    hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
    if (FAILED(hr)) {
        std::cerr << "Failed to query antivirus product list. " << std::endl;
        return false;
    }

    hr = PtrProductList->get_Count(&ProductCount);
    if (FAILED(hr))
    {
        std::cerr << "Failed to query product count." << std::endl;
        return false;
    }

    for (uint32_t i = 0; i < ProductCount; i++)
    {
        hr = PtrProductList->get_Item(i, &PtrProduct);
        if (FAILED(hr))
        {
            std::cerr << "Failed to query AV product." << std::endl;
            continue;
        }

        hr = PtrProduct->get_ProductName(&PtrVal);
        if (FAILED(hr))
        {
            PtrProduct->Release();
            std::cerr << "Failed to query AV product name." << std::endl;
            continue;
        }

        avs.Name = std::wstring(PtrVal, SysStringLen(PtrVal));

        hr = PtrProduct->get_ProductState(&ProductState);
        if (FAILED(hr))
        {
            std::cerr << "Failed to query AV product state." << std::endl;
            continue;
        }

        if (ProductState == WSC_SECURITY_PRODUCT_STATE_ON)
        {
            avs.ProductState = L"On";
        }
        else if (ProductState == WSC_SECURITY_PRODUCT_STATE_OFF)
        {
            avs.ProductState = L"Off";
        }
        else
        {
            avs.ProductState = L"Expired";
        }

        hr = PtrProduct->get_SignatureStatus(&ProductStatus);
        if (FAILED(hr)) {
            //Review: Add a breakline to not mix the error messages with the output.
            std::cerr << "Failed to query AV product definition state." << std::endl;
            continue;
        }

        avs.DefinitionStatus = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

        hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
        if (FAILED(hr)) {
            std::cerr << "Failed to query AV product definition state." << std::endl;
            continue;
        }
        avs.DefinitionUpdateTime = std::wstring(PtrVal, SysStringLen(PtrVal));
        SysFreeString(PtrVal);

        //Review: The Description and ProductState have the same value. I think it should use empty strings instead.
        avs.Description = L"";

        //Review: This could simplify the code using the same structure AVSoftware which is being filled by the query.
        par_avs_software_map[avs.Name] = avs;

        PtrProduct->Release();
    }

    //Review: The 'empty' method should be used to check for emptiness instead of 'size'
    if (par_avs_software_map.empty()) {
        return false;
    }
    return true;
}

//Review: I added the main function to test the code.
int
__cdecl
wmain(
        _In_              int argc,
        _In_reads_(argc)  LPCWSTR argv[]
) {

    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    std::map<std::wstring, AVSoftware> avs_software_map;
    if (queryWindowsForAVSoftwareDataWSC(avs_software_map)) {
        for (auto &av_software: avs_software_map) {
            std::wcout << "Name: " << av_software.second.Name << std::endl;
            std::wcout << "Description: " << av_software.second.Description << std::endl;
            std::wcout << "DefinitionUpdateTime: " << av_software.second.DefinitionUpdateTime << std::endl;
            std::cout << "DefinitionStatus: " << av_software.second.DefinitionStatus << std::endl;
            std::wcout << "Version: " << av_software.second.Version << std::endl;
            std::wcout << "ProductState: " << av_software.second.ProductState << std::endl;
        }
    } else {
        std::cout << "No AV software found." << std::endl;
    }
    CoUninitialize();
    return 0;
}