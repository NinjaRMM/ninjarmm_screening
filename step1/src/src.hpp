// Example program
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <memory>

class Job {
    private:
    std::string _name;
    std::string _description;
    int _hoursRequired;

    public:
    explicit Job( std::string name, std::string description, int hoursRequired) :
        _name(std::move(name)), _description(std::move(description)), _hoursRequired(std::move(hoursRequired)) {}

    virtual ~Job() = default;

    std::string name() const noexcept { return _name; }
    std::string description() const noexcept { return _description; }
    int hoursRequired() const noexcept { return _hoursRequired; }
    std::string doWork() const noexcept { return "My work involves " + _description; }
};

class Programmer: public Job {
    public:
    Programmer( std::string name, std::string description, int hoursRequired) :
        Job(name, description, hoursRequired){}
};

class Pilot: public Job {
    public:
    Pilot( std::string name, std::string description, int hoursRequired) :
        Job(name, description, hoursRequired){}
};

template <typename T>
std::unique_ptr<Job> factory(std::string name, std::string description, int hoursRequired)  {
    if(name == "Programmer") {
        return std::make_unique<Programmer>(name, description, hoursRequired);
    }
    else if(name == "Pilot")
    {
        return std::make_unique<Pilot>(name, description, hoursRequired);
    }
    else {
        throw "Invalid name provided";
    }
}

template <typename UnaryFunction>
size_t ContainsTheString(UnaryFunction unaryFunction, const std::vector<std::string>& list)
{
    size_t i = 0;

    for(const auto& l : list) {
        if(unaryFunction(l)) {
           ++i;
        }
    }

    return i;
}

enum HttpResponse : uint32_t {
    INFORMATIONAL,
    SUCCESS,
    REDIRECT,
    CLIENT_ERROR,
    SERVER_ERROR,
};

template <HttpResponse H>
struct HttpResponseTraits;

template <>
struct HttpResponseTraits<HttpResponse::INFORMATIONAL> {
    static constexpr uint32_t min{100};
    static constexpr uint32_t max{199};
};
template <>
struct HttpResponseTraits<HttpResponse::SUCCESS> {
    static constexpr uint32_t min{200};
    static constexpr uint32_t max{299};
};

template <>
struct HttpResponseTraits<HttpResponse::REDIRECT> {
    static constexpr uint32_t min{300};
    static constexpr uint32_t max{399};
};

template <>
struct HttpResponseTraits<HttpResponse::CLIENT_ERROR> {
    static constexpr uint32_t min{400};
    static constexpr uint32_t max{499};
};

template <>
struct HttpResponseTraits<HttpResponse::SERVER_ERROR> {
    static constexpr uint32_t min{500};
    static constexpr uint32_t max{599};
};

template<HttpResponse H, typename HT = HttpResponseTraits<H>>
bool isHttpResponseInBounds(const uint32_t first, const uint32_t second) {
    return (first >= HT::min && second <= HT::max);
}

template <typename T>
bool IsInBounds(T httpResponse, const uint32_t first, const uint32_t second)
{
    auto conditional = false;

    switch(httpResponse) {
        case HttpResponse::INFORMATIONAL:
            conditional = isHttpResponseInBounds<HttpResponse::INFORMATIONAL>(first, second);
        break;
        case HttpResponse::SUCCESS:
            conditional = isHttpResponseInBounds<HttpResponse::SUCCESS>(first, second);
        break;
        case HttpResponse::REDIRECT:
            conditional = isHttpResponseInBounds<HttpResponse::REDIRECT>(first, second);
        break;
        case HttpResponse::CLIENT_ERROR:
            conditional = isHttpResponseInBounds<HttpResponse::CLIENT_ERROR>(first, second);
        break;
        case HttpResponse::SERVER_ERROR:
            conditional = isHttpResponseInBounds<HttpResponse::SERVER_ERROR>(first, second);
        break;
        default:
            throw "Invalid http response";
        break;
    }

    return conditional;
}