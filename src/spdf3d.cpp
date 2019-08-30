#include "StepLoader.h"
#include "ArgumentParser.h"
#include "PDF3D.h"
#include <string>


int main(int argc, char* argv[])
{
	std::string stepfilePath = "";
	std::string pdf3DfilePath = "";

	// initialize argument parser.
	ArgumentParser parser(argc, argv);

	// load values from parser.
	stepfilePath = parser.GetArgumentValue("-i");
	pdf3DfilePath = parser.GetArgumentValue("-o");

	if(stepfilePath.empty() || parser.IsOptionArgument("-i")) {
		std::cout << "Input file path is empty" << std::endl;
		return -1;
	}

	if(pdf3DfilePath.empty() || parser.IsOptionArgument("-o")) {
		std::cout << "Output file path is empty" << std::endl;
		return -1;
	}

	pdf3d::StepLoader loader(stepfilePath);

	if(!loader.Initialized() || !loader.ReadAllSolidMesh())
	{
		std::cout << "Failed load step file: " << stepfilePath << std::endl;
		return -2;
	}

	pdf3d::PDF3D pdf;
	pdf.AddStepFile(loader);
	pdf.SaveFile(pdf3DfilePath);

	return 0;
}