{
    "targets": [
    ],
    "conditions": [
        [
            "OS=='win'", {
                "targets": [
                    {
                        "target_name": "ManipulateMyMemory",
                        "sources": [
                            "src/main.cpp",
                            "src/win32_close_handle.cpp",
                            "src/win32_open_process.cpp",
                            "src/win32_read_process_memory.cpp",
                            "src/win32_find_window.cpp",
                            "src/win32_get_window_thread_process_id.cpp",
                            "src/win32_shared.cpp",
                            "src/windows_extension.hpp",
                            "src/win32_close_handle.hpp",
                            "src/win32_open_process.hpp",
                            "src/win32_read_process_memory.hpp",
                            "src/win32_find_window.hpp",
                            "src/win32_get_window_thread_process_id.hpp",
                            "src/win32_shared.hpp",
                        ],
                        "include_dirs" : [
                            "<!(node -e \"require('nan')\")"
                        ]
                    }
                ]
            }
        ], [
            "OS=='linux'", {
                "targets": [
                    {
                        "target_name": "HelloWorld",
                        "sources": [
                        ],
                        "include_dirs" : [
                            "<!(node -e \"require('nan')\")"
                        ]
                    }
                ]
            }
        ]
    ]
}
