#pragma once
#include <stdint.h>

#define CRSUCCESS 0
#define CRFAILED 1

#define ERROR_GAME_ALREADY_RUNNING 1
#define ERROR_WINDOW_REGISTRATION 10
#define ERROR_WINDOW_CREATION 11
#define ERROR_WINDOW_MONITOR_INFO 12
#define ERROR_WINDOW_SET_STYLE 13
#define ERROR_WINDOW_SET_POS 14
#define ERROR_NO_MEMORY 20

int32_t m_currentError;
int32_t CRGetLastError();
void CRSetLastError(int32_t p_errorCode);