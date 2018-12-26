#include "pch.h"
#include <XeGame/XeGameSprteAnimations.h>
#include <XeSDK/XeCryptoFnv1a.h>
#include <XeSDK/XeJson.h>

using namespace Xe;
using namespace Xe::Game::SpriteAnimations;

SpriteAnimation::SpriteAnimation()
{

}

SpriteAnimation::~SpriteAnimation()
{

}

SpriteAnimation& SpriteAnimation::ReadBinary(IO::IStream& pStream)
{

}

SpriteAnimation& SpriteAnimation::ReadJson(IO::IStream& pStream)
{
	rapidjson::Document document;
	rapidjson::XeReadStream stream(&pStream);
	document.ParseStream(stream);

	SpriteAnimation o;
	o.m_Name = JSON_TRYGET(document, "name", GetString(), StringSpan::Empty);

	JSON_IFGET(document, "Textures", GetArray(), a, {
		for (rapidjson::SizeType i = 0; i < a.Size(); ++i)
		{
			StringSpan name = JSON_TRYGET(a[i], "name", GetString(), StringSpan::Empty);
			if (!name.IsEmpty())
			{
				Texture texture;
				texture.Name = Crypto::Fnv1a64(name.GetData(), name.GetLength());
				texture.MaintainPaletteOrder = JSON_TRYGET(a[i], "maintainPaletteOrder", GetBool(), true);

				JSON_IFGET(a[i], "ColorKeys", GetArray(), colorKeys, {
					if (colorKeys.Size() > 0) texture.ColorKeys[0] = colorKeys[0].GetUint();
					if (colorKeys.Size() > 1) texture.ColorKeys[1] = colorKeys[1].GetUint();
					if (colorKeys.Size() > 2) texture.ColorKeys[2] = colorKeys[2].GetUint();
					if (colorKeys.Size() > 3) texture.ColorKeys[3] = colorKeys[3].GetUint();
				});

				JSON_IFGET(a[i], "Transparencies", GetArray(), colorKeys, {
					if (colorKeys.Size() > 0) texture.ColorKeys[0] = colorKeys[0].GetUint();
					if (colorKeys.Size() > 1) texture.ColorKeys[1] = colorKeys[1].GetUint();
					if (colorKeys.Size() > 2) texture.ColorKeys[2] = colorKeys[2].GetUint();
					if (colorKeys.Size() > 3) texture.ColorKeys[3] = colorKeys[3].GetUint();
				});
			}
		}
	});

	JSON_IFGET(document, "Frames", GetArray(), a, {
		//
	});
}

void SpriteAnimation::WriteBinary(IO::IStream& pStream)
{

}

void SpriteAnimation::WriteJson(IO::IStream& pStream)
{

}