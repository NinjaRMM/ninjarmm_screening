#include <gtest/gtest.h>

#include "av_software_data.h"

/** I am not sure if it is the correct nomenclature pattern as I can't test this software
because I am developing it in a Linux machine. But the intention is to mock the windows api
calls.
*/
class MockWscProduct : public IWscProduct {
public:
    MOCK_METHOD(HRESULT, get_ProductName, (BSTR*), (override));
    MOCK_METHOD(HRESULT, get_ProductState, (WSC_SECURITY_PRODUCT_STATE*), (override));
    MOCK_METHOD(HRESULT, get_SignatureStatus, (WSC_SECURITY_SIGNATURE_STATUS*), (override));
    MOCK_METHOD(HRESULT, get_ProductStateTimestamp, (BSTR*), (override));
};

TEST(AVSoftwareDataTest, ConstructorValidProductSuccess)
{
    MockWscProduct mockProduct;
    /** The same as above. I am not sure if it is the correct nomenclature, but the intention is to
        set a default value to the mocked object output values.
    */
    EXPECT_CALL(mockProduct, get_ProductName(_)).WillOnce(::testing::DoAll(::testing::SetArgPointee<0>
        (::SysAllocString(L"Mock Antivirus")), ::testing::Return(S_OK)));
    EXPECT_CALL(mockProduct, get_ProductState(_)).WillOnce(::testing::DoAll(::testing::SetArgPointee<0>
        (WSC_SECURITY_PRODUCT_STATE_ON), ::testing::Return(S_OK)));
    EXPECT_CALL(mockProduct, get_SignatureStatus(_)).WillOnce(::testing::DoAll(::testing::SetArgPointee<0>
        (WSC_SECURITY_PRODUCT_UP_TO_DATE), ::testing::Return(S_OK)));
    EXPECT_CALL(mockProduct, get_ProductStateTimestamp(_)).WillOnce(::testing::DoAll(::testing::SetArgPointee<0>
        (::SysAllocString(L"2023-06-04 11:52:00")), ::testing::Return(S_OK)));

    AVSoftwareData avSoftwareData(&mockProduct);

    EXPECT_EQ(avSoftwareData.getName(), "Mock Antivirus");
    EXPECT_EQ(avSoftwareData.getProductState(), "On");
    EXPECT_EQ(avSoftwareData.getDefinitionStatus(), "UpToDate");
    EXPECT_EQ(avSoftwareData.getDefinitionUpdateTime(), "2023-06-04 11:52:00");
}

TEST(AVSoftwareDataTest, ConstructorFailedProductNameThrowsRuntimeError)
{
    MockWscProduct mockProduct;
    EXPECT_CALL(mockProduct, get_ProductName(_)).WillOnce(::testing::Return(E_FAIL));
    EXPECT_CALL(mockProduct, get_ProductState(_)).WillOnce(::testing::Return(S_OK));
    EXPECT_CALL(mockProduct, get_SignatureStatus(_)).WillOnce(::testing::Return(S_OK));
    EXPECT_CALL(mockProduct, get_ProductStateTimestamp(_)).WillOnce(::testing::Return(S_OK));

    EXPECT_THROW(AVSoftwareData avSoftwareData(&mockProduct), std::runtime_error);
}

TEST(AVSoftwareDataTest, ConstructorFailedProductStateThrowsRuntimeError)
{
    MockWscProduct mockProduct;
    EXPECT_CALL(mockProduct, get_ProductName(_)).WillOnce(::testing::Return(S_OK));
    EXPECT_CALL(mockProduct, get_ProductState(_)).WillOnce(::testing::Return(E_FAIL));
    EXPECT_CALL(mockProduct, get_SignatureStatus(_)).WillOnce(::testing::Return(S_OK));
    EXPECT_CALL(mockProduct, get_ProductStateTimestamp(_)).WillOnce(::testing::Return(S_OK));

    EXPECT_THROW(AVSoftwareData avSoftwareData(&mockProduct), std::runtime_error);
}

TEST(AVSoftwareDataTest, ConstructorFailedSignatureStatusThrowsRuntimeError)
{
    MockWscProduct mockProduct;
    EXPECT_CALL(mockProduct, get_ProductName(_)).WillOnce(::testing::Return(S_OK));
    EXPECT_CALL(mockProduct, get_ProductState(_)).WillOnce(::testing::Return(S_OK));
    EXPECT_CALL(mockProduct, get_SignatureStatus(_)).WillOnce(::testing::Return(E_FAIL));
    EXPECT_CALL(mockProduct, get_ProductStateTimestamp(_)).WillOnce(::testing::Return(S_OK));

    EXPECT_THROW(AVSoftwareData avSoftwareData(&mockProduct), std::runtime_error);
}

TEST(AVSoftwareDataTest, ConstructorFailedProductStateTimestampThrowsRuntimeError)
{
    MockWscProduct mockProduct;
    EXPECT_CALL(mockProduct, get_ProductName(_)).WillOnce(::testing::Return(S_OK));
    EXPECT_CALL(mockProduct, get_ProductState(_)).WillOnce(::testing::Return(S_OK));
    EXPECT_CALL(mockProduct, get_SignatureStatus(_)).WillOnce(::testing::Return(S_OK));
    EXPECT_CALL(mockProduct, get_ProductStateTimestamp(_)).WillOnce(::testing::Return(E_FAIL));

    EXPECT_THROW(AVSoftwareData avSoftwareData(&mockProduct), std::runtime_error);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
