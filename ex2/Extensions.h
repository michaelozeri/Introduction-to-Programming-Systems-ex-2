#pragma once

/* Includes */
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
/* Constants */

/* Struct Declerations */
/* ResultFile is a struct representing a read file
*	Members:
*			Results- an array of string representing the content of the file, where every line of the file is a member of the array.
*			NumberOfElements - the number of lines in the read file
*			TotalSize- The total size of the array
*/
typedef struct ResultFile {
	char **Results;
	int NumberOfElements;
	size_t TotalSize;
} ResultFile;

/* Process is a struct representing an opened process
*	Members:
*			ProcessInformation-represents the process runtime information
*			WaitCode - the code we got from waiting on the process
*			ExitCode- the exit code of the process
*			ReturnValue - the value returned by the last function done on the process
*			TCommand - an helper array to organize the command
*			Command - the command the process will run
*/
typedef struct Process {
	PROCESS_INFORMATION* ProcessInformation;
	DWORD WaitCode;
	DWORD* ExitCode;
	BOOL ReturnValue;
	TCHAR TCommand[100];
	LPTSTR Command;
} Process;

/* COMMAND_THREAD_params_t is a struct representing the params given to a command thread
*	Members:
*			Command- the command to run by the process of the thread
*			ResultsPath - the path to the output file created by the process
*			ExpectedResultPath- the path to the expected results file to compare with the actual output file created by the processs
*			ReturnCode - the code returned by the thread.
*/
typedef struct COMMAND_THREAD_params_t
{
	char* Command;
	char* ResultsPath;
	char* ExpectedResultPath;
	DWORD ReturnCode;
	bool isCrashed;
} COMMAND_THREAD_params_t;

/* Thread is a struct representing a new thread
*	Members:
*			Id - thread id
*			Handle - thread handle
*			WaitCode - the code we got from waiting on the thread
*			ExitCode- the exit code of the thread
*			ReturnValue - the value returned by the last function done on the thread
*			p_thread_params - the arguments for the thread
*/
typedef struct Thread {
	HANDLE Handle;
	DWORD* Id;
	DWORD WaitCode;
	DWORD ExitCode;
	BOOL ReturnValue;
	LPTHREAD_START_ROUTINE Function;
	COMMAND_THREAD_params_t *p_thread_params;
} Thread;

/* Functions Declerations */
/* FreeResultsObject will free all allocated resources in ResultFile Struct
* Arguments:
*		result - the result struct to free
* Returns:
*		void
*/
void FreeResultsObject(ResultFile* result);

/* FreeStringArray will free all allocated resources in a strings array
* Arguments:
*		arr - the array of strings to free
*		numOfMembers - the number of strings in the array
* Returns:
*		void
*/
void FreeStringArray(char** arr, int numOfMembers);

/* FreeProcessObject will free all allocated resources in a Process struct
* Arguments:
*		numOfMembers - the number of strings in the array
* Returns:
*		void
*/
void FreeProcessObject(Process* process);

/* CompareResultsFiles will compare two text file contents
* Arguments:
*		expectedResultPath - string, representing the path to the expected results file
*		resultsPath - string, representing the path to the actual results file
* Returns:
*		SUCCESS if the files are identical
*		CRASHED if we ran into an unrecoverable error
*		FAILURE if the files were compared succesfully and found unidentical
*/
int CompareResultsFiles(char* expectedResultPath, char* resultsPath);

/* ReadFileContents will read a file and returns it's contents
* Arguments:
*		path - string, representing the path to file to read
* Returns:
*		ResultFile representing the read file
*/
ResultFile *ReadFileContents(char *path);

/* GetThreadFromLine will return a thread struct object from a test file line
* Arguments:
*		testFileLine - string, representing a line from the tests file
* Returns:
*		Thread struct representing the new thread
*/
Thread* GetThreadFromLine(char *testFileLine);

/*
this function gets an exit code from the thread and returns a string representation
to be written to the log file
Params: exitCode - the code to be translated from int to string
return: the string representation if success else NULL is returned
*/
char* TranslateExitCode(Thread* thread);

//converts extension to txt in the end
char* ConverExeExtensionToTxt(char *orig, char *rep, char *with);

/*
this function splits line of test from the input file into the required arguments
parameters: str - the line to split into arguments
returns: an array of string that each one represents the arguments inserted
*/
char** SplitLineArguments(const char *str);

/*
splits the string given by str into sub strings by delimiter c
Parameters:
str - the string to split
c- the delimiter to split by
arr - the array of strings to store the sub strings splitted
*/
int SplitLine(const char *str, char c, char*** arr);

/*
this function creates the threads stored in allThreads then waits for them to finish and prints results into the file
given by pathToResultsFile
Parameters:
allThreads = the array of threads to run
numberOfThreads - the size of allThreads
pathToResultsFile - path To Results File
*/
int CreateAllThreads(Thread** allThreads, int numberOfThreads, char* pathToResultsFile);

/*
this function checks the resultCode of each process and prints it into the file
which path is given
Parameters:
all Threads - the array of threads to print the result into the file
numberOfThreads - the size of allThreads array
pathToResultsFile - the path to the result file to print into
Returns: 0 on success else -2
*/
int PrintResults(Thread** allThreads, int numberOfThreads, char* pathToResultsFile);



