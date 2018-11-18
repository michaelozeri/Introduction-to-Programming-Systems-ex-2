
/* Includes */
#include "Extensions.h"
#include "RunProcess.h"

/* Implementation */
DWORD CreateProcessSimpleMain(char* runCommand) {
	DWORD exitCode;
	Process* process = (Process*)malloc(sizeof(Process));
	if (process == NULL) {
		printf("Memory allocation failed.\n");
		return -1;
	}

	int len = strlen(runCommand);
	process->ExitCode = (DWORD*)malloc(sizeof(DWORD));
	if (process->ExitCode == NULL) {
		printf("Memory allocation failed.\n");
		FreeProcessObject(process);
		return -1;
	}

	process->ProcessInformation = (PPROCESS_INFORMATION*)malloc(sizeof(PROCESS_INFORMATION));
	if (process->ProcessInformation == NULL) {
		printf("Memory allocation failed.\n");
		FreeProcessObject(process);
		return -1;
	}

	int res = strcpy_s(process->TCommand, 100, runCommand);
	/*int res = swprintf(process->TCommand, 100, L"%hs", runCommand);*/
	if (res) {
		printf("Error parsing command\n");
		FreeProcessObject(process);
		return -1;
	}
	printf("Finished parsing command\n");
	process->Command = process->TCommand;
	printf("process-> command before creating process is: %s\n", process->Command);
	process->ReturnValue = CreateProcessSimple(process);

	if (process->ReturnValue == false) {
		printf("Process Creation Failed!\n");
		FreeProcessObject(process);
		return -1;
	}

	process->WaitCode = WaitForSingleObject(
		process->ProcessInformation->hProcess,
		TIMEOUT_IN_MILLISECONDS); /* Waiting 10 secs for the process to end */

	if (process->WaitCode == WAIT_TIMEOUT) {
		TerminateProcess(
			process->ProcessInformation->hProcess,
			BRUTAL_TERMINATION_CODE);
		Sleep(10);
		*process->ExitCode = (DWORD)-3;
	}
	else {
		GetExitCodeProcess(process->ProcessInformation->hProcess, process->ExitCode);
	}

	if (CloseHandle(process->ProcessInformation->hProcess) == false) {
		printf("Closing handle to process failed");
		*process->ExitCode = (DWORD)-1;
	}

	if (CloseHandle(process->ProcessInformation->hThread) == false) {
		printf("Closing handle to process failed");
		process->ExitCode = (DWORD)-1;
	}

	exitCode = *(process->ExitCode);
	FreeProcessObject(process);
	return exitCode;
}


BOOL CreateProcessSimple(Process* process) {
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 };
	PROCESS_INFORMATION* procInfo = process->ProcessInformation;
	return CreateProcess(NULL,
		process->Command,
		NULL,
		NULL,
		FALSE,
		NORMAL_PRIORITY_CLASS,
		NULL,
		NULL,
		&startinfo,
		process->ProcessInformation
	);
}