/*

NINJARMM Code Review
 
Please review the below code. 
We do not expect you to execute this code, but you are welcome to try. 

Make any code updates that you see fit (If any). 
Comments are encouraged.

*/

// I've added these includes, since I wanted to test and tweak the code a little bit
#include <string>
#include <unordered_map>
#include <iostream>
#include <iwscapi.h>
#include <wscapi.h>
#include <wrl\client.h> // For ComPtr class


struct ThirdPartyAVSoftware
{
    std::wstring Name;
    std::wstring Description;
    std::wstring DefinitionUpdateTime;
    std::string DefinitionStatus;
	// This doesn't seem to be used, and looking into the interface, there are no methods listing the product's version,
	// so I ended up commenting it out, so I can reduce the memory footprint for this object
    //std::wstring Version;
    std::wstring ProductState;
};


// I chose to use a class with a static method to provide a list of products, since this (to me) is more organized;
// I've also added a COMInitializer object otherwise the code won't run
class AVProductInformation
{
private:
    struct COMInitializer
    {
        COMInitializer()
        {
			// Even though Microsoft's enums tend not to be strong typed, I still like to enforce the scope
            auto hr = CoInitializeEx(0, COINIT::COINIT_APARTMENTTHREADED);
            if (FAILED(hr))
            {
                std::cout << "Failed to initialize COM -> error " << hr;
                exit(hr);
            }
        }
        ~COMInitializer()
        {
            CoUninitialize();
        }
    };

public:
	// One thing I changed from the original call was the container used for storing the resulting data. Originally, a map was used, and it's
	// not that it is a bad approach, internally it's a red-black tree and will do look-ups in O(log N). I changed that to a hash table,
	// which is what an unordered_map is, and while the worst-case scenario for look-ups is O(N), the average case is O(1), since we
	// use hashing to find the correct position / bucket; moreover, because the key is a string, we also have to do more string tests.
	// I don't know if I'm allowed to do that to this code, and I don't know what you guys do in the internals of your system, but
	// unless you really need these keys in order, the unordered_map is a better option, IMHO.
	// Overall, aside from the necessary COM checks, I don't know if there's a way this could be improved (in terms of clarity, that is),
	// so other than the container change, I'll leave it at that
    static bool GetThirdPartyAVSoftwareList(std::unordered_map<std::wstring, ThirdPartyAVSoftware>& thirdPartyAVSoftwareMap)
    {
		// This line runs only once, and it calls the above object's constructor to initialize COM, and when the program ends, it gets uninitialized
		static COMInitializer comInit;

		HRESULT hr = S_OK;
        // I've used smart ComPtr's, which handles management of COM objects
        Microsoft::WRL::ComPtr<IWSCProductList> PtrProductList;
		BSTR PtrVal = nullptr;
		LONG ProductCount = 0;
		WSC_SECURITY_PRODUCT_STATE ProductState;
		WSC_SECURITY_SIGNATURE_STATUS ProductStatus;

		std::wstring displayName, state, timestamp;
		std::string definitionState;

		// I thought about creating 2 macros, CHECK_RETURN and CHECK_CONTINUE to reduce the amount of repetitive code used in every
		// COM call, but since not everyone is all that keen on using macros, I dropped that idea
		hr = CoCreateInstance(__uuidof(WSCProductList), nullptr, CLSCTX::CLSCTX_INPROC_SERVER, __uuidof(IWSCProductList), &PtrProductList);
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

		for (int32_t i = 0; i < ProductCount; i++)
		{
            // Same as with the product list interface, but inside the scope of this loop so that,
            // it always resets automatically
            Microsoft::WRL::ComPtr<IWscProduct> PtrProduct;
			hr = PtrProductList->get_Item(i, &PtrProduct);
			if (FAILED(hr))
			{
				std::cout << "Failed to query AV product.";
				continue;
			}

			hr = PtrProduct->get_ProductName(&PtrVal);
			if (FAILED(hr))
			{
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

			if (ProductState == WSC_SECURITY_PRODUCT_STATE::WSC_SECURITY_PRODUCT_STATE_ON)
			{
				state = L"On";
			}
			else if (ProductState == WSC_SECURITY_PRODUCT_STATE::WSC_SECURITY_PRODUCT_STATE_OFF)
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

			definitionState = (ProductStatus == WSC_SECURITY_SIGNATURE_STATUS::WSC_SECURITY_PRODUCT_UP_TO_DATE) ? "UpToDate" : "OutOfDate";

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
			thirdPartyAVSoftware.Description = state; // state being assigned to Description and ProductState? I guess this was intended
			thirdPartyAVSoftware.ProductState = state;
			thirdPartyAVSoftwareMap[thirdPartyAVSoftware.Name] = thirdPartyAVSoftware;
		}

		if (thirdPartyAVSoftwareMap.size() == 0)
		{
			return false;
		}
		return true;
    }
};


int main()
{
	std::unordered_map<std::wstring, ThirdPartyAVSoftware> thirdPartyAVSoftwareMap;
	auto result = AVProductInformation::GetThirdPartyAVSoftwareList(thirdPartyAVSoftwareMap);
	std::cout << (result ? "Call succeeded" : "Call failed");

	return 0;
}