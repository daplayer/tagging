#include "image_file.h"

ImageFile::ImageFile(const char *file) : TagLib::File(file) { }

TagLib::ByteVector ImageFile::data() {
  return readBlock(length());
}

TagLib::Tag *ImageFile::tag() const {
  return 0;
}

TagLib::AudioProperties *ImageFile::audioProperties() const {
  return 0;
}

bool ImageFile::save() {
  return false;
}
