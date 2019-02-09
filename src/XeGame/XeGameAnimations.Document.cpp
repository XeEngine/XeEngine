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

} } }