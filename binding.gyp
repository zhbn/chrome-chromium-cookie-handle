{
  "targets": [
    {
      "target_name": "cookie_handle",
      "sources": [ "cookie-handle.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
