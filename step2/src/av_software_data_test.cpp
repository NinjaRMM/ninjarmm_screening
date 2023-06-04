#include <gtest/gtest.h>

#include "av_software_data.h"

class AVSoftwareDataTest : public ::testing::Test
{
protected:
    AVSoftwareData avSoftwareData;
};

TEST_F(AVSoftwareDataTest, QueryAVSoftwareData)
{
    ASSERT_NO_THROW(avSoftwareData.queryAVSoftwareData());
}

TEST_F(AVSoftwareDataTest, GetAVSoftwareMap)
{
    avSoftwareData.queryAVSoftwareData();
    const std::map<std::string, ThirdPartyAVSoftware>& avSoftwareMap = avSoftwareData.getAVSoftwareMap();

    ASSERT_FALSE(avSoftwareMap.empty());
    ASSERT_EQ(2, avSoftwareMap.size()); //suposing the system has 2 anti viruses

    auto softwareIter = avSoftwareMap.find("AntiVirus"); //suposing the system has an antivirus called AntiVirus
    ASSERT_NE(avSoftwareMap.end(), softwareIter);
    ASSERT_EQ("Description", softwareIter->second.description); //suposing the AntiVirus has a description "Description"
    ASSERT_EQ("UpToDate", softwareIter->second.definitionStatus); //suposing the AntiVirus is updated
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
