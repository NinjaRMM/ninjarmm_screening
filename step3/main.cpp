#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <optional>
#include <limits>
#include <array>
using namespace std::literals;

/*
    Using structs to avoid using public so many times :)
*/

struct Job {
    virtual ~Job(){}
    virtual const char * getJobName() const = 0;
    virtual const char * getJobActivities() const = 0;
    virtual int getJobLength() const {return 8;}

    /*
        I would recommend not printing from inside the class as it entangles the class with the IO
        I recommend doing instead:
            std::string print() {return "My work involves"s + getJobActivities();}
        And printing somewhere else
    */
   void DoWork() const {std::cout <<  "My work involves"s <<  getJobActivities() << std::endl;}

};
struct Programmer : Job {
    const char * getJobName() const  override {return "Programmer";};
    const char * getJobActivities() const override {return "Programming";};

};
struct Pilot : Job {
    const char * getJobName() const override {return "Pilot";};
    const char * getJobActivities() const override {return "Piloting";};
    int getJobLength() const override {return 16;};
};

//Assuming range [low, high)
template<class T>
bool IsInBounds(const T& httpResonseCode, const T& low, const T& high) {
    bool inBounds = low <= httpResonseCode && httpResonseCode < high; 
    std::cout << "Response"  << httpResonseCode 
        << (inBounds ? "is inside of range [" : "is ouside of range [" )
        << low << ", " << high << ")";
    return inBounds;
}


// Chose template to allow statefull lambdas could as easily accepted a bool(* testFn)(const std::string&)
template<class Condition>
int ContainsTheString(Condition testFn, const std::vector<std::string>& vec ) {
    return std::count_if(begin(vec), end(vec), testFn);
}


constexpr int flexing();



int main(int argc, char** argv) {
    std::vector<std::unique_ptr<Job>> Jobs;
    Jobs.emplace_back(std::make_unique<Programmer>());
    Jobs.emplace_back(std::make_unique<Pilot>());

    for(const auto& job : Jobs) {
        std::cout << "I work as a "  << job->getJobName() 
            << ", I work for " << job->getJobLength() << "hours" << std::endl;
        job->DoWork();
    }

    uint32_t httpResonseCode = 400;
    IsInBounds<uint32_t>(httpResonseCode, 500, 599);

    auto theStrings = std::vector<std::string> { "one", "two",  "test"};
    auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);

    //The flex is this parser will optimize to 0 lines of code most of the time if imputs are compiletime known :)
    // https://godbolt.org/z/P1ePx63jP
    // Created this example to teach how to generate efficient assembly using templated code
    return flexing();
}



template<size_t N>
struct StringLiteral {
    constexpr StringLiteral(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }
    
    std::array<char, N> value;
    static constexpr int charCount = N - 1;
};

template<auto ... list> struct ValList{};
namespace impl {    
    template<class Chars, class Ints> 
    struct ImplIntDictParse {};

    template<char ... chars, int... ints>
    struct ImplIntDictParse <ValList<chars...>, ValList<ints...>>{
        static_assert(sizeof...(chars) == sizeof...(ints));
        static constexpr std::array<char, sizeof...(chars)> achars {chars...};
        static constexpr std::array<int, sizeof...(ints)>   aints {ints...};
        
        static constexpr auto SortAsTuple() {
            std::array<std::pair<char, int>, sizeof...(chars)> tuples;

            for(int i = 0; i < sizeof...(chars); i++) {
                tuples[i] = std::pair{achars[i], aints[i]};
            }
            return tuples;
        }

        static constexpr auto getValue(char i) {
            std::pair<char, int> sel = {i, 0};
            auto it = std::lower_bound(vals.begin(), vals.end(), 
                sel, [](auto& a, auto& b){ return a.first < b.first;}
            );
            if (it == vals.end() || it->first != i) return -1;
            return it->second; 
        }

        static constexpr auto vals = SortAsTuple();

        static constexpr int size = sizeof...(ints);

    };
}

struct BinSymbols {
    static constexpr int size = 2;
    static constexpr auto getValue(char i) {
        if('0' <= i && i <='1') return i - '0';
        return -1;
    }
};

struct OctSymbols {
    static constexpr int size = 8;
    static constexpr auto getValue(char i) {
        if('0' <= i && i <='7') return i - '0';
        return -1;
    }
};

struct DecSymbols {
    static constexpr int size = 10;
    static constexpr auto getValue(char i) {
        if('0' <= i && i <='9') return i - '0';
        return -1;
    }
};

struct HexSymbols {
    static constexpr int size = 16;
    static constexpr auto getValue(char i) {
        if('0' <= i && i <='9' ) return i - '0';
        if('A' <= i && i <='F' ) return i - 'A' + 10;
        if('a' <= i && i <='f' ) return i - 'a' + 10;
        return -1;
    }  
};


template<class T, class Alpha> 
constexpr std::optional<T> TryUIntDictParse(std::string_view sv) {
    constexpr auto limit_oneprev = std::numeric_limits<T>::max() / Alpha::size;
    T res;
    for(auto ch : sv) {
        if(limit_oneprev < res ) return std::nullopt;
        auto v = Alpha::getValue(ch);
        if(v == -1) return std::nullopt;
        res *= Alpha::size;
        res += v;
    }
    return res;
}



constexpr int flexing(){
    return 
        //Creating a unique mapping for ints where a = 0 and b = 1
        *TryUIntDictParse<int, impl::ImplIntDictParse<ValList<'a', 'b'>, ValList<0, 1>>>("baaba") + 
        *TryUIntDictParse<int, DecSymbols>("12") + *TryUIntDictParse<int, HexSymbols>("F7");
}