#include "../ModifiedCodeReview.h"
#include <gmock/gmock.h>
#include <map>
#include <string>
#include <atlbase.h>
#include <iwscapi.h>
#include <wscapi.h>

using namespace testing;
using std::vector;
using std::wstring;

int main(int argc, wchar_t** argv) {
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}

class mock_IWSCProductList : public IWSCProductList {
public:

    //IUnknown
	MOCK_METHOD(HRESULT, QueryInterface, (
		 REFIID /*riid*/,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* /*ppvObject*/)
		, (Calltype(STDMETHODCALLTYPE), override));

    ULONG STDMETHODCALLTYPE AddRef(void) override {
        return ++m_ref;
    }

    ULONG STDMETHODCALLTYPE Release(void) override {
        if (0 == --m_ref) {
            delete this;
        }
        return m_ref;
    }

    //IDispatch
    MOCK_METHOD(HRESULT, GetTypeInfoCount, (
         __RPC__out UINT* pctinfo), (Calltype(STDMETHODCALLTYPE), override));

    //MOCK_METHOD(HRESULT, , , (Calltype(STDMETHODCALLTYPE), override));

    MOCK_METHOD(HRESULT, GetTypeInfo, (
        UINT iTInfo,
        LCID lcid,
        __RPC__deref_out_opt ITypeInfo** ppTInfo), (Calltype(STDMETHODCALLTYPE), override));

    MOCK_METHOD(HRESULT, GetIDsOfNames, (
        __RPC__in REFIID riid,
        __RPC__in_ecount_full(cNames) LPOLESTR* rgszNames,
        __RPC__in_range(0, 16384) UINT cNames,
        LCID lcid,
        __RPC__out_ecount_full(cNames) DISPID* rgDispId), (Calltype(STDMETHODCALLTYPE), override));

    MOCK_METHOD(HRESULT, Invoke, (
        _In_  DISPID dispIdMember,
        _In_  REFIID riid,
        _In_  LCID lcid,
        _In_  WORD wFlags,
        _In_  DISPPARAMS* pDispParams,
        _Out_opt_  VARIANT* pVarResult,
        _Out_opt_  EXCEPINFO* pExcepInfo,
        _Out_opt_  UINT* puArgErr), (Calltype(STDMETHODCALLTYPE), override));

    //IWSCProductList

    MOCK_METHOD(HRESULT, Initialize, (ULONG provider), (Calltype(STDMETHODCALLTYPE), override));
    MOCK_METHOD(HRESULT, get_Count, (__RPC__out LONG* pVal), (Calltype(STDMETHODCALLTYPE), override));
    MOCK_METHOD(HRESULT, get_Item, (ULONG index, __RPC__deref_out_opt IWscProduct** pVal), (Calltype(STDMETHODCALLTYPE), override));
    ~mock_IWSCProductList() {

    }
private:
	ULONG m_ref = 0;
};

CComPtr<IWSCProductList> g_product_list;

CComPtr<IWSCProductList> get_IWSCProductList()
{
    return g_product_list;

}

class QueryAVSoftware_Test : public Test{
public:
    void SetUp()override {
        g_product_list = CComPtr<IWSCProductList>(new mock_IWSCProductList);
    }

    mock_IWSCProductList& product_list() {
        mock_IWSCProductList* prod_list
            = dynamic_cast<mock_IWSCProductList*>(&*g_product_list);
        assert(prod_list);
        return *prod_list;
    }

    void TearDown() {
        g_product_list = nullptr;
    }
private:
    CComPtr<IWSCProductList> m_product_list;
};

bool operator==(const ThirdPartyAVSoftware& left, const ThirdPartyAVSoftware& right) {
    
    auto create_vector = [](const ThirdPartyAVSoftware& av_software) {
        return vector<wstring>{av_software.Name, av_software.Description
            , av_software.DefinitionUpdateTime, av_software.DefinitionStatus
            , av_software.Version, av_software.ProductState};
    };

    return create_vector(left) == create_vector(right);
}

