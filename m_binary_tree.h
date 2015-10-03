#ifndef _M_BINARY_TREE_NODE_
#define _M_BINARY_TREE_NODE_

struct m_binary_tree_node {
	struct m_binary_tree_node *parent;
	struct m_binary_tree_node *left;
	struct m_binary_tree_node *right;
};

/* returen -1 if want to stop tranver, else return 0 */
typedef int (*traver_call_back)(struct m_binary_tree_node *node, void *arg);

int m_binary_tree_insert_left(
		struct m_binary_tree_node *root, struct m_binary_tree_node *node);
int m_binary_tree_insert_right(
		struct m_binary_tree_node *root, struct m_binary_tree_node *node);
int m_binary_tree_zero_node(struct m_binary_tree_node *node);
int m_binary_tree_is_leaf_node(struct m_binary_tree_node *node);
int m_binary_tree_traver_LDR(
	struct m_binary_tree_node *root, traver_call_back call, void *arg);
int m_binary_tree_traver_LRD(
	struct m_binary_tree_node *root, traver_call_back call, void *arg);
#endif
