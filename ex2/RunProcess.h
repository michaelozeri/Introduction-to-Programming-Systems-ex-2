#ifndef RUN_PROCESS
#define RUN_PROCESS

/* Includes */
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <tchar.h> 
#include <stdbool.h>
#include "Extensions.h"

/* Definitions */
#define TIMEOUT_IN_MILLISECONDS 10000
#define BRUTAL_TERMINATION_CODE 0x55


/* Functions Declerations */
/* CreateProcessSimpleMain will create a new process, that will run a specific file, and will track it's success.
* Arguments:
*		runCommand - the complete command to run, including all arguments
* Returns:
*		0 for success, -1 for crash / permeneant error, or process return code otherwise.
*/
DWORD CreateProcessSimpleMain(char* runCommand);

/* CreateProcessSimple will create a new process out of the Process struct
* Arguments:
*		process - Process struct - will contain all information required to create new process.
* Returns:
*		true if process was created, false otherwise
*/
BOOL CreateProcessSimple(Process* process);

#endif // !RUN_PROCESS


