#include "PDF3D.h"

namespace pdf3d
{

	void PDF3D::ClearStreamBuffer()
	{
		m_vEmbeddedStream.clear();
	}


	void PDF3D::AddMesh(MeshObjectData& mesh, oPRCFile& fileHandle, PRC_FileStream& fileStream)
	{
		if (fileStream.m_DefaultName.empty() && !mesh.Name.empty()) {
			fileStream.m_DefaultName = mesh.Name;
		}

		fileStream.SetMinMaxBoundingBox(mesh.BBox[0], mesh.BBox[1]);

		if (mesh.Childrens.size() > 0) {
			fileHandle.begingroup(mesh.Name.c_str());

			for (int i = 0; i < mesh.Childrens.size(); i++) {
				AddMesh(mesh.Childrens[i], fileHandle, fileStream);
			}

			fileHandle.endgroup();
		}
		else {
			fileHandle.begingroup(mesh.Name.c_str());

			// create pointer structures.
			double(*pVerticesData)[3] = (double(*)[3]) & mesh.Vertices.data()[0];
			unsigned int(*pIndicesData)[3] = (unsigned int(*)[3]) & mesh.Indices.data()[0];
			double(*pNormals)[3] = (double(*)[3]) & mesh.Normals.data()[0];

			// Add triangles.
			fileHandle.addTriangles(mesh.Vertices.size(), pVerticesData, mesh.Indices.size(), pIndicesData, mesh.Material, mesh.Normals.size(), pNormals, 0, 0, NULL, NULL, 0, NULL, NULL, 0, NULL, NULL, 0.1f);


			fileHandle.endgroup();
		}
	}


	bool PDF3D::AddStepFile(const StepLoader& loader)
	{
		auto& meshesData = loader.getMeshObjectData();

		if (meshesData.empty())
			return false;

		// create prc file and initialize memory buffer.
		std::ostringstream buffer;
		oPRCFile aPrcFile(buffer);
		PRC_FileStream fileStream;


		// create prc for each shape.
		for (int i = 0; i < meshesData.size(); i++)
			AddMesh((MeshObjectData&)meshesData[i], aPrcFile, fileStream);

		// Create prc file and save.
		if (aPrcFile.finish())
			std::cout << "Generated PRC" << std::endl;
		else
		{
			std::cout << "Failed generate prc" << std::endl;
			return false;
		}

		// fill buffer.
		fileStream.SetBuffer(buffer);

		// add buffer to collection.
		m_vEmbeddedStream.push_back(fileStream);

		return true;
	}

	void PDF3D::SaveFile(const std::string& path, int width, int height)
	{
		if (path.empty()) {
			printf("PDF3D::SaveFile: empty path\n");
			return;
		}

		// embed prc file to pdf.
		HPDF_Doc aPdfDoc = HPDF_New(NULL, NULL);
		if (!aPdfDoc)
		{
			printf("error: cannot create PdfDoc object");
			return;
		}

		// create pdf page.
		HPDF_Rect aPageRect = { 0, 0, width, height };
		HPDF_Page aPage = HPDF_AddPage(aPdfDoc);
		char viewName[256];

		// set view rectangle.
		HPDF_Page_SetWidth(aPage, width);
		HPDF_Page_SetHeight(aPage, height);

		// create 3D View & embedded stream u3d.
		for (int i = 0; i < m_vEmbeddedStream.size(); i++) {
			// load only is not empty.
			if (m_vEmbeddedStream[i].m_vBuffer.size() > 0) {
				// laod u3d file.
				HPDF_U3D u3d_obj = HPDF_LoadU3DFromMem(aPdfDoc, (const uint8_t*)m_vEmbeddedStream[i].m_vBuffer.data(), m_vEmbeddedStream[i].m_vBuffer.size());

				// create annotation.
				HPDF_Annotation annot = HPDF_Page_Create3DAnnot(aPage, aPageRect, u3d_obj);

				// create view name.
				sprintf_s(viewName, "%sView", m_vEmbeddedStream[i].m_DefaultName.c_str());

				// create 3d view.
				HPDF_Dict view = HPDF_Page_Create3DView(aPage, u3d_obj, annot, viewName);

				// calculate zoom distance.
				float cameraDistance = 0.1f; // Constant factor
				glm::dvec3 objectSizes = m_vEmbeddedStream[i].m_vBoundingBox[1] - m_vEmbeddedStream[i].m_vBoundingBox[0];
				float objectSize = std::max({ objectSizes.x, objectSizes.y, objectSizes.z });
				float cameraView = 2.0f * tan(glm::radians(0.5f) * ((float)aPageRect.right / (float)aPageRect.top)); // Visible height 1 meter in front
				float distance = cameraDistance * objectSize / cameraView; // Combined wanted distance from the object
				distance += 0.5f * objectSize; // Estimated offset from the center to the outside of the object

				// calculate rotation matrix.
				double rotX = 45;
				double rotY = 45;
				double rotZ = 0;

				glm::dvec3 c2c(0.0, 0.0, 1.0);
				c2c = glm::rotate(c2c, rotX, glm::dvec3(1.0, 0.0, 0.0));
				c2c = glm::rotate(c2c, rotY, glm::dvec3(0.0, 1.0, 0.0));
				c2c = glm::rotate(c2c, rotZ, glm::dvec3(0.0, 0.0, 1.0));

				// configure 3d view.
				HPDF_3DView_SetLighting(view, "Headlamp");
				HPDF_3DView_SetCamera(view, 0, 0, 0, c2c.x, c2c.y, c2c.z, distance, 0);
				HPDF_3DView_SetBackgroundColor(view, 0.9, 0.9, 0.9);

				// Other nodes:
				HPDF_Dict renderModeDict = HPDF_Dict_New(view->mmgr);
				HPDF_Dict_AddName(renderModeDict, "Type", "3DRenderMode");
				HPDF_Dict_AddName(renderModeDict, "Subtype", "SolidOutline");
				HPDF_Dict_Add(view, "RM", renderModeDict);


				HPDF_U3D_Add3DView(u3d_obj, view);
				HPDF_U3D_SetDefault3DView(u3d_obj, viewName);
			}
		}

		// save document pdf.
		HPDF_SaveToFile(aPdfDoc, path.c_str());

		// free document pdf.
		HPDF_Free(aPdfDoc);

		// clear buffer.
		ClearStreamBuffer();
	}
}