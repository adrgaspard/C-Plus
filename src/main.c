#include <stdio.h>
#include "../lib/Primitives.h"
#include "../lib/Object.h"
#include "../lib/StringManager.h"

int main()
{
    StringManager sm = StringManager__New("test")->Append("yes");
	printf("%s\n", sm->GetValue());
	printf("%ld\n", ((Object)sm)->GetHashCode());
    printf("Ok !\n");
	return 0;
}