#include "utils.h"

Local<String> string(const char *cstring) {
  return Nan::New(cstring).ToLocalChecked();
}

char* CString(Local<Value> string) {
  Nan::Utf8String original(string);
  char *cstring = *original;

  return cstring;
}

char* CString(Local<Object> hash, const char* key) {
  return CString(hash->Get(string(key)));
}

char *expand(const char *given_path) {
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