TEST_F(QueryAVSoftware_Test, FailedProductListInitialize) {


    std::map<std::wstring, ThirdPartyAVSoftware> thirdPartyAVSoftwareMap_expected
        = { {L"TestAV", {L"TestAV", L"description", L"updatetime2023", L"UpToDate", L"version4", L"On"}} };

    std::map<std::wstring, ThirdPartyAVSoftware> thirdPartyAVSoftwareMap_result(thirdPartyAVSoftwareMap_expected);

    EXPECT_CALL(product_list(), Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS))
        .WillOnce(Return(E_INVALIDARG));

    EXPECT_FALSE(queryWindowsForAVSoftwareDataWSC_modified(thirdPartyAVSoftwareMap_result));
    EXPECT_EQ(thirdPartyAVSoftwareMap_expected, thirdPartyAVSoftwareMap_result);
}


class mock_IWSCProduct : public IWscProduct {
public:
    //IUnknown
    MOCK_METHOD(HRESULT, QueryInterface, (
        REFIID /*riid*/,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* /*ppvObject*/)
        , (Calltype(STDMETHODCALLTYPE), override));

    ULONG STDMETHODCALLTYPE AddRef(void) override {
        return ++m_ref;
    }

    ULONG STDMETHODCALLTYPE Release(void) override {
        if (0 == --m_ref) {
            delete this;
        }
        return m_ref;
    }

    //IDispatch
    MOCK_METHOD(HRESULT, GetTypeInfoCount, (
        __RPC__out UINT* pctinfo), (Calltype(STDMETHODCALLTYPE), override));

    //MOCK_METHOD(HRESULT, , , (Calltype(STDMETHODCALLTYPE), override));

    MOCK_METHOD(HRESULT, GetTypeInfo, (
        UINT iTInfo,
        LCID lcid,
        __RPC__deref_out_opt ITypeInfo** ppTInfo), (Calltype(STDMETHODCALLTYPE), override));

    MOCK_METHOD(HRESULT, GetIDsOfNames, (
        __RPC__in REFIID riid,
        __RPC__in_ecount_full(cNames) LPOLESTR* rgszNames,
        __RPC__in_range(0, 16384) UINT cNames,
        LCID lcid,
        __RPC__out_ecount_full(cNames) DISPID* rgDispId), (Calltype(STDMETHODCALLTYPE), override));

    MOCK_METHOD(HRESULT, Invoke, (
        _In_  DISPID dispIdMember,
        _In_  REFIID riid,
        _In_  LCID lcid,
        _In_  WORD wFlags,
        _In_  DISPPARAMS* pDispParams,
        _Out_opt_  VARIANT* pVarResult,
        _Out_opt_  EXCEPINFO* pExcepInfo,
        _Out_opt_  UINT* puArgErr), (Calltype(STDMETHODCALLTYPE), override));

    //MOCK_METHOD(HRESULT, , , (Calltype(STDMETHODCALLTYPE), override));
    MOCK_METHOD(HRESULT, get_ProductName, (BSTR* pVal), (Calltype(STDMETHODCALLTYPE), override));
    MOCK_METHOD(HRESULT, get_ProductState, (WSC_SECURITY_PRODUCT_STATE* pVal), (Calltype(STDMETHODCALLTYPE), override));
    MOCK_METHOD(HRESULT, get_SignatureStatus, (WSC_SECURITY_SIGNATURE_STATUS* pVal), (Calltype(STDMETHODCALLTYPE), override));
    MOCK_METHOD(HRESULT, get_RemediationPath, (BSTR* pVal), (Calltype(STDMETHODCALLTYPE), override));
    MOCK_METHOD(HRESULT, get_ProductStateTimestamp, (BSTR* pVal), (Calltype(STDMETHODCALLTYPE), override));
    MOCK_METHOD(HRESULT, get_ProductGuid, (BSTR* pVal), (Calltype(STDMETHODCALLTYPE), override));
    MOCK_METHOD(HRESULT, get_ProductIsDefault, (BOOL* pVal), (Calltype(STDMETHODCALLTYPE), override));

private:
    ULONG m_ref = 0;

};
//Basic success case
TEST_F(QueryAVSoftware_Test, ProductDataTest) {

    std::map<std::wstring, ThirdPartyAVSoftware> thirdPartyAVSoftwareMap_expected
        = { {L"TestAV", {L"TestAV", L"", L"Fri, 31 Mar 2023 09:22:22 GMT", L"UpToDate", L"", L"On"}} };

    ThirdPartyAVSoftware& expected_av_software = thirdPartyAVSoftwareMap_expected.begin()->second;

    

    EXPECT_CALL(product_list(), Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS))
        .WillOnce(Return(S_OK));

    EXPECT_CALL(product_list(), get_Count(IsTrue()))
        .WillOnce(DoAll(SetArgPointee<0>(1), Return(S_OK)));

    mock_IWSCProduct* product = new mock_IWSCProduct;
    product->AddRef();
    EXPECT_CALL(product_list(), get_Item(0, IsTrue()))
        .WillOnce(DoAll(SetArgPointee<1>(product), Return(S_OK)));

    EXPECT_CALL(*product, get_ProductName(IsTrue()))
        .WillOnce(DoAll(SetArgPointee<0>(SysAllocString(expected_av_software.Name.c_str())), Return(S_OK)));


    EXPECT_CALL(*product, get_ProductState(IsTrue()))
        .WillOnce(DoAll(SetArgPointee<0>(WSC_SECURITY_PRODUCT_STATE_ON), Return(S_OK)));
    EXPECT_CALL(*product, get_SignatureStatus(IsTrue()))
        .WillOnce(DoAll(SetArgPointee<0>(WSC_SECURITY_PRODUCT_UP_TO_DATE), Return(S_OK)));


    EXPECT_CALL(*product, get_ProductStateTimestamp(IsTrue()))
        .WillOnce(DoAll(SetArgPointee<0>(SysAllocString(expected_av_software.DefinitionUpdateTime.c_str())), Return(S_OK)));
    
    std::map<std::wstring, ThirdPartyAVSoftware> thirdPartyAVSoftwareMap_result;
    EXPECT_TRUE(queryWindowsForAVSoftwareDataWSC_modified(thirdPartyAVSoftwareMap_result));
    EXPECT_EQ(thirdPartyAVSoftwareMap_expected, thirdPartyAVSoftwareMap_result);
}

