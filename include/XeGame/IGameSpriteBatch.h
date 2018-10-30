#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/XeMathVector4.h>
#include <XeGame/ITextureManager.h>
#include <list>

namespace Xe { namespace Game {
	struct SpriteBatchDrawElement
	{
		float x, y, w, h;
	};

	struct SpriteBatchDrawCommand
	{
		TexId TexId;
		std::list<SpriteBatchDrawElement> Elements;
	};

	struct SpriteBatchDrawData
	{
		std::list<SpriteBatchDrawCommand> Commands;
	};

	// SORT MODES:
	//  Deferred
	//  Texture
	//  Priority
	//  PriorityTexture
	interface ISpriteBatch : public IObject
	{
		virtual void BeginRender() = 0;
		virtual void EndRender(/*SORT MODE*/) = 0;
		virtual SpriteBatchDrawData& GetDrawData() = 0;

		virtual void Draw(TexId tex, float x, float y) = 0;
		virtual void Draw(TexId tex, float x, float y, float width, float height) = 0;

		virtual void Draw(TexId tex,
			const Xe::Math::Vector2f& traslation,
			const Xe::Math::Vector2f& center,
			const Xe::Math::Vector2f& scale,
			const Xe::Math::Vector3f& rotation);

		virtual void Draw(TexId tex, const Xe::Math::Matrix4& matrix);
	};
} }
