{
  "targets": [
    {
      "target_name": "cookie-handle",
      "sources": [ "cookie-handle.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
