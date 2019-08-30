#include "StepLoader.h"



namespace pdf3d
{

	StepLoader::StepLoader() :
		m_pOcafApp(new MyOcafApplication)
	{
		m_bInitialized = false;
	}

	StepLoader::StepLoader(const std::string& path) :
		m_pOcafApp(new MyOcafApplication)
	{
		m_bInitialized = false;
		LoadFromFile(path);
	}

	bool StepLoader::LoadFromFile(const std::string& path)
	{
		Handle(TDocStd_Document) doc;
		Handle(XCAFDoc_ShapeTool) myAssembly;

		std::string validatePath = path;
		std::transform(validatePath.begin(), validatePath.end(), validatePath.begin(), ::toupper);


		if (validatePath.find(".STP") == std::string::npos && validatePath.find(".STEP") == std::string::npos)
		{
			std::cout << "Bad path: not find extension .STEP and .STP in path" << std::endl;
			return false;
		}

		if (m_bInitialized)
		{
			auto ptr = m_Reader.Reader();
			ptr.ClearShapes();

			m_vObjects.clear();
			m_bInitialized = false;
		}

		// set property names and color.
		m_Reader.SetColorMode(true);
		m_Reader.SetNameMode(true);
		m_Reader.SetLayerMode(true);

		// read file from path.
		auto status = m_Reader.ReadFile(path.c_str());

		// check status.
		if (status == IFSelect_RetError || status == IFSelect_RetVoid || status == IFSelect_RetFail)
		{
			std::cout << "Read file error. Check path file and extension." << endl;
			return false;
		}

		// create application need to create new document.
		m_pOcafApp->NewDocument("MDTV-XCAF", doc);

		// transfer to doc.
		m_Reader.Transfer(doc);

		// Get all labels
		myAssembly = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
		myAssembly->GetShapes(m_LabelSequence);

		// set initialized flag.
		m_bInitialized = true;

		return true;
	}

	bool StepLoader::LoadAssembly(MeshObjectData* parent, const TDF_Label& lbl)
	{
		TopoDS_Shape shape;
		std::string name;
		Handle(TDataStd_Name) N;
		bool status = false;

		// Get shape
		XCAFDoc_ShapeTool::GetShape(lbl, shape);

		// Really it is assembly?
		if (!XCAFDoc_ShapeTool::IsAssembly(lbl)) {
			return false;
		}

		// Get name.
		// find name attribute.
		status = lbl.FindAttribute(TDataStd_Name::GetID(), N);

		if(status) {
			std::wstring wname = N->Get().ToWideString();
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

			name = converter.to_bytes(wname);
		}
		else {
			std::cout << "Get assembly name error" << std::endl;
			return false;
		}

		// Create mesh.
		// root if ptr == null.

		if (!parent)
		{
			m_vObjects.push_back(MeshObjectData());
			auto& mesh = m_vObjects.back();
			mesh.Name = name;
			mesh.Assembly = true;


			// bounding box calculate.
			Bnd_Box bnd_box;
			BRepBndLib::Add(shape, bnd_box);
			bnd_box.Get(mesh.BBox[0][0], mesh.BBox[0][1], mesh.BBox[0][2], mesh.BBox[1][0], mesh.BBox[1][1], mesh.BBox[1][2]);
		}
		else {
			parent->Childrens.push_back(MeshObjectData());
			auto& mesh = parent->Childrens.back();
			mesh.Name = name;
			mesh.Assembly = true;

			// bounding box calculate.
			Bnd_Box bnd_box;
			BRepBndLib::Add(shape, bnd_box);
			bnd_box.Get(mesh.BBox[0][0], mesh.BBox[0][1], mesh.BBox[0][2], mesh.BBox[1][0], mesh.BBox[1][1], mesh.BBox[1][2]);
		}


		TDF_ChildIterator childIter = TDF_ChildIterator(lbl);
		childIter.Initialize(lbl, true);
		for (; childIter.More(); childIter.Next()) {
			TDF_Label childLabel = childIter.Value();
			if (XCAFDoc_ShapeTool::IsAssembly(childLabel)) {
				LoadAssembly(parent ? &parent->Childrens.back() : &m_vObjects.back(), childLabel);
			}
			else if (XCAFDoc_ShapeTool::IsShape(childLabel)) {
				LoadMesh(parent ? &parent->Childrens.back() : &m_vObjects.back(), childLabel);
			}
		}

		return true;
	}

