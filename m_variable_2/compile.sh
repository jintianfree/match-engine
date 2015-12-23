CFLAGS+="-I ../ "
CFLAGS+="-g "
CFLAGS+="-Wall "
CFLAGS+="-Wextra "
# CFLAGS+="-v "

CC="gcc ${CFLAGS}"

test_m_variable_table_dep="test_m_variable_table.c m_variable_table.c"


${CC} ${test_m_variable_table_dep} -o test_m_variable_table
