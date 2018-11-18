#pragma once

// IO_Thread.h

#ifndef __COMMAND_THREAD_H__
#define __COMMAND_THREAD_H__

/* Includes */
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <tchar.h> 
#include <stdbool.h>
#include "RunProcess.h"
#include "Extensions.h"

// Function Declarations -------------------------------------------------------

/*
* Math thread. This thread adds up to numbers.
*/
DWORD WINAPI CommandThread(LPVOID lpParam);

/*
this function creates a process to run from params checks the returnCode and if its ok compares results with
the expected result file, the function also writes to a result file given in params

the function also writes data into params - ExitCode
Parameters:
params - the struct contatining the command to run,
the path to the result file to write to and the path to the expected result file
*/
void RunProcessAndTestProgramForResults(COMMAND_THREAD_params_t* params);

#endif // !__COMMAND_THREAD_H__