#ifndef __STEP_LOADER_H__
#define __STEP_LOADER_H__

// Includes:
#include <vector>
#include <array>
#include <string>
#include <istream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <BRepBndLib.hxx>
#include <Bnd_Box.hxx>
#include <glm/gtx/rotate_vector.hpp>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <STEPControl_Reader.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <TDocStd_Document.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <TDataStd_Name.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <Quantity_Color.hxx>
#include <TDF_ChildIterator.hxx>
#include <locale>
#include <codecvt>

#include "Utils.h"
#include "Structures.h"
#include "MyOcafApplication.h"

namespace pdf3d
{
	// Step loader class
	class StepLoader
	{
	public:
		StepLoader();
		StepLoader(const std::string& path);
		~StepLoader() = default;

		bool LoadFromFile(const std::string& path);
		bool ReadAllSolidMesh();

		inline const auto& getMeshObjectData() const {
			return m_vObjects;
		}

		inline bool Initialized() {
			return m_bInitialized;
		}

	private:
		bool LoadAssembly(MeshObjectData* parent, const TDF_Label& label);
		bool LoadMesh(MeshObjectData* parent, const TDF_Label& label);
		const static double OptimalFaceDeflection(const TopoDS_Shape& shape);
		const static double OptimalEdgeDeflection(const TopoDS_Shape& shape);

		bool										 m_bInitialized;
		std::vector<MeshObjectData>					 m_vObjects;
		STEPCAFControl_Reader						 m_Reader;
		STEPControl_Reader							 m_BasicReader;
		TDF_LabelSequence							 m_LabelSequence;
		std::shared_ptr<MyOcafApplication>			 m_pOcafApp;
	};

}

#endif