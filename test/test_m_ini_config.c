#include <string.h>
#include <assert.h>
#include "m_ini_config.h"

/*

[section1]
value1=abc
value2=2
value3=3
value4=4
value5=5
value6=yes

[section2]

[section3]
value1=abcabcabcabcabcabcabc

[section4]
# int max
value2=2147483647
# uint max 
value3=4294967295
# long max on x86
value4=2147483647
# ulong max on x86
value5=4294967295

[section5]
# int min
value2=-2147483648
# long min on x86
value4=-2147483648

[section6]
# long min on x86-64
value4=-9223372036854775808
# ulong max on x86-64
value5=18446744073709551615

[section7]
# long max on x86-64
value4=9223372036854775807

[section8]
value2=-2147483649
value4=9223372036854775808
 */

int main()
{
	char value1[128];
	int value2 = 0;
	unsigned int value3 = 0;
	long value4 = 0;
	unsigned long value5 = 0;
	char value6 = 0;

	struct m_ini_config_descr descr[] = {
		{"section1", "value1", value1,  16,  ICVT_STRING, "default value1"},
		{"section1", "value2", &value2, 0,   ICVT_INT,    "2"},
		{"section1", "value3", &value3, 0,   ICVT_UINT,   "3"},
		{"section1", "value4", &value4, 0,   ICVT_LONG,   "4"},
		{"section1", "value5", &value5, 0,   ICVT_ULONG,  "5"},
		{"section1", "value6", &value6, 0,   ICVT_BOOL,   "Yes"},
		M_INI_CONFIG_DESCR_NULL
	};


	assert(m_ini_config_read("test_m_ini_config.c", descr) == 0);
	m_ini_config_print_descr(descr);
	assert(strcmp(value1, "abc") == 0);
	assert(value2 == 2);
	assert(value3 == 3);
	assert(value4 == 4);
	assert(value5 == 5);
	assert(value6 == 1);

	struct m_ini_config *config = m_ini_config_init("test_m_ini_config.c");
	assert(config != NULL);

	/* section 1 */
	assert(m_ini_config_next(config, descr) == 0);

	/* section 2 */
	assert(m_ini_config_next(config, descr) == 0);
	assert(strcmp(value1, "default value1") == 0);
	assert(value2 == 2);
	assert(value3 == 3);
	assert(value4 == 4);
	assert(value5 == 5);
	assert(value6 == 1);

	/* section 3 */
	assert(m_ini_config_next(config, descr) == 0);
	assert(strcmp(value1, "abcabcabcabcabc") == 0);
	m_ini_config_print_descr(descr);

	/* section 4 */
	assert(m_ini_config_next(config, descr) == 0);
	assert(value2 == 2147483647);
	assert(value3 == 4294967295U);
	assert(value4 == 2147483647L);
	assert(value5 == 4294967295UL);
	assert(value6 == 1);

	/* section 5 */
	assert(m_ini_config_next(config, descr) == 0);
	assert(value2 == (-2147483647 - 1));
	assert(value4 == (-2147483647 - 1));
	m_ini_config_print_descr(descr);

# if __WORDSIZE == 64
	/* section 6 */
	assert(m_ini_config_next(config, descr) == 0);

	/* section 7 */
	assert(m_ini_config_next(config, descr) == 0);
# else
	/* section 6 */
	assert(m_ini_config_next(config, descr) == -1);

	/* section 7 */
	assert(m_ini_config_next(config, descr) == -1);
#endif

	/* section 8 */
	assert(m_ini_config_next(config, descr) == -1);

	assert(m_ini_config_next(config, descr) == 1);
	
	m_ini_config_clean(config);

	return 0;
}
