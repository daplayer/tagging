#include "utils.h"

Local<String> string(Isolate *isolate, TagLib::String string) {
  return String::NewFromUtf8(isolate, string.toCString());
}

Local<String> string(Isolate *isolate, const char *string) {
  return String::NewFromUtf8(isolate, string);
}

char* CString(Isolate *isolate, Local<Value> string) {
  String::Utf8Value original(string);
  char *cstring = (char *)malloc(original.length() + 1);

  strcpy(cstring, *original);

  return cstring;
}

char* CString(Isolate *isolate, Local<Object> hash, char* key) {
  return CString(isolate, hash->Get(String::NewFromUtf8(isolate, key)));
}

const char *expand(const char *given_path) {
  int length     = MAX_PATH_LENGTH + 1 + strlen(given_path);
  char *location = (char *)malloc(length * sizeof(char));
  char cwd[MAX_PATH_LENGTH];

  if (given_path[0] != '/') {
    getcwd(cwd, MAX_PATH_LENGTH);

    strcpy(location, cwd);
    strcat(location, "/");
    strcat(location, given_path);
  } else {
    strcpy(location, given_path);
  }

  return location;
}

void throwException(Isolate *isolate, char* message) {
  isolate->ThrowException(Exception::TypeError(string(isolate, message)));
}

bool exist(const char* given_path) {
  struct stat buffer;

  if (stat(given_path, &buffer) != 0)
    return false;
  else
    return true;
}

bool image_exist(char* given_path) {
  char png_path[strlen(given_path) + 4];
  char jpg_path[strlen(given_path) + 4];

  strcpy(png_path, given_path);
  strcat(png_path, ".png");

  strcpy(jpg_path, given_path);
  strcat(jpg_path, ".jpg");

  if (exist(png_path)) {
    strcat(given_path, ".png");
    return true;
  } else if (exist(jpg_path)) {
    strcat(given_path, ".jpg");
    return true;
  }

  return false;
}
