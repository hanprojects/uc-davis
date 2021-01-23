#include <locale.h>
#include <stdio.h>
#include <wchar.h>

#define ARRAY_SIZE(x)	(sizeof(x) / sizeof(x[0]))

int main(void)
{
	/* Shades of gray as unicode block elements */
	wchar_t block[] = {
		L' ',		/* Empty */
		L'\u2591',	/* Light shade */
		L'\u2592',	/* Medium shade */
		L'\u2593',	/* Dark shade */
		L'\u2588',	/* Full */
	};

	/* Configure $LC_ALL for unicode */
	setlocale(LC_ALL, "en_US.UTF-8");

	/* Print all the unicode block elements */
	for (size_t i = 0; i < ARRAY_SIZE(block); i++)
		printf("%lc", block[i]);
	printf("\n");

	return 0;
}