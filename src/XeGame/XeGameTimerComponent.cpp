#include "pch.h"
#include <XeGame/XeGameTimerComponent.h>

namespace Xe { namespace Game {
	TimerComponent::TimerComponent(u64 eventId) :
		m_EventId(eventId),
		m_Counter(0),
		m_Timer(0.0),
		m_Interval(1.0),
		m_Enabled(false)
	{ }

	u64 TimerComponent::GetEventId() const { return m_EventId; }

	u64 TimerComponent::GetCounter() const { return m_Counter; }

	bool TimerComponent::IsEnabled() const { return m_Enabled; }

	void TimerComponent::SetEnabled(bool enabled)
	{
		m_Enabled = enabled;
	}

	double TimerComponent::GetInterval() const { return m_Interval; }

	void TimerComponent::SetInterval(double interval)
	{
		m_Interval = interval;
	}

	void TimerComponent::Update(double deltaTime)
	{
		if (m_Enabled)
		{
			m_Timer += deltaTime;
			if (m_Timer >= m_Interval)
			{
				(*this)(TimerArgs{ *this, m_Timer });
			}
		}
	}
} }