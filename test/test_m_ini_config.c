#include "m_ini_config.h"

int main()
{
	char value1[128];
	int value2 = 0;
	unsigned int value3 = 0;
	long value4 = 0;
	unsigned long value5 = 0;
	char value6 = 0;

	struct descr[] = {
		{"section1", "value1", value1,  128, ICVT_STRING, "default value1"},
		{"section1", "value2", &value2, 0,   ICVT_INT,    "0"},
		{"section1", "value3", &value3, 0,   ICVT_UINT,   "0"},
		{"section1", "value4", &value4, 0,   ICVT_LONG,   "0"},
		{"section1", "value5", &value5, 0,   ICVT_ULONG,  "0"},
		{"section1", "value6", &value6, 0,   ICVT_BOOL,   "0"},
	};

/*
int m_ini_config_read(const char *filename, struct m_ini_config_descr *descr);


 60 struct m_ini_config *m_ini_config_init(const char *filename);
  61 int m_ini_config_next(struct m_ini_config *config, struct m_ini_config_descr *descr);
   62 void m_ini_config_clean(struct m_ini_config *config);

*/
	return 0;
}
