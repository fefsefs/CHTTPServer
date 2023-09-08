#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	char *hello;
	FILE *flix = fopen("index.txt", "r");
	if (flix == NULL)
	{
		printf("err openin file");
		exit(EXIT_FAILURE);
	}
	read(hello, 170, flix);
	printf("%s", hello);

	return 0;
}
