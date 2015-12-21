/*
s0-1=1
s0-2=2
s0-3=3
 [section 1]

s1-1=
s1-2= he llo    
s1-3 :12
 ;; this is a comment

 ; this is also a comment
 # and yet another comment

[]
empty0=0
empty1=1
empty2=2

[1]
1-1=3
1-2=2
1-3=3
 [section 2]

 s2-1 = something in section 2
 s2-2: true
 s2-3=4.555

 [wrong1
 w1-1=1
 w1-2=2
 w1-3=3

 [section 3]
 s3-1=1
 s3-2=2
 s3-3=[=:;]a

 wrong2]
 w2-1=1
 w2-2=3
 w2-3:3

 [wrong3
 [wrong4
 w4-1=0
 w4-2=1
 w4-3=2

wrong5]
w5-1=1
w5-2=2
w5-3=3

[]#;wrong6]]
w6-1=1
w6-2=2
w6-3=3

[wrong7#;=]
w7-1=1
w7-2=2
w7-3=3


 [section code]
 long-key-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx=123
 long-value=long-key-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx=123
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "m_ini_parser.h"

int
main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
    char *value;

    /* ini_r is a structure that holds state to make this reader
     * reentrant */
	struct read_ini *ini_r = NULL;
	struct ini *ini = read_ini(&ini_r, "test_m_ini_parser.c");

    /* pretty printing of ini structure */
	ini_pp(ini);

    /* retrieve a value */
    value = ini_get_value(ini, "section 1", "s1-1");
	assert(strcmp(value, "") == 0);
	value = ini_get_value(ini, "section 1", "s1-2");
	assert(strcmp(value, "he llo") == 0);
	value = ini_get_value(ini, "section 1", "s1-3");
	assert(strcmp(value, "12") == 0);

	value = ini_get_value(ini, "]#;wrong6]", "w6-1");
	assert(strcmp(value, "1") == 0);
	value = ini_get_value(ini, "]#;wrong6]", "w6-2");
	assert(strcmp(value, "2") == 0);
	value = ini_get_value(ini, "]#;wrong6]", "w6-3");
	assert(strcmp(value, "3") == 0);

    /* free memory */
	destroy_ini(ini);
    cleanup_readini(ini_r);

	return 0;
}

