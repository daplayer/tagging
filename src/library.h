#ifndef LIBRARY_H
#define LIBRARY_H

#include "utils.h"

using v8::Array;

class Library {
  public:
    Library();
    Library(Local<Object> hash);
    Local<Object> getHash();
    void AddTrack(std::string artist, std::string album, Local<Object> record);
    void AddSingle(std::string artist, Local<Object> record);

  private:
    Local<Object> internal_hash;
    Local<Object> artists();
    Local<Object> singles();
    void AddArtist(std::string name);
};

#endif
