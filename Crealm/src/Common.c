#include "Common.h"

int32_t CRGetLastError()
{
	return m_currentError;
}

void CRSetLastError(int32_t p_errorCode)
{
	m_currentError = p_errorCode;
}
