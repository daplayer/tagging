#include "get.h"

void tags(std::string location, std::string cover_folder, Library *library) {
  Local<Object> record = Nan::New<Object>();

  TagLib::FileRef f(location.c_str());
  TagLib::Tag *tag = f.tag();
  TagLib::AudioProperties *properties = f.audioProperties();

  TagLib::String title  = tag->title();
  TagLib::String artist = tag->artist();
  TagLib::String album  = tag->album();
  TagLib::String genre  = tag->genre();

  // Set the default title based on the file's location
  // if there's no tags for the file.
  if (!title.length()) {
    std::string::size_type slash  = location.rfind('/');
    std::string::size_type dot    = location.rfind('.');

    std::string default_title = location.substr(slash+1, dot - slash - 1);

    for (std::string::iterator it = default_title.begin(); it != default_title.end(); it++)
      title += *it;
  }

  record->Set(string("title"),    string(title.toCString(true)));
  record->Set(string("artist"),   string(artist.toCString(true)));
  record->Set(string("genre"),    string(genre.toCString(true)));
  record->Set(string("id"),       string(location));
  record->Set(string("track"),    Nan::New(tag->track()));
  record->Set(string("duration"), Nan::New(properties->length()));

  if (cover_folder.length())
    extractPicture(location, cover_folder, title, album, artist, record);

  if (album.length())
    library->AddTrack(artist.toCString(true), album.toCString(true), record);
  else
    library->AddSingle(artist.toCString(true), record);
}

void inline extractPicture(std::string location,  std::string cover_folder,
                           TagLib::String title,  TagLib::String album,
                           TagLib::String artist, Local<Object> record) {
  std::string extension = location.substr(location.length() - 3, 3);

  if (extension == "mp3")
    mp3Picture(location, cover_folder, record);
  else if (extension == "m4a")
    mp4Picture(location, cover_folder, record);
}

void mp3Picture(std::string location, std::string cover_folder, Local<Object> record) {
  TagLib::MPEG::File mp3_file(location.c_str());
  TagLib::ID3v2::Tag *mp3_tag = mp3_file.ID3v2Tag(true);
  TagLib::ID3v2::FrameList pictures = mp3_tag->frameList("APIC");

  if (pictures.isEmpty())
    return;

  TagLib::ID3v2::AttachedPictureFrame *picture_frame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (*pictures.begin());;

  unsigned int checksum = picture_frame->picture().checksum();

  std::stringstream path_stream;

  path_stream << cover_folder;
  path_stream << "/";
  path_stream << checksum;

  std::string img_path = path_stream.str();

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

void mp4Picture(std::string location, std::string cover_folder, Local<Object> record) {
  TagLib::MP4::File mp4_file(location.c_str());
  TagLib::MP4::Tag *mp4_tag = mp4_file.tag();

  if (!mp4_tag->contains("covr"))
    return;

  TagLib::MP4::CoverArtList covert_art_list = mp4_tag->item("covr").toCoverArtList();

  if (covert_art_list.isEmpty())
    return;

  TagLib::MP4::CoverArt cover_art = covert_art_list[0];

  unsigned int checksum = cover_art.data().checksum();

  std::stringstream path_stream;

  path_stream << cover_folder;
  path_stream << "/";
  path_stream << checksum;

  std::string img_path = path_stream.str();

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
