#pragma once

#include <sstream>
#include <cassert>

constexpr bool outputSuccess{ true };

#if defined(DEBUG) | defined(_DEBUG)
	
	// A macro must be used to pass the __File__ and __LINE__ arguments since their
	// value depends on where they are used

	#ifndef HR
	#define HR(x);														\
		{																\
			const long hr = (x);										\
			if (hr < 0 || outputSuccess)								\
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

	#ifndef DB_ASSERT
	#define DB_ASSERT(x); assert(x);
	#endif

	#ifndef DB_LOG
	#define DB_LOG(x);										\
	{														\
		std::stringstream stream{};							\
		try													\
		{													\
			stream << x;									\
			d3dutil::Output(stream);						\
		}													\
		catch (...)											\
		{													\
			d3dutil::Output("Exception thrown by DB_LOG");	\
		}													\
	}
	#endif


#else

	#ifndef HR
	#define HR(x) (x)
	#endif

	#ifndef HR_RETURN
	#define HR_RETURN(x) (x)
	#endif

	#ifndef DB_ASSERT
	#define DB_ASSERT(x)
	#endif
	#ifndef DB_LOG
	#define DB_LOG(x)
	#endif
	
#endif


namespace d3dutil
{
	// Mimic DXTrace from depreciated dxerr.lib
	void DXTraceW(const char* file, unsigned long line, const long hresult, const wchar_t* opMsg, bool popMsgBox);

	void Output(const char* msg) noexcept;

	void Output(const std::stringstream& msg);

	void ReportLiveObjects();
}