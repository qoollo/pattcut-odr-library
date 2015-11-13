#include "inc/export/pattcut/c_api/Common.h"


PattCut_ErrorCode pattcut_free_error_message(char** error_message_ptr)
{
	if (error_message_ptr == nullptr)
		return PattCut_ErrorCode_Error;
	if (*error_message_ptr == nullptr)
		return PattCut_ErrorCode_OK;

	char* error_message = *error_message_ptr;

	try
	{
		delete[] error_message;
		*error_message_ptr = nullptr;
		return PattCut_ErrorCode_OK;
	}
	catch (...)
	{
		return PattCut_ErrorCode_Error;
	}
}