#include "library.h"

Library::Library() {
  internal_hash = Nan::New<Object>();

  internal_hash->Set(string("artists"), Nan::New<Object>());
  internal_hash->Set(string("singles"), Nan::New<Array>());
}

Library::Library(Local<Object> hash) {
  internal_hash = hash;
}

Local<Object> Library::getHash() {
  return internal_hash;
}

void Library::AddArtist(std::string name) {
  Local<String> fullname = string(name);
  Local<String> key      = downcase(fullname);

  if (!artists()->Has(key)) {
    Local<Object> hash = Nan::New<Object>();

    hash->Set(string("name"), fullname);
    hash->Set(string("albums"), Nan::New<Object>());

    artists()->Set(key, hash);
  }
}

void Library::AddTrack(std::string artist, std::string album, Local<Object> record) {
  Local<String> key = downcase(string(artist));

  if (!artists()->Has(key))
    AddArtist(artist);

  Local<Object> albums = artists()->Get(key)->ToObject()
                                  ->Get(string("albums"))->ToObject();

  if (!albums->Has(string(album)))
    albums->Set(string(album), Nan::New<Array>());

  Local<Object> album_array = albums->Get(string(album))->ToObject();

  // Here we could rely on the record's `track` attribute but the problem
  // is that if the frame isn't set, TagLib will automatically set its
  // value to 0 and 0 - 1 would produce a segfault and if two tracks have
  // the same track number, one would erase the other.
  album_array->Set(album_array->Get(string("length")), record);
}

void Library::AddSingle(std::string artist, Local<Object> record) {
  singles()->Set(singles()->Get(string("length")), record);
}

Local<Object> Library::artists() {
  return internal_hash->Get(string("artists"))->ToObject();
}

Local<Object> Library::singles() {
  return internal_hash->Get(string("singles"))->ToObject();
}
