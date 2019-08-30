#ifndef __ENUMS_H__
#define __ENUMS_H__

#include <string>

namespace pdf3d
{

	// Possible render modes in pdf 3d. See "PDF32000-1:2008 Table 308 - Render Modes" if you need more information.
	enum PDF3D_RenderModes
	{
		PDF3D_RM_UNKNOWN, // error flag.
		PDF3D_RM_SOLID,
		PDF3D_RM_SOLID_WIREFRAME,
		PDF3D_RM_TRANSPARENT,
		PDF3D_RM_TRANSPARENT_WIREFRAME,
		PDF3D_RM_BOUNDINGBOX,
		PDF3D_RM_TRANSPARENT_BOUNDINGBOX,
		PDF3D_RM_TRANSPARENT_BOUNDINGBOX_OUTLINE,
		PDF3D_RM_WIREFRAME,
		PDF3D_RM_SHADED_WIREFRAME,
		PDF3D_RM_HIDDEN_WIREFRAME,
		PDF3D_RM_VERTICES,
		PDF3D_RM_SHADED_VERTICES,
		PDF3D_RM_ILLUSTRATION,
		PDF3D_RM_SOLID_OUTLINE,
		PDF3D_RM_SHADED_ILLUSTRATION,
	};

	static const std::string _PDF3D_RenderModes_StringTable[] = {
		"Unknown",
		"Solid",
		"SolidWireframe",
		"Transparent",
		"TransparentWireframe",
		"BoundingBox",
		"TransparentBoundingBox",
		"TransparentBoundingBoxOutline",
		"Wireframe",
		"ShadedWireframe",
		"HiddenWireframe",
		"Vertices",
		"ShadedVertices",
		"Illustration"
		"SolidOutline",
		"ShadedIllustration"
	};

	// Possible light scene modes in pdf 3d. See "PDF32000-1:2008 Table 310 - Lighting Schemes" if you need more information.
	enum PDF3D_LightingSchemeStyles
	{
		PDF3D_LS_UNKNOWN, // error flag
		PDF3D_LS_ARTWORK,
		PDF3D_LS_NONE,
		PDF3D_LS_WHITE,
		PDF3D_LS_DAY,
		PDF3D_LS_NIGHT,
		PDF3D_LS_HARD,
		PDF3D_LS_PRIMARY,
		PDF3D_LS_BLUE,
		PDF3D_LS_RED,
		PDF3D_LS_CUBE,
		PDF3D_LS_CAD,
		PDF3D_LS_HEADLAMP,
	};

	static const std::string _PDF3D_LightingSchemeStyles_StringTable[] = {
		"Unknown",
		"Artwork",
		"None",
		"White",
		"Day",
		"Night",
		"Hard",
		"Primary",
		"Blue",
		"Red",
		"Cube",
		"CAD",
		"HeadLamp"
	};
};

#endif