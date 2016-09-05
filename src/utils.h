#ifndef UTILS_H
#define UTILS_H

#define MAX_PATH_LENGTH 300

#include <stdlib.h>
#include <string.h>

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

Local<String> string(const char *string);

char* CString(Local<Object> hash, const char* key);

char* expand(const char *given_path);

bool exist(const char* given_path);

bool image_exist(char* given_path);

#endif
