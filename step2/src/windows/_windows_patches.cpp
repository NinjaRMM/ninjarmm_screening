#include "patch_info.h"
#include "_windows_patches.h"
#include "_windows_helpers.h"
#include <Windows.h>
#include <Msi.h>
#include <iostream>

bool system_get_patches(std::vector<PatchInfo> &patches)
{

		UINT ret;
		DWORD dwIndex = 0;
		DWORD dwContext = MSIINSTALLCONTEXT_ALL;
		wchar_t szInstalledProductCode[39] = { 0 };
		wchar_t szInstalledPatchCode[39] = { 0 };
		wchar_t szSid[128] = { 0 };
		const wchar_t* szUserSid = L"s-1-1-0";
		DWORD cchSid;
		MSIINSTALLCONTEXT dwInstalledContext;

		do
		{
			PatchInfo patch;
			memset(szInstalledProductCode, 0, sizeof(szInstalledProductCode));
			cchSid = sizeof(szSid) / sizeof(szSid[0]);
			ret = MsiEnumPatchesExW(NULL,           // all the products in the context
				szUserSid,  // i.e.Everyone, all users in the system
				dwContext,
				MSIPATCHSTATE_APPLIED,//MSIPATCHSTATE_ALL, // The enumeration includes all applied, obsolete, superseded, and registered patches.
				dwIndex,
				szInstalledPatchCode,
				szInstalledProductCode,
				&dwInstalledContext,
				szSid,
				&cchSid);

			if (ret == ERROR_SUCCESS)
			{
				DWORD           pcchValue = 0;
				wchar_t* name = nullptr;
				ret = MsiGetPatchInfoExW(
					szInstalledPatchCode,
					szInstalledProductCode,
					cchSid == 0 ? NULL : szSid,
					dwInstalledContext,
					INSTALLPROPERTY_DISPLAYNAME, name, &pcchValue

				);
				if (ret == ERROR_SUCCESS)
				{
					pcchValue = pcchValue + 1;
					name = (wchar_t*)malloc(sizeof(wchar_t) * (pcchValue));

					ret = MsiGetPatchInfoExW(
						szInstalledPatchCode,
						szInstalledProductCode,
						cchSid == 0 ? NULL : szSid,
						dwInstalledContext,
						INSTALLPROPERTY_DISPLAYNAME, name, &pcchValue);
					// integrate strings in utf to be compatible for linux. (like glib strings)
					patch.name = utf8_encode(name);
					patch.type_description = "applied";
					free(name);
				}

				patches.push_back(patch);
				dwIndex++;
			}

		} while (ret == ERROR_SUCCESS);


    return true;
}

