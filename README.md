# hdcp-gen-key
`hdcp-gen-key` is a command-line tool for generating HDCP (High-bandwidth Digital Content Protection) keys for versions 1.0 through 1.4.
It utilizes Intel's Master Key Matrix for key generation.

## Features
* Generates HDCP keys for versions 1.0-1.4.
* Utilizes Intel's Master Key Matrix.
* Supports custom Key Selection Vectors (KSVs) or can generate valid random KSVs.
* Offers a wide range of output formats:
    * Human-readable text (informational, source/sink only, full details).
    * Machine-readable formats: JSON, YAML, XML, TOML.
* Can output source device keys, sink device keys, or both, optionally including the KSV and the derived HDCP shared key.

## Prerequisites
Before you begin, ensure you have the following installed:
* [Git](https://git-scm.com/)
* [CMake](https://cmake.org/) (version 3.16 or higher recommended)
* A C++ compiler that supports C++11 (GCC, Clang, MSVC)
* (Optional, for documentation) [Doxygen](https://www.doxygen.nl/)

## Building
1. Clone the repository:
    ```bash
    git clone https://github.com/SavaLione/hdcp-gen-key.git
    ```
2. Navigate to the project directory:
    ```bash
    cd hdcp-gen-key
    ```
3. Create and enter a build directory:
    ```bash
    mkdir build
    cd build
    ```
4. Configure the project with CMake:
    ```bash
    cmake ..
    ```
5. Compile the project:
    * Linux/BSD:
        ```bash
        make
        ```
    * Windows using MSBuild (typically found with Visual Studio):
        ```bash
        MSBuild.exe hdcp-gen-key.sln
        ```
        (You might need to adjust the path to `MSBuild.exe` or open the `.sln` file in Visual Studio IDE and build from there.)
    * Windows using Ninja:
        ```bash
        ninja
        ```

The executable `hdcp-gen-key` (or `hdcp-gen-key.exe` on Windows) will be located in the `build` directory (or a subdirectory like `build/Release` depending on your generator and configuration).

## Usage
After building, you can run `hdcp-gen-key` from the `build` directory.

### Examples:
Show help message:
```bash
./hdcp-gen-key --help
```

Generate keys with a randomly generated KSV and default output format (text_informational):
```bash
./hdcp-gen-key
```

Specify a Key Selection Vector (KSV) and use the default output:
```bash
./hdcp-gen-key -k 00000fffff
```

Generate only the source device key as a line of space-separated hex values, using a random KSV:
```bash
./hdcp-gen-key --out text_line_source
```

Specify a KSV and output KSV, source key, sink key, and shared key in JSON format:
```bash
./hdcp-gen-key -k 00000fffff -o json_full
```

Refer to the `-h` or `--help` output for a full list of options and output formats.

## Documentation
This project supports Doxygen for code documentation.

To generate the documentation, navigate to the project's root directory and run:
```bash
doxygen doxyfile
```

The output will be generated in the `docs/` directory.

## License
`hdcp-gen-key` is licensed under the terms of [The GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.en.html).
