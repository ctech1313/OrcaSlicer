# OrcaSlicer Windows Build Guide

## Prerequisites

Before compiling OrcaSlicer, you'll need to install the following tools:

- Visual Studio 2019
- CMake (version 3.14 or later)
- Git
- Git LFS
- Strawberry Perl

## Repository Setup

1. Fork the OrcaSlicer GitHub repository
2. Clone your fork locally:
```bash
git clone https://github.com/YOUR_USERNAME/OrcaSlicer.git
cd OrcaSlicer
git lfs pull
```

## Building OrcaSlicer

### Windows 64-bit Build Process

1. **Open Developer Command Prompt**
   - Launch the `x64 Native Tools Command Prompt for VS 2019`

2. **Run Build Script**
```bash
build_release.bat
```

## Running OrcaSlicer

After successful compilation:
- Navigate to the build directory
- Execute `OrcaSlicer.exe`

## Additional Configuration

If you're using Klipper, add the following to your `printer.cfg`:
```text
# Enable object exclusion
[exclude_object]

# Enable arcs support
[gcode_arcs]
resolution: 0.1
```

## License

OrcaSlicer is licensed under the GNU Affero General Public License, version 3. This ensures that any derivative work must also be open-source under the same license terms.

## Troubleshooting

If you encounter issues during the build process:

1. Verify all prerequisites are correctly installed
2. Ensure Visual Studio 2019 is properly configured
3. Check that all paths are correctly set in your environment variables
4. Make sure Git LFS has properly pulled all necessary files

## Build Directory Structure

After a successful build, your directory structure should look like:
```
build/
├── Release/
│   ├── OrcaSlicer.exe
│   ├── resources/
│   └── dependencies/
└── intermediate/
```
