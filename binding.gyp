{
    "targets": [
    ],
    "conditions": [
        [
            "OS=='win'", {
                "targets": [
                    {
                        "target_name": "Win32ManipulateMyMemory",
                        "sources": [
                            "src/win32/main.cpp",
                            "src/win32/close_handle.cpp",
                            "src/win32/open_process.cpp",
                            "src/win32/read_process_memory.cpp",
                            "src/win32/read_process_memory.hpp",
                            "src/win32/write_process_memory.cpp",
                            "src/win32/write_process_memory.hpp",
                            "src/win32/find_window.cpp",
                            "src/win32/get_window_thread_process_id.cpp",
                            "src/win32/shared.cpp",
                            "src/win32/main.hpp",
                            "src/win32/close_handle.hpp",
                            "src/win32/open_process.hpp",
                            "src/win32/find_window.hpp",
                            "src/win32/get_window_thread_process_id.hpp",
                            "src/win32/shared.hpp",
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
                        "target_name": "LinuxManipulateMemory",
                        "sources": [
                            "src/linux/main.cpp",
                            "src/linux/main.hpp",
                            "src/linux/process_vm_readv.cpp",
                            "src/linux/process_vm_readv.hpp",
                            "src/linux/shared.hpp",

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
