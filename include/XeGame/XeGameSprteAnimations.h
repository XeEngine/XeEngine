#pragma once
#include <XeSDK/XeDef.h>
#include <XeSDK/XeString.h>
#include <XeSDK/XeCollectionsArray.h>
#include <XeSDK/XeCollectionsDictionary.h>
#include <XeSDK/IIoStream.h>

namespace Xe { namespace Game { namespace SpriteAnimations {
	struct Texture
	{
		u64 Name;
		u32 ColorKeys[4];
		bool MaintainPaletteOrder;
	};

	struct Frame
	{
		s16 Left, Top, Right, Bottom;
		s16 CenterX, CenterY;
		s16 Speed;
		union
		{
			u8 Flags;
			struct
			{
				bool FlipX, FlipY;
				bool Trigger;
			};
		};
		u8 RESERVED;
	};

	struct FrameSequence
	{
		s16 Speed;
		u16 FrameTableIndex, FrameCount;
		u8 Loop, Texture;
		union
		{
			u8 Flags;
			struct
			{
				bool FlipX, FlipY;
			};
		};
	};

	class SpriteAnimation
	{
		String m_Name;
		Collections::Array<u32> m_Textures;
		Collections::Array<Frame> m_Frames;
		//Collections::Dictionary<u32, FrameSequence> m_FrameSequence;
		//Collections::Dictionary<u32, String> m_Names;
		
		SpriteAnimation();
		~SpriteAnimation();

		static SpriteAnimation& ReadBinary(IO::IStream& pStream);
		static SpriteAnimation& ReadJson(IO::IStream& pStream);
		
		void WriteBinary(IO::IStream& pStream);
		void WriteJson(IO::IStream& pStream);

	};
} } }