//This is to test the case querying IWSCProduct data fails
TEST_F(QueryAVSoftware_Test, ProductDataFaultTest) {


    std::map<std::wstring, ThirdPartyAVSoftware> thirdPartyAVSoftwareMap_data
        = { {L"TestAV", {L"TestAV", L"", L"Fri, 31 Mar 2023 09:22:22 GMT", L"UpToDate", L"", L"On"}} };

    ThirdPartyAVSoftware& expected_av_software = thirdPartyAVSoftwareMap_data.begin()->second;



    EXPECT_CALL(product_list(), Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS))
        .WillOnce(Return(S_OK));

    EXPECT_CALL(product_list(), get_Count(IsTrue()))
        .WillOnce(DoAll(SetArgPointee<0>(1), Return(S_OK)));

    mock_IWSCProduct* product = new mock_IWSCProduct;
    product->AddRef();
    EXPECT_CALL(product_list(), get_Item(0, IsTrue()))
        .WillOnce(DoAll(SetArgPointee<1>(product), Return(S_OK)));

    EXPECT_CALL(*product, get_ProductName(IsTrue()))
        .WillOnce(DoAll(SetArgPointee<0>(SysAllocString(expected_av_software.Name.c_str())), Return(S_OK)));


    EXPECT_CALL(*product, get_ProductState(IsTrue()))
        .WillOnce(DoAll(SetArgPointee<0>(WSC_SECURITY_PRODUCT_STATE_ON), Return(S_OK)));
    EXPECT_CALL(*product, get_SignatureStatus(IsTrue()))
        .WillOnce(DoAll(SetArgPointee<0>(WSC_SECURITY_PRODUCT_UP_TO_DATE), Return(S_OK)));

    //FAIL the productstatetimestattempt
    EXPECT_CALL(*product, get_ProductStateTimestamp(IsTrue()))
        .WillOnce(DoAll(Return(E_ACCESSDENIED)));

    std::map<std::wstring, ThirdPartyAVSoftware> thirdPartyAVSoftwareMap_expected;
    std::map<std::wstring, ThirdPartyAVSoftware> thirdPartyAVSoftwareMap_result;
    EXPECT_FALSE(queryWindowsForAVSoftwareDataWSC_modified(thirdPartyAVSoftwareMap_result));
    EXPECT_EQ(thirdPartyAVSoftwareMap_expected, thirdPartyAVSoftwareMap_result);
}

//TODO:
//test all the failure conditions for code coverage
//test for multiple IWSCProduct with some of them failing . (to ensure forloop continues)
