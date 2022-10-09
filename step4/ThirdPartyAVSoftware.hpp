#pragma once 

#include <string>
#include <iostream>

struct ThirdPartyAVSoftware {
    //To facilitate future changes to ThirdPartyAVSoftware types
    using Name = std::wstring;
    using Description = std::wstring;
    using DefinitionUpdateTime = std::wstring;
    using DefinitionStatus = std::string;
    using Version = std::wstring;
    using ProductState = std::wstring;

    Name name;
    // This field is not being used, it is missing to implement the parser for this field.
    Description description;
    DefinitionUpdateTime definitionUpdateTime;
    DefinitionStatus definitionStatus;
    // This field is not being used, it is missing to implement the parser for this field.
    Version version;
    ProductState productState;
};
