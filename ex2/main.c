/* Main - Project 2
* Authors:
*			Omer Machluf, 200892917
*			Michael Ozeri,

*/
/* Includes */
#include <stdio.h>
#include <windows.h>
#include "Command_Thread.h"
#include "Extensions.h"

int CreateAllThreads(Thread** allThreads, int numberOfThreads, char* pathToResultsFile);
int PrintResults(Thread** allThreads, int numberOfThreads, char* pathToResultsFile);

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("Incorrect number of program arguments\n");
		return -1;
	}

	return HandleAllThreads(argv[1], argv[2]);
}

int HandleAllThreads(char* pathToTestfile, char* pathToResultsFile){
	ResultFile* testFile = ReadFileContents(pathToTestfile);
	Thread** allThreads = (Thread**)malloc(sizeof(Thread*));
	if (allThreads == NULL)
	{
		printf("Memory allocation failed.\n");
		FreeResultsObject(testFile);
		return -1;
	}

	for (int i = 0; i < testFile->NumberOfElements; i++)
	{
		allThreads[i] = GetThreadFromLine(testFile->Results[i]);
		if (allThreads[i] == NULL)
		{
			FreeResultsObject(testFile);
			FreeThreadArray(allThreads, i);
			return -1;
		}

		allThreads[i]->Function = CommandThread;
	}
	
	return CreateAllThreads(allThreads, testFile->NumberOfElements, pathToResultsFile);
}

int CreateAllThreads(Thread** allThreads, int numberOfThreads, char* pathToResultsFile){
	for (int i = 0; i < numberOfThreads; i++)
	{
		//system call
		allThreads[i]->Handle = CreateThread(
			NULL,
			0,
			allThreads[i]->Function,
			allThreads[i]->p_thread_params,
			0,
			allThreads[i]->Id);

		if (allThreads[i]->Handle == NULL)
		{
			printf("Error when creating thread\n");
			FreeThreadArray(allThreads, i);
			return -1;
		}
	}

	/* Wait */
	for (int i = 0; i < numberOfThreads; i++)
	{
		//system call
		allThreads[i]->WaitCode = WaitForSingleObject(allThreads[i]->Handle, INFINITE);
		if (allThreads[i]->WaitCode != WAIT_OBJECT_0)
		{
			printf("Error when waiting\n");
			FreeThreadArray(allThreads, numberOfThreads);
			return -1;
		}
	}

	DWORD exit_code = 1;
	/* Get Exit Code */
	char** results = (char**)malloc(sizeof(char*) * numberOfThreads);
	for (int i = 0; i < numberOfThreads; i++)
	{
		if (GetExitCodeThread(allThreads[i]->Handle, &(allThreads[i]->ExitCode)) == 0)
		{
			printf("Error when getting thread exit code\n");
			return -1;
		}

	}
	
	PrintResults(allThreads, numberOfThreads, pathToResultsFile);

	if (FreeThreadArray(allThreads, numberOfThreads) != 0)
	{
		printf("Error when closing thread handles");
		return -1;
	}
	return 0;
}

int PrintResults(Thread** allThreads, int numberOfThreads, char* pathToResultsFile){
	int i;
	FILE* file = NULL;
	int retVal = fopen_s(&file,pathToResultsFile, "w+");
	if (file == NULL){
		printf("Error opening file!\n");
		return -2;
	}
	for (i = 0; i < numberOfThreads; i++){
		char buffer[100];
		retVal = sprintf_s(buffer,100,"test #%d : %s", i+1, TranslateExitCode(allThreads[i]->ExitCode));
		if (retVal == 0){
			printf("Error");
			return -2;
		}
		fprintf(file, "%s\n", buffer);
	}
	fclose(file);

	return 0;
}
