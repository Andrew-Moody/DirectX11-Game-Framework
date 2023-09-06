#pragma once

namespace d3d
{
	using UINT8 = unsigned char;

	struct Vertex
	{
		float x;
		float y;
		float z;

		UINT8 r;
		UINT8 g;
		UINT8 b;
		UINT8 a;
	};
}