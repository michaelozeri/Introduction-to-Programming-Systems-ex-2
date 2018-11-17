#include "Command_Thread.h"
#include "ThreadManager.h"
/*
* A simplified API for creating threads.
* Input Arguments :
*p_start_routine : A pointer to the function to be executed by the thread.
*     This pointer represents the starting address of the thread.
*     The function has to have this specific signature :
*DWORD WINAPI FunctionName(LPVOID lpParam);
*With FunctionName being replaced with the function's name.
*   p_thread_parameters: A(void *) to a variable to be passed to the thread.
* Output Arguments :
*p_thread_id : A pointer to a variable that receives the thread identifier.
*     If this parameter is NULL, the thread identifier is not returned.
* Return :
*If the function succeeds, the return value is a handle to the new thread.
*   If the function fails, the return value is NULL.
*   To get extended error information, call GetLastError.
* Notes :
*This function is just a wrapper for CreateThread.
*/
static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPVOID p_thread_parameters,
	LPDWORD p_thread_id);


static int CreateThreadMain()
{
	HANDLE thread_handle;
	DWORD thread_id;
	DWORD wait_code;
	DWORD exit_code;
	BOOL ret_val;
	COMMAND_THREAD_params_t *p_thread_params;

	/*
	* Allocate memory for thread parameters
	* Why use dynamic memory allocation?
	*
	* We have to keep in mind that if we pass a pointer to a local variable
	* (variable on the stack), then that variable may not exist by the time the
	* thread tries to access it. This can happen when we create a thread inside a
	* function that returns before the thread finishes execution.
	* The heap is shared among all threads of the program, so it is safe to use.
	*
	* On this particulr example, we could have avoided the malloc and use a local
	* variable, because we wait on the thread on the same function where it is
	* created.
	*/
	p_thread_params = (COMMAND_THREAD_params_t *)malloc(sizeof(COMMAND_THREAD_params_t));
	if (NULL == p_thread_params)
	{
		printf("Error when allocating memory");
		return -1;
	}

	/* Prepare parameters for thread */
	p_thread_params->Command =
		p_thread_params->ExpectedResultPath;

		/* Create thread */
		thread_handle = CreateThreadSimple(CommandThread, p_thread_params, &thread_id);
	if (NULL != thread_handle)
	{
		printf("Error when creating thread\n");
		return -1;
	}


	/* Wait */
	wait_code = WaitForSingleObject(thread_handle, INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting\n");
		return -1;
	}

	/* Check the DWORD returned by MathThread */
	ret_val = GetExitCodeThread(thread_handle, &exit_code);
	if (0 == ret_val)
	{
		printf("Error when getting thread exit code\n");
	}

	/* Print results, if thread succeeded */
	/*if (MATH_THREAD__CODE_SUCCESS == exit_code)
	{
	printf("%d + %d = %d\n", p_thread_params->num1,
	p_thread_params->num2,
	p_thread_params->res);
	}
	else
	{
	printf("Error in thread: %d\n", exit_code);
	}*/

	/* Free memory */
	free(p_thread_params);

	/* Close thread handle */
	ret_val = CloseHandle(thread_handle);
	if (false == ret_val)
	{
		printf("Error when closing\n");
		return -1;
	}

	return 0;
}

static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPVOID p_thread_parameters,
	LPDWORD p_thread_id)
{
	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(-1);
	}

	if (NULL == p_thread_id)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(-1);
	}

	thread_handle = CreateThread(
		NULL,                /*  default security attributes */
		0,                   /*  use default stack size */
		p_start_routine,     /*  thread function */
		p_thread_parameters, /*  argument to thread function */
		0,                   /*  use default creation flags */
		p_thread_id);        /*  returns the thread identifier */

	return thread_handle;
}