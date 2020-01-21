# DemoCoreIssue
Demonstrates link bug with VS2019 .NET Core C++/CLI projects.  

This solution demonstrates what I believe is a bug in Visual Studio 16.4.3.  In short, If you create a .NET Core library with C++/CLI, that library will ignore any additional library paths you supply to it.  Instead it will only use the one LIBPATH for what it needs for .NET Core assemblies.

It consists of two projects.  It was created with Visual Studio 16.4.3

1. DemoCoreIssue - a C++/CLI .NET Core 3.1 library with one class "Class1"
2. Library - a Window C++ DLL exporting a few symbols

The "Library" project exports a function called "fnLibrary"

    LIBRARY_API int fnLibrary(void);
   
The DemoCoreIssue managed class tries to call this function in "Library" from a property ("Value")

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


Yet no matter how you set the LIBPATH ("Additional Dependencies) in DemoCoreIssue, the link will fail.  It will never find the link  library.  The reason for this is that when Visual Studio builds DemoCoreIssue, the actual link command it issue throws away all of the LIBPATH values you specified and replaces them with just a single one. 

You can see this by going to the Project Properties >> Link >> Command Line for "DemoCoreIssue".  Here is what it looks like on my box:

    /OUT:"C:\Users\jmole\source\repos\DemoCoreIssue\x64\Debug\DemoCoreIssue.dll" /MANIFEST /NXCOMPAT   
    /PDB:"C:\Users\jmole\source\repos\DemoCoreIssue\x64\Debug\DemoCoreIssue.pdb" /DYNAMICBASE "LIBRARY.LIB" /FIXED:NO /DEBUG:FULL /DLL 
    /MACHINE:X64 /INCREMENTAL /PGD:"C:\Users\jmole\source\repos\DemoCoreIssue\x64\Debug\DemoCoreIssue.pgd" 
    /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /ManifestFile:"x64\Debug\DemoCoreIssue.dll.intermediate.manifest" 
    /ERRORREPORT:PROMPT /NOLOGO /LIBPATH:"C:\Users\jmole\source\repos\DemoCoreIssue\x64\Debug" /ASSEMBLYDEBUG /TLBID:1 

Note the LIBPATH:

   /LIBPATH:"C:\Users\jmole\source\repos\DemoCoreIssue\x64\Debug" 

Now make sure that this setting:

    Tools >> Options >> Projects and Solutions >> Build and Run >> "MSBuild project build log file verbosity" 
    
is set to "Normal" and build the project.  Then take a look at the log file:   DemoCoreIssue\x64\Debug\DemoCoreIssue.log   Find the "Link" command .  Here is what it looks like on my box:

    C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.24.28314\bin\HostX86\x64\link.exe /ERRORREPORT:PROMPT 
    /OUT:"C:\Users\jmole\source\repos\DemoCoreIssue\x64\Debug\DemoCoreIssue.dll" /INCREMENTAL /NOLOGO /LIBPATH:"C:\Program 
    Files\dotnet\packs\Microsoft.NETCore.App.Host.win-x64\3.1.1\runtimes\win-x64\native" LIBRARY.LIB /MANIFEST 
    /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /manifest:embed /DEBUG:FULL 
    /PDB:"C:\Users\jmole\source\repos\DemoCoreIssue\x64\Debug\DemoCoreIssue.pdb" /ASSEMBLYDEBUG /TLBID:1 /DYNAMICBASE /FIXED:NO /NXCOMPAT 
    /MACHINE:X64 /DLL x64\Debug\app.res


Note that the LIBPATH I had has been simply replaced with this:

    /LIBPATH:"C:\Program 
    Files\dotnet\packs\Microsoft.NETCore.App.Host.win-x64\3.1.1\runtimes\win-x64\native" 



