require('./test_helper');

describe('Tagging', () => {
  describe('#get', () => {
    describe('prototype', () => {
      it('should accept absolute paths', () => {
        assert.doesNotThrow(() => {
          Tagging.get(path.join(__dirname, 'fixtures/tagged.mp3'));
        });
      });

      it('should take at least one argument', () => {
        assert.throws(() => {
          Tagging.get();
        }, /Wrong number of arguments/);
      });
    });

    describe('behavior', () => {
      it('should raise if the audio file does not exist', () => {
        assert.throws(() => {
          Tagging.get('dummy/path');
        }, /The audio file doesn't exist/);
      });

      it('should return the basic tags', () => {
        var tags = Tagging.get(helpers.fixture('tagged.mp3'));

        assert.equal(tags.artist, 'Darius');
        assert.equal(tags.title, 'Maliblue');
        assert.equal(tags.album, 'Velour');
        assert.equal(tags.genre, 'Electronic');
        assert.equal(tags.track, 4);
      });

      it('should set the duration', () => {
        var tags = Tagging.get(helpers.fixture('tagged.mp3'));

        assert.equal(tags.duration, 259);
      });

      it('should set the title based on the file name', () => {
        var tags = Tagging.get(helpers.fixture('raw.mp3'));

        assert.equal(tags.title, 'raw');
      });
    });

    describe('cover file', () => {
      beforeEach(() => {
        helpers.createCoverFolder();
      });

      afterEach(() => {
        helpers.clearCoverFolder();
      });

      it('should be named based on the name of the artist and of the album', () => {
        var cover_file = helpers.fixture('covers/Darius - Velour.jpg');

        Tagging.get(helpers.fixture('tagged.mp3'), cover_folder);

        assert(fs.lstatSync(cover_file).isFile());
      });

      it('should not regenerate the artwork if it already exists', () => {
        var cover_file = helpers.fixture('covers/Darius - Velour.jpg');

        fs.appendFileSync(cover_file, 'i am a cover lulz', 'utf-8');
        Tagging.get(helpers.fixture('tagged.mp3'), cover_folder);

        assert.equal(fs.readFileSync(cover_file, 'utf-8'), 'i am a cover lulz');
      });

      it('should pick the cover file even if there is no picture frame', () => {
        var tags = Tagging.get(helpers.fixture('without_cover.mp3'), cover_folder);

        assert.equal(tags.icon, helpers.fixture('covers/Bakermat - Strandfeest.jpg'));
      });
    });
  });
});
