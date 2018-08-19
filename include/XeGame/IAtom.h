#pragma once

namespace Xe { namespace Game {
	interface IAtom
	{
		virtual void Update(double deltaTime) = 0;
	};
} }