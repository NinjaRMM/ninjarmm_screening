/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/

// see my comments below tagged with "stan:"
// stan: add all the includes
#include <iostream>
#include <map>
#include <string>
#include "Winerror.h"
#include "iwscapi.h"
#include "wscapi.h"
#include <atlbase.h>

struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
    std::wstring Version;
    std::wstring ProductState;
};

bool queryWindowsForAVSoftwareDataWSC(std::map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
{
   HRESULT hr = S_OK;
   // stan: use smart COM pointer
   //IWscProduct* PtrProduct = nullptr;
   //-->
   CComPtr<IWscProduct> PtrProduct = nullptr;
   // stan: use smart COM pointer
   //IWSCProductList* PtrProductList = nullptr;
   //-->
   CComPtr<IWSCProductList> PtrProductList = nullptr;
   BSTR PtrVal = nullptr;
   LONG ProductCount = 0;
   WSC_SECURITY_PRODUCT_STATE ProductState;
   WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

   std::wstring displayName, versionNumber, state, timestamp;
   std::string definitionState;

   // stan: use IID_PPV_ARGS which would allow the compiler to catch additional errors
   //hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), reinterpret_cast<LPVOID*>(&PtrProductList));
   //-->
   hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&PtrProductList));
   if (FAILED(hr))
   {
      std::cout << "Failed to create WSCProductList object. ";
      return false;
   }

   hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
   if (FAILED(hr))
   {
      std::cout << "Failed to query antivirus product list. ";
      return false;
   }

   hr = PtrProductList->get_Count(&ProductCount);
   if (FAILED(hr))
   {
      std::cout << "Failed to query product count.";
      return false;
   }

   // stan: change uint32_t to LONG
   //for (uint32_t i = 0; i < ProductCount; i++)
   // -->
   for (LONG i = 0; i < ProductCount; i++)
   {
      hr = PtrProductList->get_Item(i, &PtrProduct);
      if (FAILED(hr))
      {
         std::cout << "Failed to query AV product.";
         continue;
      }

      hr = PtrProduct->get_ProductName(&PtrVal);
      if (FAILED(hr))
      {
         // stan: should be deleted
         //PtrProduct->Release();
         std::cout << "Failed to query AV product name.";
         continue;
      }

      displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

      hr = PtrProduct->get_ProductState(&ProductState);
      if (FAILED(hr))
      {
         std::cout << "Failed to query AV product state.";
         continue;
      }

      if (ProductState == WSC_SECURITY_PRODUCT_STATE_ON)
      {
         state = L"On";
      }
      else if (ProductState == WSC_SECURITY_PRODUCT_STATE_OFF)
      {
         state = L"Off";
      }
      else
      {
         state = L"Expired";
      }

      hr = PtrProduct->get_SignatureStatus(&ProductStatus);
      if (FAILED(hr))
      {
         std::cout << "Failed to query AV product definition state.";
         continue;
      }

      if (ProductState == WSC_SECURITY_PRODUCT_STATE_ON)
      {
         state = L"On";
      }
      else if (ProductState == WSC_SECURITY_PRODUCT_STATE_OFF)
      {
         state = L"Off";
      }
      else
      {
         state = L"Expired";
      }

      hr = PtrProduct->get_SignatureStatus(&ProductStatus);
      if (FAILED(hr))
      {
         std::cout << "Failed to query AV product definition state.";
         continue;
      }

      definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

      hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
      if (FAILED(hr))
      {
         std::cout << "Failed to query AV product definition state.";
         continue;
      }
      timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
      SysFreeString(PtrVal);

      ThirdPartyAVSoftware thirdPartyAVSoftware;
      thirdPartyAVSoftware.Name = displayName;
      thirdPartyAVSoftware.DefinitionStatus = definitionState;
      thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
      thirdPartyAVSoftware.Description = state;
      thirdPartyAVSoftware.ProductState = state;
      thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;

      // stan: should be deleted
      //PtrProduct->Release();
   }

   if (thirdPartyAVSoftwareMap.size() == 0)
   {
      return false;
   }
   return true;
}
