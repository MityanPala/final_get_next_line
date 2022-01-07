/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcoralie <gcoralie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/20 15:20:12 by gcoralie          #+#    #+#             */
/*   Updated: 2022/01/07 18:56:12 by gcoralie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>

t_tree	*gc_create_node(int fd)
{
	t_tree	*node;

	node = (t_tree *)malloc(sizeof(t_tree));
	if (node == NULL)
		return (NULL);
	node->fd = fd;
	node->eof_met = 0;
	node->buf_len = 0;
	node->str_len = 0;
	node->str_left = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

/*	Insert node to the tree */
void	gc_insert_node(t_tree **root, t_tree *node)
{
	if (root == NULL || node == NULL)
		return ;
	if (*root == NULL)
	{
		*root = node;
		return ;
	}
	if (node->fd < (*root)->fd)
		gc_insert_node(&((*root)->left), node);
	else
		gc_insert_node(&((*root)->right), node);
}

/*	Returns NULL if no node with such FD was found or pointer to the node; 
	Parent pointer will be used to erase nodes - it will be only valid
	if the node we find is not null and is not equal to the root */
t_tree	*gc_find_node(t_tree *root, int fd, t_tree **parent)
{
	t_tree	*res;

	if (root == NULL || parent == NULL)
		return (NULL);
	if (root->fd == fd)
		return (root);
	*parent = root;
	res = gc_find_node(root->left, fd, parent);
	if (res == NULL)
	{
		*parent = root;
		res = gc_find_node(root->right, fd, parent);
	}
	return (res);
}

/*	If delete_all_children == 1, then we recursively delete the tree */
void	gc_free_nodes(t_tree *node, int delete_all_children)
{
	if (node == NULL)
		return ;
	if (delete_all_children == 1)
	{
		gc_free_nodes(node->left, delete_all_children);
		gc_free_nodes(node->right, delete_all_children);
	}
	free(node->str_left);
	free(node->left);
	free(node->right);
	free(node);
}

/*	If we want to delete root node, then one of its children will be root
	and another one will be inserted into new root;
	Otherwise we just insert children into root and then delete the node
	we wanted */
void	gc_erase_node(t_tree **root, t_tree *node, t_tree *parent)
{
	if (root == NULL || node == NULL)
		return ;
	if (node == *root)
		*root = node->right;
	else
	{
		if (parent == NULL)
			return ;
		if (parent->left == node)
			parent->left = NULL;
		else
			parent->right = NULL;
		gc_insert_node(root, node->right);
	}
	gc_insert_node(root, node->left);
	node->right = NULL;
	node->left = NULL;
	gc_free_nodes(node, 0);
}
