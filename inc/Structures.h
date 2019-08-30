#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

#include <glm/glm.hpp>
#include <vector>
#include <oPRCFile.h>
#include <sstream>
#include <algorithm>

namespace pdf3d
{
	// Mesh object data
	struct MeshObjectData
	{
		bool Assembly;
		PRCmaterial Material;
		std::string Name;
		std::vector<glm::dvec3> Vertices;
		std::vector<glm::uvec3> Indices;
		std::vector<glm::dvec3> Normals;
		glm::dvec3	BBox[2];
		std::vector<MeshObjectData> Childrens;

	};

	// Default material
	const PRCmaterial DEFAULT_MATERIAL = PRCmaterial(
		RGBAColour(1.0, 1.0, 1.0),
		RGBAColour(0.5, 0.5, 0.5),
		RGBAColour(0.0, 0.0, 0.0),
		RGBAColour(0.45, 0.45, 0.45),
		1.0,
		0.5
	);

	struct PRC_FileStream
	{
		std::string			m_DefaultName;
		glm::dvec3			m_vBoundingBox[2];
		std::vector<char>	m_vBuffer;

		PRC_FileStream();

		void SetBuffer(const std::ostringstream& stream);
		void SetBoundingBox(const glm::dvec3& min_bbox, const glm::dvec3& max_bbox);
		void SetMinMaxBoundingBox(const glm::dvec3& min_bbox, const glm::dvec3& max_bbox);
	};

};

#endif