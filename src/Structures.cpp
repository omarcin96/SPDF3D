#include "Structures.h"

namespace pdf3d
{

	PRC_FileStream::PRC_FileStream()
	{
		m_vBoundingBox[0] = m_vBoundingBox[1] = glm::dvec3(0.0f);
	}

	// Set new values.
	void PRC_FileStream::SetMinMaxBoundingBox(const glm::dvec3& min_bbox, const glm::dvec3& max_bbox) {
		m_vBoundingBox[0] = std::min(m_vBoundingBox[0], min_bbox, [](const glm::dvec3& a, const glm::dvec3& b) {
			return a[0] < b[0] && a[1] < b[1] && a[2] < b[2];
			});
		m_vBoundingBox[1] = std::max(m_vBoundingBox[1], max_bbox, [](const glm::dvec3& a, const glm::dvec3& b) {
			return a[0] < b[0] && a[1] < b[1] && a[2] < b[2];
			});
	}

	void PRC_FileStream::SetBoundingBox(const glm::dvec3& min_bbox, const glm::dvec3& max_bbox)
	{
		m_vBoundingBox[0] = min_bbox;
		m_vBoundingBox[1] = max_bbox;
	}

	void PRC_FileStream::SetBuffer(const std::ostringstream& stream)
	{
		// clear buffer.
		m_vBuffer.clear();

		// fill buffer.
		auto bufferStr = stream.str();
		std::copy(bufferStr.c_str(), bufferStr.c_str() + bufferStr.length(), std::back_inserter(m_vBuffer));
	}

}