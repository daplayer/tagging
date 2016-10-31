#ifndef EXTRACT_H
#define EXTRACT_H

#include "utils.h"
#include "library.h"

#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>

using v8::Array;
using v8::Function;

void Get(const Nan::FunctionCallbackInfo<Value>& args);

void tags(std::string location, std::string cover_folder, Library *library);

void inline extractPicture(std::string location,  std::string cover_folder,
                           TagLib::String title,  TagLib::String album,
                           TagLib::String artist, Local<Object> record);

void mp3Picture(std::string location, std::string img_path, Local<Object> record);

void mp4Picture(std::string location, std::string img_path, Local<Object> record);

#endif
