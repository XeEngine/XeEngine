#pragma once
#include <XeSDK/XeEvent.h>

namespace Xe { namespace Game {
	//! \brief Callback to define 
	interface ITimerCallback
	{
		//! \brief Triggered every time that the timer reaches 0.
		void OnTimer(u64 counter, double elapsed);
	};

	class TimerComponent;
	struct TimerArgs
	{
		// The timer that triggered the event.
		TimerComponent& Timer;

		// Elapsed time from the last call.
		double Elapsed;
	};

	class TimerComponent : public Xe::Event<TimerArgs>
	{
		u64 m_EventId;
		u64 m_Counter;
		double m_Timer;
		double m_Interval;
		bool m_Enabled;

	public:
		TimerComponent(u64 eventId);

		// Event ID
		u64 GetEventId() const;

		// Number of times which the timer triggered.
		u64 GetCounter() const;

		bool IsEnabled() const;
		void SetEnabled(bool enabled);

		double GetInterval() const;
		void SetInterval(double interval);

		void Update(double deltaTime);
	};
} }