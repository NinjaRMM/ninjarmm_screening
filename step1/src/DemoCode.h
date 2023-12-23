
#ifndef DEMOCODE_H_
#define DEMOCODE_H_

/**
 * Sample code demonstrating usage of std::unique_ptr with custom cleanup function
 * for freeing up data structures that does not use 'delete' to free memory.
 */
void demoScopedPointerCustom();


/**
 * Variadic template function addValues
 * Variadic template functions provide following benefits.
 * 1. Being a template, it is type agnostic.
 * 2. It can accept variable number of arguments.
 */
template<typename T>
T addValues(const T&a)
{
	return a;
}

template<typename T, typename ...Types>
T addValues(const T&a, Types...b)
{
	return addValues(a) + addValues(b...);
}

#endif /* DEMOCODE_H_ */
