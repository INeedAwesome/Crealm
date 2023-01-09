#pragma once
#include <stdint.h>

#define SUCCESS 0
#define FAILED 1

#define ERROR_GAME_ALREADY_RUNNING 1
#define ERROR_WINDOW_REGISTRATION 10
#define ERROR_WINDOW_CREATION 11
#define ERROR_WINDOW_MONITOR_INFO 12
#define ERROR_NO_MEMORY 20

int m_currentError;
int CRGetLastError();
void CRSetLastError(int p_errorCode);