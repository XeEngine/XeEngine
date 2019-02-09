#pragma once
#include <XeGame/IGameAnimationsSequenceAnimator.h>
#include <XeGame/XeGameAnimationDef.h>

using namespace Xe;
using namespace Xe::Game;
using namespace Xe::Game::Animations;

class SequenceAnimator : public ISequenceAnimator
{
	const FrameSequence m_Sequence;

public:
	SequenceAnimator(const FrameSequence& frameSequence) :
		m_Sequence(frameSequence)
	{
		
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
};