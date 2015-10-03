#include <string.h>
#include <assert.h>
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

int m_binary_tree_traver_LDR(
	struct m_binary_tree_node *root, traver_call_back call, void *arg)
{
	if(m_binary_tree_is_leaf_node(root)) {
		if(call(root, arg) < 0) {
			return -1;
		} else {
			return 0;
		}
	}

	if(root->left) {
		if(m_binary_tree_traver_LDR(root->left, call, arg) < 0) {
			return -1;
		}
	}

	if(call(root, arg) < 0) {
		return -1;
	}

	if(root->right) {
		if(m_binary_tree_traver_LDR(root->right, call, arg) < 0) {
			return -1;
		}
	}

	return 0;
}

int m_binary_tree_traver_LRD(
	struct m_binary_tree_node *root, traver_call_back call, void *arg)
{
	if(m_binary_tree_is_leaf_node(root)) {
		if(call(root, arg) < 0) {
			return -1;
		} else {
			return 0;
		}
	}

	if(root->left) {
		if(m_binary_tree_traver_LRD(root->left, call, arg) < 0) {
			return -1;
		}
	}

	if(root->right) {
		if(m_binary_tree_traver_LRD(root->right, call, arg) < 0) {
			return -1;
		}
	}

	if(call(root, arg) < 0) {
		return -1;
	}

	return 0;
}

