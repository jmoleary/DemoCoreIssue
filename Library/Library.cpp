// Library.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "Library.h"


// This is an example of an exported variable
LIBRARY_API int nLibrary=0;

// This is an example of an exported function.
LIBRARY_API int fnLibrary(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CLibrary::CLibrary()
{
    return;
}
