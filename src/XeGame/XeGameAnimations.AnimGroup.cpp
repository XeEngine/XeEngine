#include "pch.h"
#include <XeGame/IGameAnimationsAnimationGroup.h>
#include <XeGame/XeGameAnimations.h>
#include <XeGame/XeGameAnimationDef.h>

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
		for (const auto& sequence : m_Document.Sequences)
		{
			if (StringSpan::Compare(sequence.Name, sequenceName) == 0)
				return true;
		}

		return false;
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
};

bool Animations::Factory(IAnimationGroup** ppAnimationGroup, const AnimationDocument& document)
{
	*ppAnimationGroup = new AnimGroup(document);

	return !!*ppAnimationGroup;
}