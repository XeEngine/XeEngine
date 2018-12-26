#pragma once
#include <XeSDK/XeExtensionsRapidjsonReadStream.h>
#include <rapidjson/document.h>
#include <rapidjson/stream.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/writer.h>

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