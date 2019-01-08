#include <tinyxml2.h>

extern "C" {
	#include "tinyxml2_helper.h"
}

using namespace tinyxml2;

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); }
#endif

void Tinyxml2_BackupFriendList(char *path, u32 count, char *friendNames, u64 *friendCodes) {
	XMLDocument xmlDoc;
	XMLNode *pRoot = xmlDoc.NewElement("FriendMii Backup");
	xmlDoc.InsertFirstChild(pRoot);

	XMLElement *pElement;

	for (u32 i = 0; i < count; i++) {
		pElement = xmlDoc.NewElement(&friendNames[i * 0xB]);

		char *friendcode_parsed = (char *)malloc(21);
		snprintf(friendcode_parsed, 21, "%04llu%04llu%04llu", friendCodes[i]/100000000LL, (friendCodes[i]/10000)%10000, friendCodes[i]%10000);
		pElement->SetText(friendcode_parsed);

		pRoot->InsertEndChild(pElement);

		free(friendcode_parsed);
	}

	XMLError eResult = xmlDoc.SaveFile(path);
	XMLCheckResult(eResult);
}
