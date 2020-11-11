{
  "targets": [
    {
      "target_name": "cryptocookies",
      "sources": [ "cryptocookies.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
