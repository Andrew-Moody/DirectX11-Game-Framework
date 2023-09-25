#include "bytecode.h"

#include "d3dutil.h"

#include <fstream>
#include <string>

namespace d3d
{
	std::vector<uint8_t> CreateShaderByteCode(const std::string& path)
	{
		std::vector<uint8_t> byteCode{};

		std::ifstream fileStream(path, std::ios::in | std::ios::binary | std::ios::ate);

		if (!fileStream)
		{
			DB_LOG("Failed to open shader file" << '\n' << path);
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

		fileStream.read(reinterpret_cast<char*>(byteCode.data()), size);

		fileStream.close();

		return byteCode;
	}
}