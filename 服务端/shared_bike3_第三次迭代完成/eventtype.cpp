#include "eventtype.h"

static EErrorReason EER[] =
{
	{ERRC_SUCCESS, "OK."},
	{ERRC_INVALID_MSG, "Invalid message."},
	{ERRC_INVALID_DATA, "Invalid data."},
	{ERRC_METHOD_NOT_ALLOWED, "Method not allowed."},
	{ERRO_PROCCESS_FALED, "Proccess failed."},
	{ERRO_BIKE_IS_TOOK, "Bike is took."},
	{ERRO_BIKE_IS_RUNNING, "Bike is running."},
	{ERRO_BIKE_IS_DAMAGED, "Bike is damaged."},
	{ERRO_NULL, "Undefined"}
};

const char* getReasonByErrorCode(i32 code)
{
	i32 i = 0;
	for (i = 0; EER[i].code != ERRO_NULL; i++)
	{
		if (EER[i].code == code)
		{
			return EER[i].reason;
		}
	}
	return EER[i].reason;
}
