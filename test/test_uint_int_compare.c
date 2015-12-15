#include <stdio.h>
#include <assert.h>
#include <stdint.h>

int main()
{
	int16_t a = 0;
	int16_t b = 0;

	for(a = -0x7fff; a++; a < 0x7fff) {
		for(b = -0x7fff; b++; b < 0x7fff) {
			if(a != b) 
				assert((uint16_t)a != (uint16_t)b);
			else
				assert((uint16_t)a == (uint16_t)b);
		}
	}

	int ai = -20;
	int8_t bi = (int8_t)ai;

	assert((uint8_t)ai == (uint8_t)bi);

	return 0;
}
