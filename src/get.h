#ifndef EXTRACT_H
#define EXTRACT_H

#include "utils.h"

#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>

using v8::Number;

void Get(const Nan::FunctionCallbackInfo<Value>& args);

void mp3Picture(std::string location, std::string img_path, Local<Object> record);

void mp4Picture(std::string location, std::string img_path, Local<Object> record);

#endif
