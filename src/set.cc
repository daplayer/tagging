#include "set.h"
#include "image_file.h"

void Set(const Nan::FunctionCallbackInfo<Value>& args) {
  // Raise if the user doesn't supply enough arguments
  if (args.Length() < 2)
    return Nan::ThrowRangeError("Wrong number of arguments");

  Nan::Utf8String v8_file_name(args[0]);

  std::string location  = std::string(*v8_file_name);
  std::string extension = location.substr(location.length() - 3, 3);
  Local<Object> hash    = args[1]->ToObject();

  // Raise if the audio file doesn't exist
  if (!exist(location))
    return Nan::ThrowError("The audio file doesn't exist");

  TagLib::FileRef audioFile(location.c_str());

  // Set the basic text attributes like title, etc.
  if (hash->Has(string("title")))
    audioFile.tag()->setTitle(TString(hash, "title"));
  if (hash->Has(string("album")))
    audioFile.tag()->setAlbum(TString(hash, "album"));
  if (hash->Has(string("artist")))
    audioFile.tag()->setArtist(TString(hash, "artist"));
  if (hash->Has(string("genre")))
    audioFile.tag()->setGenre(TString(hash, "genre"));
  if (hash->Has(string("track")))
    audioFile.tag()->setTrack(hash->Get(string("track"))->NumberValue());

  audioFile.save();

  // Set the attached picture
  if (hash->Has(string("icon"))) {
    std::string img_location = std::string(CString(hash, "icon"));

    if (!img_location.length())
      return;

    if (!exist(img_location)) {
      Nan::ThrowError("The given artwork path doesn't exist");
      return;
    }

    ImageFile imageFile(img_location.c_str());
    TagLib::MPEG::File mpegFile(location.c_str());

    TagLib::ID3v2::Tag *tag = mpegFile.ID3v2Tag(true);
    TagLib::ID3v2::AttachedPictureFrame *frame = new TagLib::ID3v2::AttachedPictureFrame;

    if (extension == "png")
      frame->setMimeType("image/png");
    else
      frame->setMimeType("image/jpeg");

    frame->setPicture(imageFile.data());

    tag->addFrame(frame);

    mpegFile.save();
  }

  args.GetReturnValue().Set(true);
}

