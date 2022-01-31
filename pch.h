// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
struct Node;
uintptr_t  FindDMAAddy(uintptr_t  BaseAddress, std::vector<unsigned int> Offsets);
Node* GenerateBazaar(uintptr_t base,UINT16 PageQuantity);
#endif //PCH_H
