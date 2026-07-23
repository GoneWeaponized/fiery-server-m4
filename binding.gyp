{
  "targets": [
    {
      "target_name": "addon-cpp",
      "sources": [
      "./addon-cpp/math/distance.cpp" ,
      "./addon-cpp/math/haversine.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "xcode_settings": { "GCC_ENABLE_CPP_EXCEPTIONS": "YES" },
      "msvs_settings": { "VCCLCompilerTool": { "ExceptionHandling": 1 } }
    }
  ]
}
