#ifndef _PATTCUT_COMMON_C_H_
#define _PATTCUT_COMMON_C_H_

#include "pattcut/export_helper.h"


/** Represents bool type (0 = false, otherwise = true) */
typedef int PattCut_BOOL;

/** Represents error codes (look at 'PattCut_ErrorCode_*' definitions) */
typedef int PattCut_ErrorCode;

#define PattCut_ErrorCode_OK (0)
#define PattCut_ErrorCode_Error (-1)


extern "C"
{
	/** 
	 * Free memory allocated for 'error_message' buffer
	 * @error_message_ptr - pointer to the the string containing error message
	 * @return - PattCut_ErrorCode_OK if there's no errors, otherwise returns PattCut_ErrorCode_Error
	 */
	DLL_EXPORT PattCut_ErrorCode pattcut_free_error_message(char** error_message_ptr);
}

#endif