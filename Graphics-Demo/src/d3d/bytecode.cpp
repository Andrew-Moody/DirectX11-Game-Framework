#include "bytecode.h"

#include "d3dutil.h"

#include <fstream>

namespace d3d
{
	std::vector<char> CreateShaderByteCode(const wchar_t* path)
	{
		std::vector<char> byteCode{};

		std::ifstream fileStream(path, std::ios::in | std::ios::binary | std::ios::ate);

		if (!fileStream)
		{
			d3dutil::Output("Failed to open shader file");
			return byteCode;
		}

		const std::streampos pos{ fileStream.tellg() };

		if (pos < 0)
		{
			d3dutil::Output("tellg returned a negative streampos");
			return byteCode;
		}

		const size_t size = static_cast<size_t>(pos);

		byteCode.resize(size);

		fileStream.seekg(0, std::ios::beg);

		fileStream.read(byteCode.data(), size);

		fileStream.close();

		return byteCode;
	}
}