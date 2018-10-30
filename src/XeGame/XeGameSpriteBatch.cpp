#include "pch.h"
#include <XeGame/XeGameSpriteBatch.h>
#include <unordered_map>

using namespace Xe::Game;

class CSpriteBatch : public ISpriteBatch
{
	struct DrawSpriteCmd
	{
		float x, y, w, h;
	};

	SpriteBatchDrawData m_DrawData;
	std::unordered_map<TexId, std::list<DrawSpriteCmd>> m_DrawBuilder;
public:
	CSpriteBatch()
	{
	}

	~CSpriteBatch()
	{
	}

	void BeginRender()
	{
		m_DrawBuilder.clear();
	}

	void EndRender()
	{
		for (auto it = m_DrawBuilder.begin(); it != m_DrawBuilder.end(); ++it)
		{
			auto& item = *it;

			SpriteBatchDrawCommand cmd;
			cmd.TexId = item.first;
			m_DrawData.Commands.push_back(EndRender(cmd, item.second));
		}
	}

	SpriteBatchDrawCommand& EndRender(SpriteBatchDrawCommand& cmd, std::list<DrawSpriteCmd>& list)
	{
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			auto& item = *it;
			cmd.Elements.push_back(SpriteBatchDrawElement
				{
					item.x,
					item.y,
					item.w,
					item.h
				});
		}

		return cmd;
	}

	SpriteBatchDrawData& GetDrawData()
	{
		return m_DrawData;
	}

	void Draw(TexId tex, float x, float y)
	{
		Draw(tex, x, y, 1.0f, 1.0f);
	}

	void Draw(TexId tex, float x, float y, float width, float height)
	{
		auto it = m_DrawBuilder.find(tex);
		if (it == m_DrawBuilder.end())
		{
			std::list<DrawSpriteCmd> cmds;
			cmds.push_back(NewDrawSpriteCmd(x, y, width, height));
			m_DrawBuilder[tex] = cmds;
		}
		else
		{
			auto& cmd = (*it).second;
			cmd.push_back(NewDrawSpriteCmd(x, y, width, height));
		}
	}

	DrawSpriteCmd NewDrawSpriteCmd(float x, float y, float w, float h)
	{
		return DrawSpriteCmd { x, y, w, h };
	}
};

void Factory(ISpriteBatch** ppSpriteBatch)
{
	*ppSpriteBatch = new CSpriteBatch;
}