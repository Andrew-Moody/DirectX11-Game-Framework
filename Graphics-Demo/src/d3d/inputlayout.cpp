#include "inputlayout.h"

#include "d3dutil.h"

namespace d3d
{
	using std::vector;

	void InputLayout::bind(ID3D11DeviceContext& context)
	{
		DB_LOG("Binding InputLayout");

		context.IASetInputLayout(m_inputLayout.Get());
	}

	InputLayout::InputLayout(ID3D11Device& device, const vector<D3D11_INPUT_ELEMENT_DESC>& inputDescs, const vector<char>& bytecode)
	{
		DB_LOG("Creating InputLayout, inputDescs size: " << inputDescs.size() << ", bytcode size: " << bytecode.size() << '\n');

		HR(device.CreateInputLayout(inputDescs.data(), inputDescs.size(), bytecode.data(), bytecode.size(), &m_inputLayout));

		assert(m_inputLayout.Get());
	}
}
