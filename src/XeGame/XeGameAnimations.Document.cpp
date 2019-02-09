#include "pch.h"
#include <XeGame/XeGameAnimationDef.h>

namespace Xe { namespace Game { namespace Animations {

	void Internal_WriteToStream(IO::IStream* pStream, const rapidjson::Value& value)
	{
		rapidjson::XeWriteStream stream(pStream);
		rapidjson::PrettyWriter<rapidjson::XeWriteStream> writer(stream);
		writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
		value.Accept(writer);
	}

	void Internal_Parse(Frame& obj, const rapidjson::Value& value)
	{
		JSON_GET(value, "left", GetUint(), obj.Rect.left);
		JSON_GET(value, "top", GetUint(), obj.Rect.top);
		JSON_GET(value, "right", GetUint(), obj.Rect.right);
		JSON_GET(value, "bottom", GetUint(), obj.Rect.bottom);
		JSON_GET(value, "pivotx", GetUint(), obj.Pivot.x);
		JSON_GET(value, "pivoty", GetUint(), obj.Pivot.y);
		JSON_GET(value, "duration", GetUint(), obj.Duration);
		JSON_GET(value, "reference", GetUint(), obj.Reference);
	}

	void Internal_Save(const Frame& obj, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& allocator)
	{
		JSON_ADD(value, "left", obj.Rect.left);
		JSON_ADD(value, "top", obj.Rect.top);
		JSON_ADD(value, "right", obj.Rect.right);
		JSON_ADD(value, "bottom", obj.Rect.bottom);
		JSON_ADD(value, "pivotx", obj.Pivot.x);
		JSON_ADD(value, "pivoty", obj.Pivot.y);
		JSON_ADD(value, "duration", obj.Duration);
		JSON_ADD(value, "reference", obj.Reference);
	}

	void Internal_Parse(FrameSequence& obj, const rapidjson::Value& value)
	{
		JSON_GET(value, "name", GetString(), obj.Name);
		JSON_GET(value, "duration", GetUint(), obj.Duration);
		JSON_GET(value, "loop", GetUint(), obj.Loop);
		JSON_GET(value, "flags", GetUint(), obj.Flags);

		JsonGetList(obj.Frames, value, "frames", Internal_Parse);
	}

	void Internal_Save(const FrameSequence& obj, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& allocator)
	{
		JSON_STRADD(value, "name", obj.Name);
		JSON_ADD(value, "duration", obj.Duration);
		JSON_ADD(value, "loop", obj.Loop);
		JSON_ADD(value, "flags", obj.Flags);

		JsonAddList(obj.Frames, value, allocator, "frames", Internal_Save);
	}

	void Internal_Parse(SpriteSheet& obj, const rapidjson::Value& value)
	{
		JSON_GET(value, "path", GetString(), obj.Path);
	}

	void Internal_Save(const SpriteSheet& obj, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& allocator)
	{
		JSON_STRADD(value, "path", obj.Path);
	}

	Frame& Animations::ParseJson(Frame& obj, Xe::IO::IStream* pStream)
	{
		rapidjson::XeReadStream stream(pStream);
		Internal_Parse(obj, rapidjson::Document().ParseStream(stream));

		return obj;
	}

	const Frame& Animations::SaveJson(const Frame& obj, Xe::IO::IStream* pStream)
	{
		rapidjson::Document document;
		document.SetObject();
		Internal_Save(obj, document, document.GetAllocator());
		Internal_WriteToStream(pStream, document);

		return obj;
	}

	FrameSequence& Animations::ParseJson(FrameSequence& obj, Xe::IO::IStream* pStream)
	{
		rapidjson::XeReadStream stream(pStream);
		Internal_Parse(obj, rapidjson::Document().ParseStream(stream));

		return obj;
	}

	const FrameSequence& Animations::SaveJson(const FrameSequence& obj, Xe::IO::IStream* pStream)
	{
		rapidjson::Document document;
		document.SetObject();
		Internal_Save(obj, document, document.GetAllocator());
		Internal_WriteToStream(pStream, document);

		return obj;
	}

	SpriteSheet& ParseJson(SpriteSheet& obj, Xe::IO::IStream* pStream)
	{
		rapidjson::XeReadStream stream(pStream);
		Internal_Parse(obj, rapidjson::Document().ParseStream(stream));

		return obj;
	}

	const SpriteSheet& SaveJson(const SpriteSheet& obj, Xe::IO::IStream* pStream)
	{
		rapidjson::Document document;
		document.SetObject();
		Internal_Save(obj, document, document.GetAllocator());
		Internal_WriteToStream(pStream, document);

		return obj;
	}

	void Animations::ParseJson(AnimationDocument& obj, const rapidjson::Value& value)
	{
		JSON_GET(value, "name", GetString(), obj.Name);
		JsonGetList(obj.SpriteSheets, value, "spritesheets", Internal_Parse);
		JsonGetList(obj.Sequences, value, "sequences", Internal_Parse);
	}

	void Animations::SaveJson(const AnimationDocument& obj, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& allocator)
	{
		JSON_STRADD(value, "name", obj.Name);
		JsonAddList(obj.SpriteSheets, value, allocator, "spritesheets", Internal_Save);
		JsonAddList(obj.Sequences, value, allocator, "sequences", Internal_Save);
	}

	AnimationDocument& Animations::ParseJson(AnimationDocument& obj, Xe::IO::IStream* pStream)
	{
		rapidjson::XeReadStream stream(pStream);
		ParseJson(obj, rapidjson::Document().ParseStream(stream));

		return obj;
	}

	const AnimationDocument& Animations::SaveJson(const AnimationDocument& obj, Xe::IO::IStream* pStream)
	{
		rapidjson::Document document;
		document.SetObject();
		SaveJson(obj, document, document.GetAllocator());
		Internal_WriteToStream(pStream, document);

		return obj;
	}
} } }