#include "m_binary_tree.h"

int m_binary_tree_insert_left(
		struct m_binary_tree_node *root, struct m_binary_tree_node *node)
{
	assert(root && node);
	assert(!root->left);

	root->left = node;
	node->parent = root;

	return 0;
}

int m_binary_tree_insert_right(
		struct m_binary_tree_node *root, struct m_binary_tree_node *node)
{
	assert(root && node);
	assert(!root->right);

	root->right = node;
	node->parent = root;

	return 0;
}

int m_binary_tree_zero_node(struct m_binary_tree_node *node)
{
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;

	return 0;
}

int m_binary_tree_is_leaf_node(struct m_binary_tree_node *node)
{
	return (node->left == NULL && node->right == NULL);
}

void m_binary_tree_traver_LDR(
	struct m_binary_tree_node *root, traver_call_back call, void *arg)
{
	if(m_binary_tree_is_leaf_node(root)) {
		call(root, arg);
		return;
	}

	if(root->left) {
		m_binary_tree_traver_LDR(root->left, call, arg);
	}

	call(root, arg);

	if(root->right) {
		m_binary_tree_traver_LDR(root->right, call, arg);
	}

	return;
}

