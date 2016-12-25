require('./test_helper');

describe('Library', () => {
  beforeEach(() => {
    this.library = new Tagging.Library();
  });

  describe('#get', () => {
    describe('prototype', () => {
      it('should accept absolute paths', () => {
        assert.doesNotThrow(() => {
          this.library.get([path.join(__dirname, 'fixtures/tagged.mp3')]);
        });
      });

      it('should take at least one argument', () => {
        assert.throws(() => {
          this.library.get();
        }, /Wrong number of arguments/);
      });
    });

    describe('behavior', () => {
      it('should raise if the audio file does not exist', () => {
        assert.throws(() => {
          this.library.get(['dummy/path']);
        }, /The audio file doesn't exist/);
      });

      it('should properly update the library fields', () => {
        var file_path = helpers.fixture('tagged.mp3');

        this.library.get([file_path]);

        assert.deepEqual(this.library.artists, {
          darius: {
            name: 'Darius',
            albums: {
              Velour: [{
                artist:   'Darius',
                duration: 259,
                id:       file_path,
                title:    'Maliblue',
                track:    4,
                genre:    'Electronic'
              }]
            },
              singles: []
            }
        });
      });

      it('should set the title based on the file name', () => {
        this.library.get([helpers.fixture('raw.mp3')])

        var tags = this.library.singles[0];

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

      it('should be named based on the name of the artist and album', () => {
        var cover_file = helpers.fixture('covers/2087182743.jpg');

        this.library.get([helpers.fixture('tagged.mp3')], cover_folder);

        assert(fs.lstatSync(cover_file).isFile());
      });

      it('should not regenerate the artwork if it already exists', () => {
        var cover_file = helpers.fixture('covers/2087182743.jpg');

        fs.appendFileSync(cover_file, 'i am a cover lulz', 'utf-8');
        this.library.get([helpers.fixture('tagged.mp3')], cover_folder);

        assert.equal(fs.readFileSync(cover_file, 'utf-8'), 'i am a cover lulz');
      });
    });

    describe('with a callback', () => {
      it('should execute the given function each time a file has been processed', () => {
        this.library.get([helpers.fixture('without_cover.mp3')], '', (index, length) => {
          assert.equal(index, 1);
          assert.equal(length, 1);
        });
      });
    });

    describe('with an existing library', () => {
      it('should keep the existing records', () => {
        var artists;

        this.library.get([helpers.fixture('tagged.mp3')])

        artists = Object.keys(this.library.artists);
        assert.equal(artists.length, 1);

        this.library.get([helpers.fixture('without_cover.mp3')]);

        artists = Object.keys(this.library.artists);
        assert.equal(artists.length, 2);
      });
    });

    describe('with an existing library and a callback', () => {
      it('should update the library and call the given function', () => {
        this.library.get([helpers.fixture('tagged.mp3')]);
        this.library.get([helpers.fixture('without_cover.mp3')], (index, total) => {
          assert.equal(index, 1);
          assert.equal(total, 1);
        });

        assert.equal(this.library.singles.length, 1);
        assert.equal(this.library.singles[0].title, 'Strandfeest');
      })
    });
  });
});
