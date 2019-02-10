#pragma once
#include <XeGame/IGameAnimationsSequenceAnimator.h>
#include <XeGame/XeGameAnimationDef.h>

using namespace Xe;
using namespace Xe::Game;
using namespace Xe::Game::Animations;

class SequenceAnimator : public ISequenceAnimator
{
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
		if (m_Sequence.Duration == 0 || IsAnimationFinish())
			return;

		m_AbsoluteTimer += time;
		m_CurrentFrameTimer += time;

		double prevTimer;
		do
		{
			auto currentFrameSpeed = (*m_ItCurrent).Duration;
			if (currentFrameSpeed == 0)
				currentFrameSpeed = 64;

			prevTimer = m_CurrentFrameTimer;

			auto timeForNextFrame = 1024.0 / (m_Sequence.Duration * 64.0 / currentFrameSpeed);
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
};