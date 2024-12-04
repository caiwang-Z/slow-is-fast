#pragma once

#ifdef BUILD_CALC_DLL
#define CALC_DLL_EXPORT __declspec(dllexport)
#else
#define CALC_DLL_EXPORT __declspec(dllimport)
#endif

namespace CALC {
CALC_DLL_EXPORT int add(int a, int b);
CALC_DLL_EXPORT int substract(int a, int b);

}  // namespace CALC