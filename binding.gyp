{
  "targets": [
    {
      "target_name": "tagging",
      "sources": [ "src/get.cc", "src/image_file.cc", "src/init.cc", "src/set.cc", "src/utils.cc" ],
      "include_dirs": ['deps/include'],
      "conditions": [
        ['OS=="mac"', {
          "libraries": ['../deps/lib/libtag-darwin.a'],
        }],
        ['OS=="linux"', {
          "architecture": "ia32",
          "libraries": ['../deps/lib/libtag-linux-x86.a']
        }],
        ['OS=="linux"', {
          "libraries": ['../deps/lib/libtag-linux-x86_64.a']
        }]
      ]
    }
  ]
}
