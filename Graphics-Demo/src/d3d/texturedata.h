#pragma once

#include <memory>

namespace d3d
{
	struct TextureData
	{
		std::unique_ptr<uint8_t[]> data;

		int width{};
		int height{};
		int channels;
	};
}
