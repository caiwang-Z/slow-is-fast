#ifndef MAVISION_COMMON_DEFINITIONS_H
#define MAVISION_COMMON_DEFINITIONS_H

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

#ifdef __cplusplus

extern "C" {
#endif

///
enum maErr {
  maErrOK,
  maErrCamera,
  maErrTimeout,
  maErrAbort,
  maErrFunctionDeactivated,
  maErrFileIO,
  maErrImageProcessing,
  maErrUnknown

};

/* start/resume image processing pipeline */
MAVISION_DLL_EXPORT maErr vscStart();

/* stop/pause image processing pipeline */
MAVISION_DLL_EXPORT maErr vscStop();

#ifdef __cplusplus
}
#endif

#endif