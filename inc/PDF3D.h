#ifndef __PDF_3D_H__
#define __PDF_3D_H__


#include "StepLoader.h"
#include "Structures.h"
#include <string>
#include <vector>
#include <oPRCFile.h>
#include "hpdf.h"
#include <hpdf_u3d.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace pdf3d
{

	// PDF 3D Writer
	class PDF3D
	{
	public:
		// Add step file to pdf 3d.
		bool AddStepFile(const StepLoader& loader);

		// Save file pdf 3d.
		void SaveFile(const std::string& path, int width = 1024, int height = 768);

		// clear stream.
		void ClearStreamBuffer();


	protected:
		std::vector<PRC_FileStream> m_vEmbeddedStream;

	protected:
		void AddMesh(MeshObjectData& mesh, oPRCFile& fileHandle, PRC_FileStream& fileStream);
	};

};

#endif