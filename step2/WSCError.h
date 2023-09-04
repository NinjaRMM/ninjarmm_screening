#pragma once

#include <exception>
#include <string>

class WSCError : public std::exception
{
private:
    std::string message;

public:
    inline static const std::string LIST_CREATION_ERRROR = "Failed to create WSCProductList object.";
    inline static const std::string AV_PRODUCT_LIST_QUERY_ERROR = "Failed to query antivirus product list.";
    inline static const std::string PRODUCT_COUNT_QUERY_ERROR = "Failed to query product count.";

    inline static const std::string AV_PRODUCT_QUERY_ERROR = "Failed to query AV product.";
    inline static const std::string AV_PRODUCT_QUERY_NAME_ERROR = "Failed to query AV product name.";
    inline static const std::string AV_PRODUCT_QUERY_STATE_ERROR = "Failed to query AV product state.";
    inline static const std::string AV_PRODUCT_QUERY_DEFINITION_STATE_ERROR = "Failed to query AV product definition state.";


    WSCError(std::string_view _message) : message(_message) {};

    const char *what() const noexcept override
    {
        return message.c_str();
    }
};

class WSCQueryError : public WSCError
{
public:
    WSCQueryError(std::string_view _message) : WSCError(_message) {};
};

class WSCListError : public WSCError
{
public:
    WSCListError(std::string_view _message) : WSCError(_message) {};
};