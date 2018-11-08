#pragma once
#include <XeSDK/XeExtensionsRapidjsonReadStream.h>
#include <rapidjson/document.h>
#include <rapidjson/stream.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/writer.h>

#define JSON_TRYGET(val, key, func, defaultValue) \
	val.HasMember(key) ? val[key].func : (defaultValue)

#define JSON_IFGET(val, key, func, var, body) \
	if (val.HasMember(key)) { auto var = val.func; body }

#define JSON_IFARRAY(val, key, func, var, body) \
	if (val.HasMember(key)) { \
		auto var = val.func; \
		for (rapidjson::SizeType i = 0; i < var.Size(); ++i) { body } }

#define JSON_ADD(doc, key, val) \
	doc.AddMember(key, val, doc.GetAllocator());

#define JSON_STRADD(doc, key, val) { \
		Xe::StringSpan somerandomtmpthing1122 = val; \
		doc.AddMember(key, rapidjson::Value().SetString(somerandomtmpthing1122.GetData(), somerandomtmpthing1122.GetLength()), doc.GetAllocator()); \
	}