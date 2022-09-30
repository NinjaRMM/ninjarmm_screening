#include <Windows.h>
#include <iostream>
#include <atlbase.h>
#include <Wuapi.h>
#include <wuerror.h>
#include <list>
#include <fstream>
#include "atlbase.h"
#include "atlstr.h"
#include "comutil.h"
#include <MsXml.h>

using namespace std;


int main()
{
    try
    {
        HRESULT hr;
        hr = CoInitialize(NULL);

        IUpdateSession* iUpdate;
        IUpdateSearcher* searcher;
        ISearchResult* results;
        BSTR criteria = SysAllocString(L"IsInstalled=1 or IsHidden=1 or IsPresent=1");

        hr = CoCreateInstance(CLSID_UpdateSession, NULL, CLSCTX_INPROC_SERVER, IID_IUpdateSession, (LPVOID*)&iUpdate);
        hr = iUpdate->CreateUpdateSearcher(&searcher);

        wcout << L"Searching for updates ..." << endl;
        hr = searcher->Search(criteria, &results);
        SysFreeString(criteria);

        switch (hr)
        {
        case S_OK:
            wcout << L"List of applicable items on the machine:" << endl;
            break;
        case WU_E_LEGACYSERVER:
            wcout << L"No server selection enabled" << endl;
            return 0;
        case WU_E_INVALID_CRITERIA:
            wcout << L"Invalid search criteria" << endl;
            return 0;
        }

        IUpdateCollection* updateList;
        IUpdate* updateItem;
        LONG updateSize;
        LONG totalKB = 0;
        results->get_Updates(&updateList);
        updateList->get_Count(&updateSize);

        if (updateSize == 0)
        {
            wcout << L"No updates found" << endl;
        }

        ofstream outputFile;
        outputFile.open("C:\\temp\\est.txt", ios::out);

        for (LONG i = 0; i < updateSize; i++)
        {
            IStringCollection* KBCollection;
            BSTR updateName;
            LONG KBCount;
            updateList->get_Item(i, &updateItem);


            updateList->get_Item(i, &updateItem);
            updateItem->get_Title(&updateName);
            USES_CONVERSION;
            outputFile << W2A(CString(updateName)) << " --- ";
            updateItem->get_KBArticleIDs(&KBCollection);
            KBCollection->get_Count(&KBCount);
            for (int i = 0; i < KBCount; i++)
            {
                BSTR KBValue;
                totalKB += 1;
                KBCollection->get_Item(i, &KBValue);
                USES_CONVERSION;
                outputFile << W2A(CString("KB")) << W2A(CString(KBValue)) << endl;
            }

            IUpdateCollection* updtCollection;
            LONG updtBundledCount;

            //Retrieve the bundled updates
            outputFile << W2A(CString("\t Bundled Updates : "));
            updateItem->get_BundledUpdates(&updtCollection);
            hr = updtCollection->get_Count(&updtBundledCount);
            if ((updtBundledCount > 0) && (hr == S_OK))
            {
                //wcout << L"Bundled Updates " <<(updtBundledCount) << endl;
                for (LONG j = 0; j < updtBundledCount; j++)
                {
                    IUpdate* bundledUpdateItem;

                    updtCollection->get_Item(j, &bundledUpdateItem);

                    bundledUpdateItem->get_Title(&updateName);
                    USES_CONVERSION;
                    outputFile << W2A(CString("\t")) << W2A(CString(updateName)) << " - ";

                    updateItem->get_KBArticleIDs(&KBCollection);
                    KBCollection->get_Count(&KBCount);
                    for (int i = 0; i < KBCount; i++)
                    {
                        BSTR KBValue;
                        totalKB += 1;
                        KBCollection->get_Item(i, &KBValue);
                        outputFile << W2A(CString("KB")) << W2A(CString(KBValue)) << endl;
                    }
                    //wcout<<L"    "<<j+1<<" - "<<bundledName<<endl;
                }

            }
        }
        wcout << "Total KBs : " << totalKB << endl;
        outputFile.close();
        ::CoUninitialize();
    }
    catch (const std::exception& ex)
    {
        cout << ex.what();
        ::CoUninitialize();
    }
    return 0;
}