// btconn.h - Contains declarations of connect/ disconnect functions functions
#pragma once

#ifdef BTCONN_EXPORTS
#define BTCONN_API __declspec(dllexport)
#else
#define BTCONN_API __declspec(dllimport)
#endif


extern "C" BTCONN_API DWORD Connect(long long mac);

// Get the current value in the sequence.
extern "C" BTCONN_API DWORD Disconnect(long long mac);