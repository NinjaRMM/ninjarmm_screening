/*

NINJARMM Code Review

Please review the below code.
We do not expect you to execute this code, but you are welcome to try.

Make any code updates that you see fit (If any).
Comments are encouraged.

*/

// General comment: I will not modify code but will put code blocks in my code
// review I hope this works too. The reason being I dont fill comfortable
// writing comments and changing code at the same time - it wont be clear
// if I put a comment under the line that I modified. I understood this task
// as a code review task so I will write comments similarly to how I would have
// done at work.

struct ThirdPartyAVSoftware {
  std::wstring Name;
  std::wstring Description;
  std::wstring DefinitionUpdateTime;
  std::string DefinitionStatus;
  std::wstring Version;
  std::wstring ProductState;
};
// In this file includes are mising - std::wstring, std::map , .h of CodeReview
// (I suppose this function wil be used somewhere else), CoCreateInstance,
// WSCProductList ... please make it compilable.
bool queryWindowsForAVSoftwareDataWSC(
    std::map<std::wstring, ThirdPartyAVSoftware> &thirdPartyAVSoftwareMap) {
  HRESULT hr = S_OK;
  IWscProduct *PtrProduct = nullptr;
  IWSCProductList *PtrProductList = nullptr;
  // In moderne C++ the use of raw pointers is highly discouraged
  // if there is a possibility to rewrite CoCreateInstance to use shared_ptr
  // or unique_ptr please do so.
  BSTR PtrVal = nullptr;
  LONG ProductCount = 0;
  WSC_SECURITY_PRODUCT_STATE ProductState;
  WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

  std::wstring displayName, versionNumber, state, timestamp;
  std::string definitionState;

  hr = CoCreateInstance(__uuidof(WSCProductList), NULL, CLSCTX_INPROC_SERVER,
                        __uuidof(IWSCProductList),
                        reinterpret_cast<LPVOID *>(&PtrProductList));

  if (FAILED(hr)) {
    std::cout << "Failed to create WSCProductList object. ";
    // Here and for all other 'std::cout' - better to use log function defined
    // for this translation unit or namespace. This would allow in the future to
    // add timestamps nzme all the file etc for all the logs.
    return false;
  }

  hr = PtrProductList->Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS);
  if (FAILED(hr)) {
    std::cout << "Failed to query antivirus product list. ";
    return false;
  }

  hr = PtrProductList->get_Count(&ProductCount);
  if (FAILED(hr)) {
    std::cout << "Failed to query product count.";
    return false;
  }
  // MINOR: Lines 49-70 can be unitied in a stand alone function.
  // It is the initialization of the PtrProductList and ProductCount
  for (uint32_t i = 0; i < ProductCount; i++) {
    hr = PtrProductList->get_Item(i, &PtrProduct);
    if (FAILED(hr)) {
      std::cout << "Failed to query AV product.";
      continue;
    }

    hr = PtrProduct->get_ProductName(&PtrVal);
    if (FAILED(hr)) {
      PtrProduct->Release();
      std::cout << "Failed to query AV product name.";
      continue;
    }

    displayName = std::wstring(PtrVal, SysStringLen(PtrVal));

    hr = PtrProduct->get_ProductState(&ProductState);
    if (FAILED(hr)) {
      std::cout << "Failed to query AV product state.";
      continue;
    }

    if (ProductState == WSC_SECURITY_PRODUCT_STATE_ON) {
      state = L"On";
    } else if (ProductState == WSC_SECURITY_PRODUCT_STATE_OFF) {
      state = L"Off";
    } else {
      state = L"Expired";
    }

    hr = PtrProduct->get_SignatureStatus(&ProductStatus);
    if (FAILED(hr)) {
      std::cout << "Failed to query AV product definition state.";
      continue;
    }

    definitionState = (ProductStatus == WSC_SECURITY_PRODUCT_UP_TO_DATE)
                          ? "UpToDate"
                          : "OutOfDate";

    hr = PtrProduct->get_ProductStateTimestamp(&PtrVal);
    if (FAILED(hr)) {
      std::cout << "Failed to query AV product definition state.";
      continue;
    }
    timestamp = std::wstring(PtrVal, SysStringLen(PtrVal));
    SysFreeString(PtrVal);
    // Lines 82 till this line:
    // I would suggest to wrap this in a function.
    // Something like ;
    // bool get_ThirdPartyAVSoftwareForProduct(const PtrProduct&, size_t ,
    // ThirdPartyAVSoftware&) inside of this function I would suggest to adress
    // the SysFreeString issue. Early exits shouldn't lead to memory licks. The
    // best way would be to create a  wrapper object for both PtrVal so that in
    // a detructor of this object SysFreeString is called. This would allow also
    // not to leak recources in case of exeptions being thrown

    // the same applies to PtrProduct->Release();
    // If my suggesstions with get_ThirdPartyAVSoftwareForProduct is implemented
    // then the code would look like this :
    // for (uint32_t i = 0; i < ProductCount; i++) {
    //     hr = PtrProductList->get_Item(i, &PtrProduct);
    //     if (FAILED(hr))
    //    {
    //        std::cout << "Failed to query AV product.";
    //        continue;
    //    }
    //    ThirdPartyAVSoftware thirdPartyAVSoftware;
    //    get_ThirdPartyAVSoftwareForProduct(PtrProduct, i ,
    //    thirdPartyAVSoftware); PtrProduct->Release();
    //    ....
    // }
    // while this would resolve the problem of early exists for
    // PtrProduct->Release(); it is still preferable to wrap PtrProduct in an
    // object where Release is called in a destructor due to possible exeptions
    // that can lead to leaked resources.

    ThirdPartyAVSoftware thirdPartyAVSoftware;
    thirdPartyAVSoftware.Name = displayName;
    thirdPartyAVSoftware.DefinitionStatus = definitionState;
    thirdPartyAVSoftware.DefinitionUpdateTime = timestamp;
    thirdPartyAVSoftware.Description = state;
    thirdPartyAVSoftware.ProductState = state;
    thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;
    // Operator [] doesnt tell us if element is overriden. Is it ok to override
    // existing values ?
    PtrProduct->Release();
  }

  if (thirdPartyAVSoftwareMap.size() == 0) {
    return false;
  }
  // lines 171-174: If the map argument wasnt empty this return will return true
  // however may be no data has been added/updated in the map. Logically it has
  // different meaning compared to a return on line 55.
  return true;
}
