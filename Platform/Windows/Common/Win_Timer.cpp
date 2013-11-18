/*
*	Molotok Editor, 2013 (C) PrinceOfDispersia
*	Win_Timer.cpp - timer implementation
*
**/

#include "PlatformCommon.h"

LARGE_INTEGER frequency;
LARGE_INTEGER startTime;

void Sys_TimerStart()
{
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startTime);
}

float Sys_TimeElapsed()
{
	 LARGE_INTEGER currentTime;

	 QueryPerformanceCounter(&currentTime);
	 
	 return (float)((double)(currentTime.QuadPart - startTime.QuadPart) /(double)frequency.QuadPart) ;
}