#pragma once
/* Includes */
#include "Command_Thread.h"
#include "RunProcess.h"


DWORD WINAPI CommandThread(LPVOID lpParam) {
	COMMAND_THREAD_params_t* p_params;

	/* Check if lpParam is NULL */
	if (NULL == lpParam) {
		printf("Did not recieve thread params\n");
		return -1;
	}

	p_params = (COMMAND_THREAD_params_t *)lpParam;
	RunProcessAndTestProgramForResults(p_params);
	return p_params->ReturnCode;
}

void RunProcessAndTestProgramForResults(COMMAND_THREAD_params_t* p_params) {
	p_params->ReturnCode = CreateProcessSimpleMain(p_params->Command);
	p_params->isCrashed = false;
	int realReturnCode = (int)p_params->ReturnCode;
	if (realReturnCode != 0) {
		//not success - need to check if (crash -1, timeout -3)
		printf("for command %s the return code is: %d\n", p_params->Command, p_params->ReturnCode);
		if (realReturnCode != -3) {
			p_params->isCrashed = true;
		}
		return;
	}
	//check if success and return it else return failed (-2)
	p_params->ReturnCode = CompareResultsFiles(p_params->ExpectedResultPath, p_params->ResultsPath);
}
