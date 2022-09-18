#include <cstdint>
#include <cstdio>
#include <string>

//--------------------------------------
struct Thing {
    Thing(const char *v)         { str = v; printf("L-value (const char *)\n"); }
    Thing(std::string &v)        { str = v; printf("L-value\n"); }
    Thing(const std::string &v)  { str = v; printf("const L-value\n"); }
    Thing(std::string &&v)       { str = v; printf("R-Value\n"); }
    Thing(const std::string &&v) { str = v; printf("const R-Value\n"); }

    std::string str{};
};

//--------------------------------------
template <typename R, typename T>
R createPerfectForwarding(T &&str){
    return R(std::forward<T>(str));   // maintain cv-qualifiers
}

//--------------------------------------
template <typename R, typename T>
R create(const T &str){
    return R(str);
}

//--------------------------------------
template <typename R, typename T>
R create(T &&str){
    return R(str);
}

//--------------------------------------
int
main() {
    std::string str1("Hello");
    const std::string str2{"World"};

    createPerfectForwarding<Thing>("Hey!");             // L-Value (const char *)
    createPerfectForwarding<Thing>(str1);               // L-Value
    createPerfectForwarding<Thing>(str2);               // const L-Value
    createPerfectForwarding<Thing>(std::move(str1));    // convert L-Value to R-Value
    createPerfectForwarding<Thing>(std::move(str2));    // convert const L-Value to const R-Value (if it makes sense)
    createPerfectForwarding<Thing>(str1 + " " + str2);  // R-Value

    printf("--\n");

    create<Thing>("Hey!");                              // L-Value (const char *)
    create<Thing>(str1);                                // L-Value
    create<Thing>(str2);                                // const L-Value
    create<Thing>(std::move(str1));                     // L-Value
    create<Thing>(std::move(str2));                     // const L-Value
    create<Thing>(str1 + " " + str2);                   // L-Value

    return 0;
}