#pragma once

#if defined(DEBUG) | defined(_DEBUG)
	
	// A macro must be used to pass the __File__ and __LINE__ arguments since their
	// value depends on where they are used

	#ifndef HR
	#define HR(x);													\
	{																\
		const long hr = (x);										\
		if (hr < 0)													\
		{															\
			d3dutil::DXTraceW(__FILE__, __LINE__, hr, L#x, false);	\
		}															\
	}														
	#endif

	#ifndef HR_RETURN
	#define HR_RETURN(x);											\
	{																\
		const long hr = (x);										\
		d3dutil::DXTraceW(__FILE__, __LINE__, hr, L#x, false);		\
		if (hr < 0)													\
		{															\
			return hr;												\
		}															\
	}
	#endif

#else
	#ifndef
	#define HR(x) (x)
	#endif
#endif


namespace d3dutil
{
	// Mimic DXTrace from depreciated dxerr.lib
	void DXTraceW(const char* file, unsigned long line, const long hresult, const wchar_t* opMsg, bool popMsgBox);

	void Output(const char* msg);

	void ReportLiveObjects();
}