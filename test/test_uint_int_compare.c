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

		printf(".");

	}
}
