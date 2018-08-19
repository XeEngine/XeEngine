#pragma once

namespace Xe { namespace Game {
	interface IGamePlatformCallback
	{
		//! \brief Called when the platform overlay changed its state.
		//! \param[in] activated If the overlay is currently activated or not.
		/** When activated, the game should freeze or put in "pause".
		 */
		virtual void OnGameOverlayActivated(bool activated) = 0;
	};
} }