#ifndef UTILS_H
#define UTILS_H

#define MAX_PATH_LENGTH 300

#include <stdlib.h>

#include <unistd.h>
#include <sys/stat.h>

#include <node.h>

#include <taglib/tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v2frame.h>
#include <taglib/Attachedpictureframe.h>

using v8::Local;
using v8::String;
using v8::Object;
using v8::Isolate;
using v8::Value;
using v8::Exception;

Local<String> string(Isolate *isolate, TagLib::String string);

Local<String> string(Isolate *isolate, const char *string);

char* CString(Isolate *isolate, Local<Value> string);

char* CString(Isolate *isolate, Local<Object> hash, char* key);

const char* expand(const char *given_path);

void throwException(Isolate *isolate, char* message);

bool exist(const char* given_path);

bool image_exist(char* given_path);

#endif
