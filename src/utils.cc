#include "utils.h"

Local<String> string(std::string stdstring) {
  return Nan::New(stdstring.c_str()).ToLocalChecked();
}

Local<String> downcase(Local<String> original) {
  return original->ToObject()->Get(string("toLowerCase"))
                 ->ToObject()->CallAsFunction(original, 0, 0)
                 .As<String>();
}

char* CString(Local<Value> string) {
  Nan::Utf8String original(string);
  char *cstring = *original;

  return cstring;
}

char* CString(Local<Object> hash, const char* key) {
  return CString(hash->Get(string(key)));
}

bool exist(const char* given_path) {
  struct stat buffer;

  if (stat(given_path, &buffer) != 0)
    return false;
  else
    return true;
}

bool exist(std::string given_path) {
  return exist(given_path.c_str());
}

bool image_exist(std::string *given_path) {
  std::string png_path, jpg_path;

  png_path.reserve(given_path->length() + 4);
  jpg_path.reserve(given_path->length() + 4);

  png_path.append(*given_path);
  png_path.append(".png");

  jpg_path.append(*given_path);
  jpg_path.append(".jpg");

  if (exist(png_path)) {
    given_path->append(".png");
    return true;
  } else if (exist(jpg_path)) {
    given_path->append(".jpg");
    return true;
  }

  return false;
}

void copy(TagLib::String str, std::string *dest) {
  for (TagLib::String::Iterator it = str.begin(); it != str.end(); it++) {
    if (isalnum(*it) || isspace(*it))
      *dest += *it;
  }
}
