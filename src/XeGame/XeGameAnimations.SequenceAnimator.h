#pragma once
#include <XeGame/IGameAnimationsSequenceAnimator.h>
#include <XeGame/XeGameAnimationDef.h>

using namespace Xe;
using namespace Xe::Game;
using namespace Xe::Game::Animations;

class SequenceAnimator : public ISequenceAnimator
{
	const int DefaultFrameSpeed = 256;
	const double FrameSpeedDivisor = 1;
	const FrameSequence m_Sequence;
	std::list<Frame>::const_iterator m_ItLoop;
	std::list<Frame>::const_iterator m_ItCurrent;
	size_t m_IdxCurrent;
	size_t m_LoopCount;
	double m_AbsoluteTimer;
	double m_CurrentFrameTimer;

	std::list<Frame>::const_iterator GetFrameIteratorAt(size_t index)
	{
		size_t i = 0;
		auto it = m_Sequence.Frames.begin();

		while (i++ != index)
		{
			++it;
		}

		return it;
	}



	void MoveTimeForward(double time)
	{
		double prevTimer;
		do
		{
			auto currentFrameSpeed = (*m_ItCurrent).Duration;
			if (currentFrameSpeed == 0)
				currentFrameSpeed = DefaultFrameSpeed;

			prevTimer = m_CurrentFrameTimer;

			auto timeForNextFrame = FrameSpeedDivisor / (m_Sequence.Speed * 64.0 / currentFrameSpeed);
			if (m_CurrentFrameTimer >= timeForNextFrame)
			{
				m_CurrentFrameTimer -= timeForNextFrame;
				++m_IdxCurrent;
				++m_ItCurrent;

				if (IsAnimationFinish())
				{
					m_IdxCurrent = m_Sequence.Loop;
					m_ItCurrent = m_ItLoop;
					++m_LoopCount;
				}
			}

		} while (m_CurrentFrameTimer != prevTimer);
	}

	void MoveTimeBackward(double time)
	{
		double prevTimer;
		do
		{
			auto prevIndex = m_IdxCurrent;
			auto prevFrame = m_ItCurrent;
			auto prevLoop = m_LoopCount;
			if (m_LoopCount > 0)
			{
				if (m_IdxCurrent == m_Sequence.Loop)
				{
					prevFrame = std::prev(m_Sequence.Frames.end());
					prevIndex = m_Sequence.Frames.size() - 1;
					prevLoop--;
				}
				else
				{
					prevFrame = std::prev(prevFrame);
					prevIndex--;
				}
			}
			else if (m_IdxCurrent > 0)
			{
				prevFrame = std::prev(prevFrame);
				prevIndex--;
			}
			else
			{
				break;
			}

			auto previousFrameSpeed = (*prevFrame).Duration;
			if (previousFrameSpeed == 0)
				previousFrameSpeed = DefaultFrameSpeed;

			prevTimer = m_CurrentFrameTimer;

			auto timeForPrevFrame = FrameSpeedDivisor / (m_Sequence.Speed * 64.0 / previousFrameSpeed);
			if (-m_CurrentFrameTimer >= timeForPrevFrame)
			{
				m_CurrentFrameTimer += timeForPrevFrame;
				m_IdxCurrent = prevIndex;
				m_ItCurrent = prevFrame;
				m_LoopCount = prevLoop;
			}

		} while (m_CurrentFrameTimer != prevTimer);
	}

public:
	SequenceAnimator(const FrameSequence& frameSequence) :
		m_Sequence(frameSequence)
	{
		if (frameSequence.Frames.size() == 0)
			throw std::invalid_argument(NAMEOF(frameSequence)" must have at least 1 frame");

		if (frameSequence.Loop > GetFrameCount())
			throw std::invalid_argument(NAMEOF(frameSequence.Loop)" must be less or equal than " NAMEOF(GetFramesCount));

		m_ItLoop = GetFrameIteratorAt(m_Sequence.Loop);
		m_ItCurrent = m_Sequence.Frames.begin();
		m_IdxCurrent = 0;
		m_LoopCount = 0;
		m_AbsoluteTimer = 0;
		m_CurrentFrameTimer = 0;
	}

	~SequenceAnimator()
	{
		
	}

	const String& GetName() const
	{
		return m_Sequence.Name;
	}

	size_t GetLoopIndex() const
	{
		return m_Sequence.Loop;
	}

	size_t GetFrameCount() const
	{
		return m_Sequence.Frames.size();
	}

	const Frame& GetFrame(size_t index) const
	{
		if (index >= GetFrameCount())
			throw std::invalid_argument(NAMEOF(index)" must be less than " NAMEOF(GetFramesCount));

		size_t i = 0;
		for (const auto& frame : m_Sequence.Frames)
		{
			if (i++ == index)
				return frame;
		}

		ASSERT(!"Unexpected end of " NAMEOF(GetFrame) ": possible bug");
	}

	size_t GetCurrentFrameIndex() const
	{
		return m_IdxCurrent;
	}

	const Frame& GetCurrentFrame() const
	{
		return *m_ItCurrent;
	}

	bool IsAnimationFinish() const
	{
		return m_ItCurrent == m_Sequence.Frames.end();
	}

	size_t GetLoopHitCount() const
	{
		return m_LoopCount;
	}

	double GetTime() const
	{
		return m_AbsoluteTimer;
	}

	void SetTime(double time)
	{
		double timeDifference = time - m_AbsoluteTimer;
		AddTime(timeDifference);
	}

	void AddTime(double time)
	{
		if (time == 0 || m_Sequence.Speed == 0 || IsAnimationFinish())
			return;

		m_AbsoluteTimer += time;
		m_CurrentFrameTimer += time;

		if (m_AbsoluteTimer < 0)
			throw std::invalid_argument(NAMEOF(GetTime)" cannot be negative");

		if (time > 0)
		{
			MoveTimeForward(time);
		}
		else
		{
			MoveTimeBackward(time);
		}
	}
};