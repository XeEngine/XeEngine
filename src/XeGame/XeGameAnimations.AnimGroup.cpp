#include "pch.h"
#include <XeGame/IGameAnimationsAnimationGroup.h>
#include <XeGame/XeGameAnimations.h>
#include <XeGame/XeGameAnimationDef.h>
#include "XeGameAnimations.SequenceAnimator.h"

using namespace Xe;
using namespace Xe::Game;
using namespace Xe::Game::Animations;

class AnimGroup : public IAnimationGroup
{
	AnimationDocument m_Document;
public:
	AnimGroup(const AnimationDocument& document) :
		m_Document(document)
	{
		
	}

	~AnimGroup()
	{
		
	}

	const String& GetName() const
	{
		return m_Document.Name;
	}


	std::vector<SpriteSheet> GetSpriteSheets() const
	{
		return
		{
			std::make_move_iterator(std::begin(m_Document.SpriteSheets)),
			std::make_move_iterator(std::end(m_Document.SpriteSheets))
		};
	}

	std::vector<String> GetSequenceNames() const
	{
		std::vector<String> names;
		for (const auto& sequence : m_Document.Sequences)
		{
			names.push_back(sequence.Name);
		}

		return names;
	}

	bool IsSequenceExists(const StringSpan& sequenceName) const
	{
		const FrameSequence* pFrameSequence;
		return TryGetSequence(&pFrameSequence, sequenceName);
	}

	bool TryGetSequence(const FrameSequence** ppFrameSequence, const StringSpan& sequenceName) const
	{
		for (const auto& sequence : m_Document.Sequences)
		{
			if (StringSpan::Compare(sequence.Name, sequenceName) == 0)
			{
				*ppFrameSequence = &sequence;
				return true;
			}
		}

		*ppFrameSequence = nullptr;
		return false;
	}

	bool TryCreateSequenceAnimator(ISequenceAnimator** ppSequenceAnimator, const StringSpan& sequenceName) const
	{
		const FrameSequence* pFrameSequence;
		if (!TryGetSequence(&pFrameSequence, sequenceName))
		{
			*ppSequenceAnimator = nullptr;
			return false;
		}

		*ppSequenceAnimator = new SequenceAnimator(*pFrameSequence);
		return true;
	}
};

bool Animations::Factory(IAnimationGroup** ppAnimationGroup, const AnimationDocument& document)
{
	*ppAnimationGroup = new AnimGroup(document);

	return !!*ppAnimationGroup;
}