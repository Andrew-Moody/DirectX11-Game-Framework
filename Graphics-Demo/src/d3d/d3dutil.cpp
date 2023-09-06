#include "d3dutil.h"

#include <d3d11.h>
#include <wrl.h>
#include <dxgidebug.h>
#include <dxgi1_3.h>

#include <sstream>
#include <comdef.h>

namespace d3dutil
{
	void DXTraceW(const char* file, unsigned long line, const long hresult, const wchar_t* opMsg, bool popMsgBox)
	{
		// Basically a class that manages an error string decoded from HRESULT
		// Visual studio specific but wouldn't be that hard to implement with FormatMessage
		_com_error error(hresult);


		std::wostringstream stream;

		stream << '\n';

		stream << "DXTrace " << file << "(" << line << ")" << '\n' << error.ErrorMessage() << '\n';

		if (opMsg)
		{
			stream << opMsg << '\n';
		}

		stream << '\n';

		if (popMsgBox)
		{
			MessageBoxW(nullptr, stream.str().c_str(), nullptr, 0);
		}
		else
		{
			OutputDebugStringW(stream.str().c_str());
		}
	}


	void Output(const char* msg) noexcept
	{
		OutputDebugStringA("\n");
		OutputDebugStringA(msg);
		OutputDebugStringA("\n");
	}

	void Output(const std::stringstream& msg)
	{
		OutputDebugStringA("\n");
		OutputDebugStringA(msg.str().c_str());
		OutputDebugStringA("\n");
	}


	void ReportLiveObjects()
	{
		Microsoft::WRL::ComPtr<IDXGIDebug1> dxgiDebug;
		DXGIGetDebugInterface1(0, __uuidof(IDXGIDebug1), &dxgiDebug);

		Output("Start DXGI Report");

		Output("Detail");
		dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
		Output("Summary");
		dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_SUMMARY);
		Output("Ignore Internal");
		dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_IGNORE_INTERNAL);

		Output("End DXGI Report\n");
	}
}
