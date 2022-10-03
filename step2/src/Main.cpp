
/*

Please write small program in C++ to do the following:
	a. Query for installed OS patches
	b. Query for general system information
	c. Feel free to add any functionality you feel will showcase your abilities.

You are free to use whatever compiler/platform you are most comfortable with. You may also use an online C++ compiler such as http://cpp.sh/ if you wish,
if you do so please be sure to make a note of it as a comment in your code.

We expect your program to be able to run across Windows, Linux and Mac.

At the top of you main function, please add a paragraph that explains what developer driven validation you took and what process you would expect to take if we were to bring this code to production.
If you are unable to confirm compilation and execution on any particular OS, please state so in the first scentence of this paragraph.


*/


/*

ANSWERS:

a. Query for installed OS patches - this question was developed, but i just got installed patches from windows, i did not get thirdparty updates
b. Query for general system information was not developed

This code was tested on Windows and we can see the result in Result file.


*/



#include <Windows.h>
#include <iostream>
#include <atlbase.h>
#include <Wuapi.h>
#include <wuerror.h>
#include <list>
#include <fstream>
#include <MsXml.h>
#include "atlbase.h"
#include "atlstr.h"
#include "comutil.h"
#include "vector"

class PatchItem
{
private:
	//Member Variables
	std::string name;
	std::string code;
	std::string status;
	std::vector<std::string> bundledUpdate;

public:

	PatchItem()
	{

	}

	virtual ~PatchItem()
	{

	}

	std::string getName()
	{
		return name;
	}

	std::string getDescription()
	{
		return code;
	}

	std::string getStatus()
	{
		return status;
	}

	void setName(std::string name) {
		name = name;
	}

	void setCode(std::string code) {
		code = code;
	}

	void setStatus(std::string status) {
		status = status;
	}

	void setBundledUpdate(std::string item) {
		bundledUpdate.push_back(item);

	}

	const std::vector<std::string>& getBundledUpdate() const {

		return bundledUpdate;
	}

};

bool queryWindowsForInstalledPatches(std::vector<PatchItem>& patchList)
{
	try
	{
		//PatchItem* patch = new PatchItem();

		std::string name, code, status, test4, test5, bundledUpdatesItem;
		std::vector<std::string> bundledUpdates;

		HRESULT hr = S_OK;
		hr = CoInitialize(NULL);

		IUpdateSession* iUpdate = nullptr;
		IUpdateSearcher* searcher = nullptr;
		ISearchResult* results = nullptr;
		BSTR criteria = SysAllocString(L"IsInstalled=1 or IsHidden=1 or IsPresent=1");

		hr = CoCreateInstance(CLSID_UpdateSession, NULL, CLSCTX_INPROC_SERVER, IID_IUpdateSession, (LPVOID*)&iUpdate);
		hr = iUpdate->CreateUpdateSearcher(&searcher);

		std::wcout << L"Searching for updates ..." << std::endl;
		hr = searcher->Search(criteria, &results);
		SysFreeString(criteria);

		switch (hr)
		{
		case S_OK:
			std::wcout << L"List of applicable items on the machine:" << std::endl;
			break;
		case WU_E_LEGACYSERVER:
			std::wcout << L"No server selection enabled" << std::endl;
			return 0;
		case WU_E_INVALID_CRITERIA:
			std::wcout << L"Invalid search criteria" << std::endl;
			return 0;
		}

		IUpdateCollection *updateList;
		IUpdate *updateItem;
		LONG updateSize;
		LONG totalKB = 0;


		results->get_Updates(&updateList);
		updateList->get_Count(&updateSize);

		if (updateSize == 0)
		{
			std::wcout << L"No updates found" << std::endl;
		}

		std::ofstream outputFile;
		outputFile.open("C:\\Test\\test.txt", std::ios::out);

		for (LONG i = 0; i < updateSize; i++)
		{
			IStringCollection *KBCollection;
			BSTR updateName;
			LONG KBCount;
			updateList->get_Item(i, &updateItem);
			updateItem->get_Title(&updateName);
			USES_CONVERSION;
			outputFile << W2A(CString(updateName)) << " --- ";

			//patch->setName(W2A(CString(updateName)));


			updateItem->get_KBArticleIDs(&KBCollection);
			KBCollection->get_Count(&KBCount);
			for (int i = 0; i < KBCount; i++)
			{
				BSTR KBValue;
				totalKB += 1;
				KBCollection->get_Item(i, &KBValue);
				USES_CONVERSION;
				outputFile << W2A(CString("KB")) << W2A(CString(KBValue)) << std::endl;
				code += "KB";
				code += W2A(CString(KBValue));
				//patch->setCode(code);
			}

			IUpdateCollection *updtCollection;
			LONG updtBundledCount;

			//check if installed
			VARIANT_BOOL isinstalled;
			updateItem->get_IsInstalled(&isinstalled);
			if (isinstalled) {
				outputFile << " installed" << std::endl;
				status = "installed";

				//patch->setStatus(status);
			}

			//Retrieve the bundled updates
			outputFile << W2A(CString("  Bundled Updates : "));
			updateItem->get_BundledUpdates(&updtCollection);
			hr = updtCollection->get_Count(&updtBundledCount);
			if ((updtBundledCount > 0) && (hr == S_OK))
			{
				for (LONG j = 0; j < updtBundledCount; j++)
				{
					IUpdate *bundledUpdateItem;

					updtCollection->get_Item(j, &bundledUpdateItem);

					bundledUpdateItem->get_Title(&updateName);
					USES_CONVERSION;
					outputFile << W2A(CString("    ")) << W2A(CString(updateName)) << " - ";
					bundledUpdatesItem = W2A(CString(updateName));
					bundledUpdatesItem += " - ";

					updateItem->get_KBArticleIDs(&KBCollection);
					KBCollection->get_Count(&KBCount);
					for (int i = 0; i < KBCount; i++)
					{
						BSTR KBValue;
						totalKB += 1;
						KBCollection->get_Item(i, &KBValue);
						outputFile << W2A(CString("KB")) << W2A(CString(KBValue)) << std::endl;
						bundledUpdatesItem += "KB";
						bundledUpdatesItem += W2A(CString(KBValue));
					}

					//patch->setBundledUpdate(bundledUpdatesItem);

				}

			}
			outputFile << std::endl;

			//patchList.emplace_back(patch);

		}
		std::wcout << "Total KBs : " << totalKB << std::endl;
		outputFile.close();
		::CoUninitialize();
	}
	catch (const std::exception & ex)
	{
		std::cout << ex.what();
		::CoUninitialize();
	}


	return 0;
}



int main()
{

	std::vector<PatchItem> patchList;
	queryWindowsForInstalledPatches(patchList);

	/*for (unsigned int i = 0; i < patchList.size(); i++)
	{
		std::cout << "Name: " << patchList[i].getName() << std::endl;
		std::cout << "Description: " << patchList[i].getDescription() << std::endl;
		std::cout << "Status: " << patchList[i].getStatus() << std::endl;
		for (const auto& p : patchList[i].getBundledUpdate())
		{
			std::cout << p << "\n";
		}
		std::cout << std::endl;
	}*/

	return 0;
}