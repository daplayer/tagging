require ('./test_helper');

describe('Library', () => {
  describe('#remove', () => {
    beforeEach(() => {
      this.library = new Tagging.Library({
        artists: {
          darius: {
            albums: { Velour: [
              {id: 'Road Trip'},
              {id: 'Dans Tes Yeux'},
            ]},
            singles: [{id: 'Pyor'}]
          }
        },
        singles: [{id: 'Strandfeest'}, {id: 'Pyor'}]
      });
    });

    it("should remove from both artist's singles and singles", () => {
      assert.equal(this.library.singles.length, 2);
      assert.equal(this.library.artists.darius.singles.length, 1);

      this.library.remove(['Pyor']);

      assert.equal(this.library.singles.length, 1);
      assert.equal(this.library.artists.darius.singles.length, 0);
    });

    it('should remove from singles', () => {
      assert.equal(this.library.singles.length, 2);

      this.library.remove(['Strandfeest']);

      assert.equal(this.library.singles.length, 1);
    });

    it('should remove from albums', () => {
      assert.equal(this.library.artists.darius.albums.Velour.length, 2);

      this.library.remove(['Road Trip']);

      assert.equal(this.library.artists.darius.albums.Velour.length, 1);
    });

    it('should remove empty albums', () => {
      assert.equal(this.library.artists.darius.albums.Velour.length, 2);

      this.library.remove(['Road Trip', 'Dans Tes Yeux']);

      assert.equal(this.library.artists.darius.albums.Velour, undefined);
    });
  });
});
