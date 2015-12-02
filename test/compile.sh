
CFLAGS+="-g "
CFLAGS+="-Wall "
CFLAGS+="-Wextra "
# CFLAGS+="-v "
CC="gcc ${CFLAGS}"

# gcc -g main.c ../m_stack.c -o main

# gcc ${CFLAGS} test_m_stack.c ../m_stack.c -I ../ -o test_m_stack

${CC} test_m_logic_expression.c ../m_stack.c ../m_binary_tree.c -o test_m_logic_expression
${CC} sample_m_logic_expression.c ../m_stack.c ../m_binary_tree.c -o sample_m_logic_expression
