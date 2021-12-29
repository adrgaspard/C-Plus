#include <stdio.h>
#include "../lib/Primitives.h"
#include "../lib/Object.h"
#include "../lib/StringManager.h"
#include "../lib/Collections.h"

int main()
{
    StringManager sm = StringManager__New("test")->Append("yes");
	printf("%s\n", sm->GetValue());
	printf("%ld\n", sm->GetHashCode());
    printf("Ok !\n");
	return 0;
}