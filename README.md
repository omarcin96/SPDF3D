# SPDF3D - Step to PDF3D
> This program convert .step format files to .pdf with 3d models.

## Table of content
- [Usage](#usage)
- [Motivation](#motivation)
- [Compile](#compile)
- [License](#license)

## Usage
```
SPDF3D.exe -i <input_step_path> -o <output_pdf_path>
```
Where:
i - path to load step file.
o - save path pdf 3d.

## Motivation
I created this program because, i couldn't find free and opensource converter. I used this program to convert lots of files in company.

## Compile
### Dependicies:
Before compile SPDF3D project. You need compile this dependicies:
- [OpenCascade] - Modeling library
- [Libharu] - Free pdf library
- [GLM] - 3D Mathematics library
- [libPRC] - PRC file format library.

### Using CMake
You should run cmake gui and configure necessary variables. Optionaly, you can using terminal:
```
cmake <path_to_source> \
	-GLM_ROOT_DIR=<path> \
	-HARU_INCLUDES_DIR=<path> \
	-HARU_DBG_LIB_DIR=<path> \
	-HARU_OPT_LIB_DIR=<path> \
	-PRC_INCLUDES_DIR=<path> \
	-PRC_DBG_LIBS_DIR=<path> \
	-PRC_OPT_LIBS_DIR=<path> \
	-OCS_INCLUDES_DIR=<path> \
	-OCS_DBG_LIBS_DIR=<path> \
	-OCS_OPT_LIBS_DIR=<path> \
	-ZLIB_DBG_LIBS_DIR=<path> \
	-ZLIB_OPT_LIBS_DIR=<path> \
```	
## License:
This program use MIT license.

[OpenCascade]: https://github.com/tpaviot/oce
[Libharu]: https://github.com/libharu/libharu
[GLM]: https://glm.g-truc.net
[libPRC]: https://github.com/XenonofArcticus/libPRC
