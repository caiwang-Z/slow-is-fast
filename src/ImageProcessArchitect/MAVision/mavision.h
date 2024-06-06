#pragma once

//#ifndef MAVISION_H
//#define MAVISION_H

#ifdef _WIN32
	#ifdef MAVISION_BUILD_DLL
	#define MAVISION_DLL_EXPORT __declspec(dllexport)
	#else
	#define MAVISION_DLL_EXPORT __declspec(dllimport)
	#endif
#else
	#ifdef MAVISION_BUILD_DLL
	#define MAVISION_DLL_EXPORT __attribute__((visibility("default")))
	#else
	#define MAVISION_DLL_EXPORT
	#endif
#endif

#include "commondefinitions.h"
#include "Core.h"

#ifdef __cplusplus
extern "C" {
#endif

///
	MAVISION_DLL_EXPORT maErr maStart();

	MAVISION_DLL_EXPORT maErr maStop();

	//MAVISION_DLL_EXPORT maErr maShutdown();

	MAVISION_DLL_EXPORT maErr maEnableDebug();

	MAVISION_DLL_EXPORT maErr maDisableDebug();

	MAVISION_DLL_EXPORT maErr maStopWaiting();

	MAVISION_DLL_EXPORT maErr maSaveDebugData(const char* debugDataFolder, int timeoutMS);


#ifdef __cplusplus
}
#endif
