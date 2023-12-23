

#include "DemoCode.h"

#include <memory>

/**
 * Sample code demonstrating usage of std::unique_ptr with custom cleanup function
 * for freeing up data structures that does not use 'delete' to free memory.
 */
void demoScopedPointerCustom()
{
	// Using unique_ptr to automatically free unsigned char allocation using malloc.
	using scoped_ptr_unsignchrPtrMalloced = std::unique_ptr<unsigned char, decltype(&free)>;

	unsigned char *pOutData = NULL;
	unsigned int memSize = 1000;

	pOutData = (unsigned char*) malloc(memSize);

	// This will cause 'free' to be called on 'pOutData' when 'mySmartPtr' goes out of scope.
	scoped_ptr_unsignchrPtrMalloced mySmartPtr(pOutData, free);
}
