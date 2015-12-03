CFLAGS+="-I ../ "
CFLAGS+="-g "
CFLAGS+="-Wall "
CFLAGS+="-Wextra "
# CFLAGS+="-v "

CC="gcc ${CFLAGS}"

test_m_stack_dep="test_m_stack.c ../m_stack.c" 
test_m_logic_expression_dep="test_m_logic_expression.c ../m_stack.c ../m_binary_tree.c "
sample_m_logic_expression_dep="../m_stack.c ../m_binary_tree.c sample_m_logic_expression.c "


${CC} ${test_m_stack_dep} -o test_m_stack
${CC} ${test_m_logic_expression_dep} -o test_m_logic_expression
${CC} ${sample_m_logic_expression_dep} -o sample_m_logic_expression
