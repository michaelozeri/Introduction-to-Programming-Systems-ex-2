/* Includes */
#include "Command_Thread.h"


static void TestProgramForResults(COMMAND_THREAD_params_t* params);

DWORD WINAPI CommandThread(LPVOID lpParam)
{
	COMMAND_THREAD_params_t *p_params;

	/* Check if lpParam is NULL */
	if (NULL == lpParam)
	{
		printf("Did not recieve thread params\n");
		return -1;
	}

	p_params = (COMMAND_THREAD_params_t *)lpParam;
	TestProgramForResults(p_params);
	return p_params->ReturnCode;
}

static void TestProgramForResults(COMMAND_THREAD_params_t* params)
{
	params->ReturnCode = CreateProcessSimpleMain(params->Command);
	if (params->ReturnCode != 0)
	{
		return;
	}

	params->ReturnCode = CompareResultsFiles(params->ExpectedResultPath, params->ResultsPath);
}
