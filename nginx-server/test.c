#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

#include "traffic_resolv.h"

int main(int argc, char * argv[])
{
	char* z;
	char text[]="{\"Time\":       \"22:27:46\",\"Rxtotal\":       \"2222222.3\", \n\"Txtotal\":     \"333.2\",\n\"Counts\":     \"121\"\n}";
	char* result;
puts("1");
	char* xz;
	xz = json_analysis(text, &result);
puts("dsfsdfsd");
	puts(xz);
	return 0;
}
