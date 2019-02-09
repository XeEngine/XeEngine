#pragma once
#include <XeSDK/XeExtensionsRapidjsonReadStream.h>
#include <rapidjson/document.h>
#include <rapidjson/stream.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/writer.h>
#include <list>

#define JSON_GET(val, key, func, var) \
	if (val.HasMember(key)) { var = val[key].func; }

#define JSON_TRYGET(val, key, func, defaultValue) \
	val.HasMember(key) ? val[key].func : (defaultValue)

#define JSON_IFARRAY(val, key, func, var, body) \
	if (val.HasMember(key)) { \
		const auto& var = val[key].func; \
		for (rapidjson::SizeType i = 0; i < var.Size(); ++i) { body } }

#define JSON_ADD(doc, key, val) \
	doc.AddMember(key, val, allocator)

#define JSON_ADDIF(doc, key, val, def) { if (val != def) { JSON_ADD(doc, key, val); } }

#define JSON_STRADD(doc, key, val) { \
		Xe::StringSpan somerandomtmpthing1122 = val; \
		if (!somerandomtmpthing1122.IsEmpty()) { \
		doc.AddMember(key, rapidjson::Value().SetString(somerandomtmpthing1122.GetData(), somerandomtmpthing1122.GetLength()), allocator); } \
	}

#define JSON_STRADDIF (doc, key, val, def) { if (val != def) { JSON_STRADD(doc, key, val); } }

template <class T>
void JsonGetList(
	std::list<T>& list,
	const rapidjson::Value& value,
	const char* memberName,
	void(*fget)(T&, const rapidjson::Value&))
{
	if (value.HasMember(memberName))
	{
		const auto& items = value[memberName].GetArray();
		list.clear();

		for (rapidjson::SizeType i = 0; i < items.Size(); ++i)
		{
			list.push_back(T());
			auto& item = *std::prev(list.end());
			fget(item, items[i]);
		}
	}
}

template <class T>
void JsonAddList(
	const std::list<T>& list,
	rapidjson::Value& value,
	rapidjson::MemoryPoolAllocator<>& allocator,
	const char* memberName,
	void (*fadd)(const T&, rapidjson::Value&, rapidjson::MemoryPoolAllocator<>&))
{
	if (list.size() > 0)
	{
		rapidjson::Value jsonArray;
		jsonArray.SetArray();

		for (const auto& item : list)
		{
			rapidjson::Value jsonItem;
			jsonItem.SetObject();

			fadd(item, jsonItem, allocator);
			jsonArray.PushBack(jsonItem, allocator);
		}

		value.AddMember(rapidjson::Value::StringRefType::GenericStringRef(memberName), jsonArray, allocator);
	}
}