	const double StepLoader::OptimalFaceDeflection(const TopoDS_Shape& shape)
	{
		Bnd_Box bndBox;
		BRepBndLib::Add(shape, bndBox);
		if (!bndBox.IsVoid())
		{
			double xmin, ymin, zmin, xmax, ymax, zmax;
			bndBox.Get(xmin, ymin, zmin, xmax, ymax, zmax);
			return std::max(xmax - xmin, std::max(ymax - ymin, zmax - zmin)) *
				1e-3 * 4;
			// Note : 1e-3 is the default DeviationCoefficient() of a drawer.
		}
		else
			return 5000000.; // Default MaximalChordialDeviation() of a drawer.
	}


	const double StepLoader::OptimalEdgeDeflection(const TopoDS_Shape& shape)
	{
		return std::min(OptimalFaceDeflection(shape), 0.1);
	}


	bool StepLoader::LoadMesh(MeshObjectData* parent, const TDF_Label& lbl)
	{
		MeshObjectData*			ptr;
		TopoDS_Shape			shape;
		Handle(TDataStd_Name)	N;
		bool					status = false;
		Quantity_Color			color;
		PRCmaterial				meshMaterial = DEFAULT_MATERIAL;


		// Add new mesh data.
		if (parent)
		{
			parent->Childrens.push_back(MeshObjectData());
			ptr = &parent->Childrens.back();
		}
		else {
			m_vObjects.push_back(MeshObjectData());
			ptr = &m_vObjects.back();
		}

		// Get reference object.
		MeshObjectData& meshData = *ptr;
		auto& vertices = meshData.Vertices;
		auto& indices = meshData.Indices;
		auto& normals = meshData.Normals;
		auto& bbox = meshData.BBox;
		auto& name = meshData.Name;

		meshData.Material = meshMaterial;
		meshData.Assembly = false;


		// find name attribute.
		status = lbl.FindAttribute(TDataStd_Name::GetID(), N);

		if (status)
		{
			std::wstring wname = N->Get().ToWideString();
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

			name = converter.to_bytes(wname);
		}
		else {
			if (parent) {

				name = parent->Name + "[" + std::to_string(parent->Childrens.size() - 1) + "]";
			}
			else {
				name = "Default";
			}
		}

		// Detect subshapes.
		bool Subshapes = false;

		TDF_ChildIterator childIter = TDF_ChildIterator(lbl);
		childIter.Initialize(lbl, true);
		for (; childIter.More(); childIter.Next()) {
			TDF_Label childLabel = childIter.Value();
			if (XCAFDoc_ShapeTool::IsShape(childLabel)) {
				LoadMesh(ptr, childLabel);
				Subshapes = true;
			}
		}

		// Get shape
		XCAFDoc_ShapeTool::GetShape(lbl, shape);

		// Check shape type.
		int shapeType = shape.ShapeType();

		// triangulization shape.
		try
		{
			double linearDeflection = StepLoader::OptimalEdgeDeflection(shape);
			BRepMesh_IncrementalMesh(shape, linearDeflection, true, linearDeflection, false, false);

		}
		catch (Standard_Failure)
		{
			return false;
		}

		// Get ambient color.
		Handle_XCAFDoc_ColorTool colorTool = XCAFDoc_ColorTool::Set(lbl);
		if (colorTool->IsSet(shape, XCAFDoc_ColorGen)) {
			colorTool->GetColor(shape, XCAFDoc_ColorGen, color);
			meshMaterial.ambient.R = color.Red();
			meshMaterial.ambient.G = color.Green();
			meshMaterial.ambient.B = color.Blue();
		}

		// get bounding box.
		Bnd_Box bnd_box;
		BRepBndLib::Add(shape, bnd_box);
		bnd_box.Get(bbox[0][0], bbox[0][1], bbox[0][2], bbox[1][0], bbox[1][1], bbox[1][2]);

		// subshapes ? this is all needed.
		if (Subshapes)
			return false;

		// get vertex from OpenCascade Mesh.
		Standard_Integer aIndex = 1, nbNodes = 0;
		TColgp_SequenceOfPnt aPoints, aPoints1;
		uint32_t idxCounter = 0;
		uint32_t idxNormalsCounter = 0;
		for (TopExp_Explorer aExpFace(shape, TopAbs_FACE); aExpFace.More(); aExpFace.Next())
		{
			// Get face information and orientation face.
			TopoDS_Face aFace = TopoDS::Face(aExpFace.Current());
			TopAbs_Orientation faceOrientation = aFace.Orientation();


			TopLoc_Location aLocation;
			Handle(Poly_Triangulation) aTr = BRep_Tool::Triangulation(aFace, aLocation);


			if (!aTr.IsNull())
			{
				const TColgp_Array1OfPnt& aNodes = aTr->Nodes();
				const Poly_Array1OfTriangle& triangles = aTr->Triangles();
				const TColgp_Array1OfPnt2d& uvNodes = aTr->UVNodes();


				TColgp_Array1OfPnt aPoints(1, aNodes.Length());
				for (Standard_Integer j = 1; j < aNodes.Length() + 1; j++)
					aPoints(j) = aNodes(j).Transformed(aLocation);


				Standard_Integer nnn = aTr->NbTriangles();
				Standard_Integer nt, n1, n2, n3;

				// Get vertices & indices & normals & normals indices per triangle.
				for (nt = 1; nt < nnn + 1; nt++)
				{
					triangles(nt).Get(n1, n2, n3);
					gp_Pnt aPnt1 = aPoints(n1);
					gp_Pnt aPnt2 = aPoints(n2);
					gp_Pnt aPnt3 = aPoints(n3);


					gp_Pnt2d uv1 = uvNodes(n1);
					gp_Pnt2d uv2 = uvNodes(n2);
					gp_Pnt2d uv3 = uvNodes(n3);


					// Get triangle points.
					glm::dvec3 p1, p2, p3;
					if (faceOrientation == TopAbs_Orientation::TopAbs_FORWARD)
					{
						p1 = glm::dvec3(aPnt1.X(), aPnt1.Y(), aPnt1.Z());
						p2 = glm::dvec3(aPnt2.X(), aPnt2.Y(), aPnt2.Z());
						p3 = glm::dvec3(aPnt3.X(), aPnt3.Y(), aPnt3.Z());
					}
					else
					{
						p3 = glm::dvec3(aPnt1.X(), aPnt1.Y(), aPnt1.Z());
						p2 = glm::dvec3(aPnt2.X(), aPnt2.Y(), aPnt2.Z());
						p1 = glm::dvec3(aPnt3.X(), aPnt3.Y(), aPnt3.Z());
					}


					// check duplicate vertices.
					auto res_p1 = findInVector<glm::dvec3>(vertices, p1);
					auto res_p2 = findInVector<glm::dvec3>(vertices, p2);
					auto res_p3 = findInVector<glm::dvec3>(vertices, p3);

					// calculate direction and normals.
					glm::dvec3 dir1 = p2 - p1;
					glm::dvec3 dir2 = p3 - p1;
					glm::dvec3 normal = glm::cross(dir1, dir2);


					// add normals if no duplicated.
					if (!res_p1.first) normals.push_back(normal);
					if (!res_p2.first) normals.push_back(normal);
					if (!res_p3.first) normals.push_back(normal);

					// add vertices when is no duplicated.
					if (!res_p1.first) vertices.push_back(p1);
					if (!res_p2.first) vertices.push_back(p2);
					if (!res_p3.first) vertices.push_back(p3);

					// add indices.
					glm::uvec3 index;
					index[0] = res_p1.first ? res_p1.second : idxCounter++;
					index[1] = res_p2.first ? res_p2.second : idxCounter++;
					index[2] = res_p3.first ? res_p3.second : idxCounter++;

					indices.push_back(index);
				}
			}
		}


		return true;
	}


	bool StepLoader::ReadAllSolidMesh()
	{
		// Globals:
		TopoDS_Shape shape;
		TDF_Label lbl;
		bool StepWithAssembly = false;
		std::vector<TDF_Label> assemblyLabels;

		// Not initialzed;
		if (!m_bInitialized)
			return false;

		// Search assembly.
		for (int i = 1; i <= m_LabelSequence.Length(); i++) {
			lbl = m_LabelSequence.Value(i);
			if (XCAFDoc_ShapeTool::IsAssembly(lbl)) {
				// detect assembly file.
				assemblyLabels.push_back(lbl);
				StepWithAssembly = true;
			}
		}

		// 1. When detect assembly load -> load all solid using assemblies entry.
		if (StepWithAssembly) {
			for (auto ass_lbl : assemblyLabels) {
				if (!LoadAssembly(NULL, ass_lbl)) {
					std::cout << "Failed load assemebly" << std::endl;
					return false;
				}

			}
		}
		// 2. None detect assembly -> load all solid to root.
		else {
			for (int i = 1; i <= m_LabelSequence.Length(); i++) {
				lbl = m_LabelSequence.Value(i);
				LoadMesh(NULL, lbl);
			}
		}
				return true;
	}

}