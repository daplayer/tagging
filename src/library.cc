#include "library.h"
#include "get.h"

void Library::Initialize(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target) {
  v8::Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);

  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->SetClassName(string("Library"));

  Nan::SetPrototypeMethod(tpl, "get", Get);

  Library::constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, string("Library"), tpl->GetFunction());
}

void Library::NewInstance(const Nan::FunctionCallbackInfo<v8::Value>& args) {
  Local<Function> cons = Nan::New(Library::constructor());
  args.GetReturnValue().Set(Nan::NewInstance(cons, 0, {}).ToLocalChecked());
}

void Library::New(const Nan::FunctionCallbackInfo<v8::Value>& args) {
  if (args.IsConstructCall()) {
    Library *library = new Library();
    library->Wrap(args.This());

    if (args.Length() == 1) {
      Local<Object> hash = args[0]->ToObject();

      library->setArtists(hash->Get(string("artists"))->ToObject());
      library->setSingles(hash->Get(string("singles"))->ToObject());
    }

    args.This()->Set(string("artists"), library->artists());
    args.This()->Set(string("singles"), library->singles());

    args.GetReturnValue().Set(args.This());
  } else {
    v8::Local<v8::Function> cons = Nan::New(Library::constructor());

    args.GetReturnValue().Set(Nan::NewInstance(cons, 1, {}).ToLocalChecked());
  }
}

Library::Library() {
  Local<Object> hash = Nan::New<Object>();

  hash->Set(string("artists"), Nan::New<Object>());
  hash->Set(string("singles"), Nan::New<Array>());

  internal_hash.Reset(hash);
}

Library::~Library() {
}

Library::Library(Local<Object> hash) {
  internal_hash.Reset(hash);
}

void Library::AddArtist(std::string name) {
  Local<String> fullname = string(name);
  Local<String> key      = downcase(fullname);

  if (!artists()->Has(key)) {
    Local<Object> hash = Nan::New<Object>();

    hash->Set(string("name"), fullname);
    hash->Set(string("albums"), Nan::New<Object>());
    hash->Set(string("singles"), Nan::New<Array>());

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
  Local<String> key = downcase(string(artist));

  // If the record has an artist, we must first ensure that it
  // is referenced and then push the single to the artist's singles
  // array.
  if (artist.length()) {

    if (!artists()->Has(key))
      AddArtist(artist);

    Local<Object> artist_singles = artists()->Get(key)->ToObject()
                                            ->Get(string("singles"))->ToObject();

    artist_singles->Set(artist_singles->Get(string("length")), record);
  }

  singles()->Set(singles()->Get(string("length")), record);
}

Local<Object> Library::artists() {
  Local<Object> hash = Nan::New(internal_hash);

  return hash->Get(string("artists"))->ToObject();
}

Local<Object> Library::singles() {
  Local<Object> hash = Nan::New(internal_hash);

  return hash->Get(string("singles"))->ToObject();
}

void Library::setArtists(Local<Object> object) {
  Local<Object> hash = Nan::New(internal_hash);

  hash->Set(string("artists"), object);
}

void Library::setSingles(Local<Object> array) {
  Local<Object> hash = Nan::New(internal_hash);

  hash->Set(string("singles"), array);
}

void Library::Get(const Nan::FunctionCallbackInfo<Value>& args) {
  const unsigned int argc = args.Length();

  Library *library = Nan::ObjectWrap::Unwrap<Library>(args.This());

  if (argc == 0)
    return Nan::ThrowRangeError("Wrong number of arguments");

  Local<Array>  files    = args[0]->ToObject().As<Array>();
  Local<Object> callback = Nan::New<Object>(), second_arg, third_arg;

  const unsigned int files_count = files->Length();

  std::string cover_folder;

  // The different possible syntaxes are:
  //
  //   get(files);
  //   get(files, callback);
  //   get(files, cover_folder);
  //   get(files, cover_folder, callback);

  // If we have two arguments, it could either mean that
  // a callback has been passed or that a folder containing
  // cover arts has been given.
  if (argc == 2) {
    second_arg = args[1]->ToObject();

    if (second_arg->IsFunction()) {
      callback = second_arg.As<Function>();
      cover_folder = "";
    } else {
      Nan::Utf8String covers(args[1]);
      cover_folder = std::string(*covers);
    }
  } else if (argc == 3) {
    Nan::Utf8String covers(args[1]);

    cover_folder = std::string(*covers);
    callback     = args[2]->ToObject().As<Function>();
  }

  for (uint32_t i = 0; i < files_count; i++) {
    Nan::Utf8String name(files->Get(i));
    std::string path = std::string(*name);

    if (!exist(path))
      return Nan::ThrowError("The audio file doesn't exist");

    tags(path, cover_folder, library);

    if (callback->IsFunction()) {
      Local<Value> argv[2] = { Nan::New(i+1), Nan::New(files_count) };
      callback->CallAsFunction(callback, 2, argv);
    }
  }

  args.GetReturnValue().Set(true);
}
