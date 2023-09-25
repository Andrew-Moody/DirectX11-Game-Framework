#include "inputlayout.h"
#include "d3dapp.h"
#include "d3dutil.h"

namespace d3d
{
	using std::vector;

	void InputLayout::bind(D3DApp& app)
	{
		DB_LOG("Binding InputLayout");

		app.getContext().IASetInputLayout(m_inputLayout.Get());
	}

	InputLayout::InputLayout(D3DApp& app, const vector<D3D11_INPUT_ELEMENT_DESC>& inputDescs, const vector<uint8_t>& bytecode)
	{
		DB_LOG("Creating InputLayout, inputDescs size: " << inputDescs.size() << ", bytecode size: " << bytecode.size() << '\n');

		HR(app.getDevice().CreateInputLayout(inputDescs.data(), static_cast<UINT>(inputDescs.size()), bytecode.data(), bytecode.size(), &m_inputLayout));

		assert(m_inputLayout.Get());
	}
}
