#include "get.h"

void Get(const Nan::FunctionCallbackInfo<Value>& args) {
  Local<Object> record = Nan::New<Object>();

  // Ensure that we have at least the file's location
  if (args.Length() == 0)
    return Nan::ThrowRangeError("Wrong number of arguments");

  Nan::Utf8String v8_file_name(args[0]);

  std::string location = std::string(*v8_file_name);

  // Raise if the file actually doesn't exist
  if (!exist(location))
    return Nan::ThrowError("The audio file doesn't exist");

  TagLib::FileRef f(location.c_str());
  TagLib::Tag *tag = f.tag();
  TagLib::AudioProperties *properties = f.audioProperties();

  TagLib::String title  = tag->title();
  TagLib::String artist = tag->artist();
  TagLib::String album  = tag->album();
  TagLib::String genre  = tag->genre();

  // std::string title;

  // Set the default title based on the file's location
  // if there's no tags for the file.
  if (!title.length()) {
    std::string::size_type slash  = location.rfind('/');
    std::string::size_type dot    = location.rfind('.');

    std::string default_title = location.substr(slash+1, dot - slash - 1);

    for (std::string::iterator it = default_title.begin(); it != default_title.end(); it++)
      title += *it;
  }

  record->Set(string("title"),    string(title.toCString()));
  record->Set(string("artist"),   string(artist.toCString()));
  record->Set(string("album"),    string(album.toCString()));
  record->Set(string("genre"),    string(genre.toCString()));
  record->Set(string("id"),       string(location));
  record->Set(string("track"),    Nan::New(tag->track()));
  record->Set(string("duration"), Nan::New(properties->length()));

  if (args.Length() > 1) {
    Nan::Utf8String v8_cover_folder(args[1]);
    std::string cover_folder = std::string(*v8_cover_folder);

    //   1 ('/')
    // + 3 (" - ")
    // + 1 ('.')
    // + 3 ("png" or "jpg")
    // + 1 ('\0')
    // = 9
    std::string img_path;
    img_path.reserve(cover_folder.length() + artist.length() + 9 +
                     album.length() ? album.length() : title.length());

    img_path.append(cover_folder);
    img_path += '/';

    // Copy the name of the artist making sure that we are
    // only copying alpha-numeric chars in order to avoid
    // any filesystem errors creating the file.
    if (artist.length()) {
      for (TagLib::String::Iterator it = artist.begin(); it != artist.end(); it++) {
        if (isalnum(*it) || isspace(*it))
          img_path += *it;
      }

      img_path.append(" - ");
    }

    // Ditto `artist` but for the album or the title variable;
    // we only want the alpha-numeric chars.
    if (album.size())
      for (TagLib::String::Iterator it = album.begin(); it != album.end(); it++) {
        if (isalnum(*it) || isspace(*it))
          img_path += *it;
      }
    else
      for (TagLib::String::Iterator it = title.begin(); it != title.end(); it++) {
        if (isalnum(*it) || isspace(*it))
          img_path += *it;
      }

    if (image_exist(&img_path)) {
      record->Set(string("icon"), string(img_path.c_str()));
    } else {
      std::string extension = location.substr(location.length() - 3, 3);

      if (extension == "mp3")
        mp3Picture(location, img_path, record);
      else if (extension == "m4a")
        mp4Picture(location, img_path, record);
    }
  }

  args.GetReturnValue().Set(record);
}

void mp3Picture(std::string location, std::string img_path, Local<Object> record) {
  TagLib::MPEG::File mp3_file(location.c_str());
  TagLib::ID3v2::Tag *mp3_tag = mp3_file.ID3v2Tag(true);
  TagLib::ID3v2::FrameList pictures = mp3_tag->frameList("APIC");

  if (pictures.isEmpty())
    return;

  TagLib::ID3v2::AttachedPictureFrame *picture_frame;

  for (TagLib::ID3v2::FrameList::ConstIterator it = pictures.begin(); it != pictures.end(); it++) {
    picture_frame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (*it);

    if (picture_frame->mimeType() == "image/png")
      img_path.append(".png");
    else
      img_path.append(".jpg");

    if (!exist(img_path)) {
      size_t image_size = picture_frame->picture().size();
      const char *data  = picture_frame->picture().data();
      FILE *cover_file  = fopen(img_path.c_str(), "wb");

      fwrite(data, image_size, 1, cover_file);
      fclose(cover_file);
    }

    record->Set(string("icon"), string(img_path));
  }
}

void mp4Picture(std::string location, std::string img_path, Local<Object> record) {
  TagLib::MP4::File mp4_file(location.c_str());
  TagLib::MP4::Tag *mp4_tag = mp4_file.tag();
  TagLib::MP4::ItemListMap items_list_map = mp4_tag->itemListMap();
  TagLib::MP4::Item cover_item = items_list_map["covr"];
  TagLib::MP4::CoverArtList covert_art_list = cover_item.toCoverArtList();

  if (covert_art_list.isEmpty())
    return;

  TagLib::MP4::CoverArt cover_art = covert_art_list.front();

  img_path.append(".jpg");

  if (!exist(img_path)) {
    size_t image_size = cover_art.data().size();
    const char *data  = cover_art.data().data();
    FILE *cover_file  = fopen(img_path.c_str(), "wb");

    fwrite(data, image_size, 1, cover_file);
    fclose(cover_file);
  }

  record->Set(string("icon"), string(img_path));
}
