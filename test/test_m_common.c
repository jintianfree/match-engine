#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "m_log.h"

#include "m_common.h"
int main()
{
	char str[] = "0xab 0xef 0xab";
	uint8_t bytes[127];

	int len = bytes_string_2_bytes(str, bytes, 127);

	printf("%s \n", str);
	print_buffer(bytes, len);

	return 0;
}
