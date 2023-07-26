#pragma once

#define RETURN_HR(hr)                                           return(hr)
#define RETURN_LAST_ERROR()                                     return HRESULT_FROM_WIN32(GetLastError())
#define RETURN_WIN32(win32err)                                  return HRESULT_FROM_WIN32(win32err)

#define RETURN_IF_FAILED(hr)                                    do { HRESULT __hrRet = hr; if (FAILED(__hrRet)) { RETURN_HR(__hrRet); }} while (0, 0)
#define RETURN_IF_FAILED_PLAYFAB(PlayFabResult)                 do { auto __pfRet = PlayFabResult; if (FAILED(__pfRet.hr)) { return {__pfRet.hr, __pfRet.errorMessage}; }} while (0, 0)
#define RETURN_IF_WIN32_BOOL_FALSE(win32BOOL)                   do { BOOL __boolRet = win32BOOL; if (!__boolRet) { RETURN_LAST_ERROR(); }} while (0, 0)
#define RETURN_IF_NULL_ALLOC(ptr)                               do { if ((ptr) == nullptr) { RETURN_HR(E_OUTOFMEMORY); }} while (0, 0)
#define RETURN_HR_IF(hr, condition)                             do { if (condition) { RETURN_HR(hr); }} while (0, 0)
#define RETURN_HR_IF_FALSE(hr, condition)                       do { if (!(condition)) { RETURN_HR(hr); }} while (0, 0)
#define RETURN_HR_INVALIDARG_IF_NULL(arg)                       do { RETURN_HR_IF(E_INVALIDARG, arg == nullptr); } while(0, 0)
#define RETURN_LAST_ERROR_IF(condition)                         do { if (condition) { RETURN_LAST_ERROR(); }} while (0, 0)
#define RETURN_LAST_ERROR_IF_NULL(ptr)                          do { if ((ptr) == nullptr) { RETURN_LAST_ERROR(); }} while (0, 0)

#if _DEBUG
#define ASSERT_SUCCEEDED(hr)                                    do { HRESULT __hrRet = hr; assert(SUCCEEDED(__hrRet)); } while(0, 0)
#else 
#define ASSERT_SUCCEEDED(hr)                                    do { (void)hr; } while(0, 0)
#endif

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(args) (void)(args);
#endif
