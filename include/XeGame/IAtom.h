#pragma once

namespace Xe { namespace Game {
	struct IAtom
	{
		virtual void Update(double deltaTime) = 0;
	};
} }