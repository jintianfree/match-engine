CFLAGS+="-I ../ "
CFLAGS+="-g "
CFLAGS+="-Wall "
CFLAGS+="-Wextra "
# CFLAGS+="-v "

CC="gcc ${CFLAGS}"

sample_m_logic_expression_dep="../m_stack.c ../m_binary_tree.c sample_m_logic_expression.c "
test_m_stack_dep="test_m_stack.c ../m_stack.c" 
test_m_logic_expression_dep="test_m_logic_expression.c ../m_stack.c ../m_binary_tree.c "
test_m_variable_dep="test_m_variable.c ../m_variable.c"
test_m_operator_equal_dep="test_m_operator_equal.c ../m_variable.c"
test_m_operator_null_dep="test_m_operator_null.c ../m_variable.c"
test_m_operation_dep="test_m_operation.c ../m_variable.c ../m_operation.c ../m_operator_equal.c ../m_operator_null.c"
test_m_common_dep="test_m_common.c"
test_m_logic_operation_dep="test_m_logic_operation.c ../m_logic_operation.c ../m_logic_expression.c ../m_stack.c ../m_binary_tree.c ../m_variable.c ../m_operation.c ../m_operator_equal.c ../m_operator_null.c"


${CC} ${test_m_stack_dep} -o test_m_stack
${CC} ${test_m_logic_expression_dep} -o test_m_logic_expression
${CC} ${sample_m_logic_expression_dep} -o sample_m_logic_expression
${CC} ${test_m_variable_dep} -o test_m_variable
${CC} ${test_m_operator_equal_dep} -o test_m_operator_equal
${CC} ${test_m_operator_null_dep} -o test_m_operator_null
${CC} ${test_m_operation_dep} -o test_m_operation
${CC} ${test_m_common_dep} -o test_m_common
${CC} ${test_m_logic_operation_dep} -o test_m_logic_operation
