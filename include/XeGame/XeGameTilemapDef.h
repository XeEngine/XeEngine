#pragma once

namespace Xe { namespace Game {

	struct TileData
	{
		union
		{
			unsigned int Data;
			struct
			{
				unsigned int Tile : 29;
				unsigned int Rotate : 1;
				unsigned int Flip : 1;
				unsigned int Mirror : 1;

			};
		};

		operator int() const { return Data; }
		int operator = (int x) { Data = x; return x; }
	};

} }