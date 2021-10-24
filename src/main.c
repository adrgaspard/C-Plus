#include <stdio.h>
#include "../lib/C+/StringManager.h"
#include "../lib/C+/Exception.h"

int main()
{
	StringManager* sm = StringManager_New("test")->append("yes");
	printf("%s\n", sm->getValue());
	printf("%ld\n", ((Object*)sm)->getHashCode());
	EXCEPTION("Mon exception de test");
	return (0);
}