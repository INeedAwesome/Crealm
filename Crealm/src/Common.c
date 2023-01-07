#include "Common.h"

int CRGetLastError()
{
	return m_currentError;
}

void CRSetLastError(int p_errorCode)
{
	m_currentError = p_errorCode;
}
