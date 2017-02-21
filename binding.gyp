{
    "targets": [
        {
            "target_name": "NativeExtension",
            "sources": [
                "src/native.cc"
            ],
            "include_dirs" : [
                "<!(node -e \"require('nan')\")"
            ]
        }
    ]
}
