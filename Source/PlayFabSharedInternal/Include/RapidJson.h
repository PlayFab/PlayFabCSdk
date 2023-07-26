#pragma once

#include "Memory.h" // Required as rapidjson will depend on our Allocators

#define RAPIDJSON_NAMESPACE PlayFab::rapidjson
#define RAPIDJSON_NAMESPACE_BEGIN namespace PlayFab { namespace rapidjson {
#define RAPIDJSON_NAMESPACE_END } }

#define RAPIDJSON_DEFAULT_ALLOCATOR PlayFab::JsonAllocator
#define RAPIDJSON_DEFAULT_STACK_ALLOCATOR PlayFab::JsonAllocator

#define RAPIDJSON_NO_INT64DEFINE

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/error/en.h>
#include <rapidjson/stream.h>