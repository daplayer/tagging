#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

#include <unistd.h>
#include <sys/stat.h>

#include <nan.h>

#include <taglib/tag.h>
#include <taglib/mpegfile.h>
#include <taglib/mp4file.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v2frame.h>
#include <taglib/attachedpictureframe.h>

using v8::Local;
using v8::String;
using v8::Object;
using v8::Isolate;
using v8::Value;
using v8::Exception;

Local<String> string(std::string stdstring);

char* CString(Local<Object> hash, const char* key);

bool exist(std::string given_path);

bool image_exist(std::string *given_path);

#endif
