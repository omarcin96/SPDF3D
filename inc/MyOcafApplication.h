#ifndef __MY_OCAF_APPLICATION_H__
#define __MY_OCAF_APPLICATION_H__

// OpenCascade headers
#include <TDocStd_Application.hxx>
#include <TColStd_SequenceOfExtendedString.hxx>

namespace pdf3d
{

	/*!
	* \class MyOcafApplication
	* \inherit TDocStd_Application
	* \brief Class inherited from OpenCascade's TDocStd_Application. It is needed to use a TDocStd_Application that is an abstract class!
	*/
	class MyOcafApplication : public TDocStd_Application
	{
		/*!
		 * \fn void Formats(TDolStd_SequenceOfExtendedString& Formats)
		 * \brief Method to add the format of the documents to be read by the application
		 * \param Formats
		 */
		void Formats(TColStd_SequenceOfExtendedString& Formats)
		{
			Formats.Append(TCollection_ExtendedString("MyOcafApplication"));
		}

		/*!
		 * \fn Standard_CString ResourcesName()
		 * \brief method to define the name of the resource file
		 */
		Standard_CString ResourcesName()
		{
			return Standard_CString("Resources");
		}
	};

}


#endif