{
    "targets": [
        {
            "target_name": "ManipulateMyMemory",
            "sources": [
                "src/main.cc",
                "src/windows_extension.cc",
                "src/windows_extension.h"
            ],
            "include_dirs" : [
                "<!(node -e \"require('nan')\")"
            ]
        }
    ]
}
