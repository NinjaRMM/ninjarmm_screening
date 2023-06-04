#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "av_software_data_querier.h"

/** I am not sure if it is the correct nomenclature pattern as I can't test this software
because I am developing it in a Linux machine. But the intention is to mock the windows api
calls.
*/
class MockWSCProductList : public IWSCProductList
{
public:
    MOCK_METHOD(HRESULT, Initialize, (WSC_SECURITY_PROVIDER), (override));
    MOCK_METHOD(HRESULT, get_Count, (LONG*), (override));
    MOCK_METHOD(HRESULT, get_Item, (LONG, IWscProduct**), (override));
};

class MockWscProduct : public IWscProduct
{
public:
    MOCK_METHOD(HRESULT, get_ProductName, (BSTR*), (override));
    MOCK_METHOD(HRESULT, get_ProductState, (WSC_SECURITY_PRODUCT_STATE*), (override));
    MOCK_METHOD(HRESULT, get_SignatureStatus, (WSC_SECURITY_SIGNATURE_STATUS*), (override));
    MOCK_METHOD(HRESULT, get_ProductStateTimestamp, (BSTR*), (override));
};

TEST(AVSoftwareDataQuerierTest, QueryAVSoftwareData)
{
    MockWSCProductList mockProductList;
    MockWscProduct mockProduct;

    EXPECT_CALL(mockProductList, Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS)).WillOnce(testing::Return(S_OK));
    EXPECT_CALL(mockProductList, get_Count(testing::_)).WillOnce(testing::DoAll(testing::SetArgPointee<0>(2),
        testing::Return(S_OK)));
    EXPECT_CALL(mockProductList, get_Item(0, testing::_)).WillOnce(testing::DoAll(testing::SetArgPointee<1>
        (&mockProduct), testing::Return(S_OK)));
    EXPECT_CALL(mockProductList, get_Item(1, testing::_)).WillOnce(testing::DoAll(testing::SetArgPointee<1>
        (&mockProduct), testing::Return(S_OK)));
    EXPECT_CALL(mockProduct, get_ProductName(_)).WillOnce(::testing::DoAll(::testing::SetArgPointee<0>
        (::SysAllocString(L"Mock Antivirus")), ::testing::Return(S_OK)));
    EXPECT_CALL(mockProduct, get_ProductState(_)).WillOnce(::testing::DoAll(::testing::SetArgPointee<0>
        (WSC_SECURITY_PRODUCT_STATE_ON), ::testing::Return(S_OK)));
    EXPECT_CALL(mockProduct, get_SignatureStatus(_)).WillOnce(::testing::DoAll(::testing::SetArgPointee<0>
        (WSC_SECURITY_PRODUCT_UP_TO_DATE), ::testing::Return(S_OK)));
    EXPECT_CALL(mockProduct, get_ProductStateTimestamp(_)).WillOnce(::testing::DoAll(::testing::SetArgPointee<0>
        (::SysAllocString(L"2023-06-04 11:52:00")), ::testing::Return(S_OK)));

    AVSoftwareDataQuerier querier;

    ASSERT_NO_THROW(querier.queryAVSoftwareData());

    const std::map<std::string, std::shared_ptr<AVSoftwareData>>& dataMap = querier.getAVSoftwareDataMap();

    ASSERT_EQ(dataMap.size(), 2);

    auto it = dataMap.find("Mock Antivirus");
    ASSERT_NE(it, dataMap.end());
    ASSERT_EQ(it->second->getName(), "Mock Antivirus");
    ASSERT_EQ(it->second->getProductState(), "On");
    ASSERT_EQ(it->second->getDefinitionStatus(), "UpToDate");
    ASSERT_EQ(it->second->getDefinitionUpdateTime(), "2023-06-04 11:52:00");
}

TEST(AVSoftwareDataQuerierTest, QueryAVSoftwareDataInitializeFailed)
{
    AVSoftwareDataQuerier querier;
    MockWSCProductList mockProductList;

    EXPECT_CALL(mockProductList, Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS)).WillOnce(testing::Return(E_FAIL));

    ASSERT_THROW(querier.queryAVSoftwareData(), std::runtime_error);
}

TEST(AVSoftwareDataQuerierTest, QueryAVSoftwareDataGetCountFailed)
{
    AVSoftwareDataQuerier querier;
    MockWSCProductList mockProductList;

    EXPECT_CALL(mockProductList, Initialize(WSC_SECURITY_PROVIDER_ANTIVIRUS)).WillOnce(testing::Return(S_OK));
    EXPECT_CALL(mockProductList, get_Count(testing::_)).WillOnce(testing::Return(E_FAIL));

    ASSERT_THROW(querier.queryAVSoftwareData(), std::runtime_error);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
