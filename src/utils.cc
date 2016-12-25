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

TagLib::String TString(Local<Object> hash, const char *key) {
  return TagLib::String(CString(hash, key), TagLib::String::UTF8);
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
