var Tagging = require(__dirname + '/../build/Release/tagging.node')

/**
 * Removes all given files from the library.
 *
 * This is implemented in JavaScript because it looks like
 * dealing with Arrays at the C++ level is really slow and
 * anyway, a library won't drastically change so even if
 * we had a performance boost on the C++ side, this method
 * won't have to deal with big arrays and the code is much
 * cleaner this way.
 *
 * @param  {Array} files - The files to remove.
 * @return {null}
 */
Tagging.Library.prototype.remove = function(files) {
  var artists      = this.artists;
  var singles      = this.singles;
  var artist_names = Object.keys(this.artists);

  files.forEach((file) => {
    // +------------------------+
    // | Looking inside artists |
    // +------------------------+

    // Artist's albums
    artist_names.every((artist) => {
      return Object.keys(artists[artist].albums).every((titre) => {
        var album = artists[artist].albums[titre];

        return album.every((item, index) => {
          if (item.id == file) {
            album.splice(index, 1);

            if (album.length == 0)
              delete artists[artist].albums[titre];

            return false;
          } else {
            return true;
          }
        });
      });
    });

    // Artist's singles
    artist_names.every((artist) => {
      var singles = artists[artist].singles;

      return singles.every((single, index) => {
        if (single.id == file) {
          singles.splice(index, 1);
          return false;
        } else {
          return true;
        }
      });
    });

    // +------------------------+
    // | Looking inside singles |
    // +------------------------+
    singles.every((single, index) => {
      if (single.id == file) {
        singles.splice(index, 1);
        return false;
      } else {
        return true;
      }
    });
  });
};

module.exports = Tagging;
