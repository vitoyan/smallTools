// createFilesCaseSensitive.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"

#include <string>
#include <iostream>

#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>



bool testCreateFile(const std::wstring& file)
{
    HANDLE hFile = CreateFile(file.c_str(),                // name of the write
        GENERIC_WRITE,          // open for writing
        0,                      // do not share
        NULL,                   // default security
        CREATE_NEW,             // create new file only
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_POSIX_SEMANTICS,  // normal file, posix SEMANTICS
        NULL);                  // no attr. template

    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    HCRYPTPROV   hCryptProv;
    BYTE         pbData[RAND_MAX];
    DWORD size = rand();
    DWORD wsize = 0;

    if(CryptAcquireContext(    
           &hCryptProv,
           NULL,
           NULL,
           PROV_RSA_FULL,
           0)
       &&
       CryptGenRandom(
           hCryptProv, 
           size, 
           pbData)) 
    {
         if(!WriteFile( 
                    hFile,           // open file handle
                    pbData,      // start of data to write
                    size,  // number of bytes to write
                    &wsize, // number of bytes that were written
                    NULL))
         {
               std::cout<<"error during writing file"<<std::endl;
         }
    }
    else
    {
         std::cout<<"Error during CryptGenRandom"<<std::endl;
    }

    CloseHandle(hFile);

    return true;
}


int _tmain(int argc, _TCHAR* argv[])
{
    if(argc < 2)
    {
        std::cout<<"pls input file path"<<std::endl;
        std::cout<<"exmaple:  C:\\a.txt C:\\A.txt"<<std::endl;
        return 0;
    }

    for(int i = 1; i < argc; i++)
    {
        std::wstring path(argv[i]);
        if(testCreateFile(path))
            std::wcout<<L"we created "<<path<<L" succeed"<<std::endl;
        else
        {
            std::wcout<<L"we create "<<path<<L" failed"<<std::endl;             
            //Get the error message, if any.
            DWORD errorMessageID = ::GetLastError();
            std::cout<<"the error code is "<<errorMessageID<<std::endl;
            if(errorMessageID == 0)
                continue;

            LPSTR messageBuffer = nullptr;
            size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                         NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

            std::string message(messageBuffer, size);
            std::cout<<"the reason is "<<message<<std::endl;

            //Free the buffer.
            LocalFree(messageBuffer);
        }
    }

	return 0;
}

