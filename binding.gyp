{
  "targets": [
    {
      "target_name": "cryptcookies",
      "sources": [ "cryptcookies.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
