{
  "targets": [
    {
      "target_name": "tagging",
      "sources": [ "src/get.cc", "src/image_file.cc", "src/init.cc", "src/set.cc", "src/utils.cc" ],
      "include_dirs": ['deps/include'],
      "libraries": ['../deps/lib/libtag.a']
    }
  ]
}
