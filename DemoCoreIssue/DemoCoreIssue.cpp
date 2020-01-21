#include "pch.h"

#include "DemoCoreIssue.h"
#include "..\Library\Library.h"

namespace DemoCoreIssue
{
    int Class1::Value::get()
    {
        return fnLibrary();
    }

}

