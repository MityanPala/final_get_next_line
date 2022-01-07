/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcoralie <gcoralie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/20 15:20:10 by gcoralie          #+#    #+#             */
/*   Updated: 2022/01/07 18:19:55 by gcoralie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <unistd.h>
#include <stdlib.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 10
#endif

static char	*gc_read_line(t_tree *node, int fd);
static char	*gc_erase_line(t_tree *node);
static void	*gc_realloc(t_tree *node);
static void	*gc_memcpy(void *dest, const void *src, size_t n);

char	*get_next_line(int fd)
{
	static t_tree	*root = NULL;
	t_tree			*node;
	t_tree			*parent;
	char			*res;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	node = gc_find_node(root, fd, &parent);
	if (node == NULL)
	{
		node = gc_create_node(fd);
		gc_insert_node(&root, node);
	}
	res = gc_read_line(node, fd);
	if (res == NULL || (node->eof_met == 1 && node->str_len == 0))
		gc_erase_node(&root, node, parent);
	return (res);
}

static char	*gc_read_line(t_tree *node, int fd)
{
	int		len_read;
	char	*res;

	res = gc_erase_line(node);
	if (res != NULL)
		return (res);
	len_read = 1;
	while (len_read > 0)
	{
		if (node->buf_len - node->str_len < BUFFER_SIZE)
			if (gc_realloc(node) == NULL)
				return (NULL);
		len_read = read(fd, (node->str_left + node->str_len), BUFFER_SIZE);
		if (len_read < 0)
			return (NULL);
		if (len_read == 0)
			node->eof_met = 1;
		if (len_read > 0)
			node->str_len += len_read;
		res = gc_erase_line(node);
		if (res != NULL)
			return (res);
	}
	return (res);
}

/*	Function allows us to check for \n and if there is no \n
	then we return NULL, if there is \n, we copy string and
	return pointer to it;
	If node->eof_met == 0 and len == node->str_len, then it means
	that we have not find \n in the string and that we still have
	some text to read from file descriptor */
static char	*gc_erase_line(t_tree *node)
{
	size_t	len;
	char	*res;

	len = 0;
	while (len < node->str_len)
	{
		if (*((unsigned char *)(node->str_left) + len) == (unsigned char) '\n')
			break ;
		++len;
	}
	if ((len == node->str_len && node->eof_met == 0) || node->str_len == 0)
		return (NULL);
	if (len != node->str_len)
		++len;
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	*(res + len) = '\0';
	gc_memcpy((void *)res, (const void *)node->str_left, len);
	node->str_len = node->str_len - len;
	gc_memcpy(node->str_left,
		(const void *)(node->str_left + len), node->str_len);
	return (res);
}

/*	Return NULL if something bad happens, otherwise return new 
	str pointer and assign new values to node fields before that */
static void	*gc_realloc(t_tree *node)
{
	void	*new_str;
	size_t	new_size;

	if (node->buf_len == 0)
		new_size = BUFFER_SIZE;
	else
		if ((size_t)(-1) / 2 >= node->buf_len)
			new_size = 2 * node->buf_len;
	else
		if (((size_t)(-1) - node->buf_len) / BUFFER_SIZE >= 10)
			new_size = BUFFER_SIZE * 10 + node->buf_len;
	else
		if (((size_t)(-1) - node->buf_len) / BUFFER_SIZE >= 1)
			new_size = BUFFER_SIZE + node->buf_len;
	else
		return (NULL);
	new_str = malloc(sizeof(char) * new_size);
	if (!new_str)
		return (NULL);
	gc_memcpy(new_str, node->str_left, node->str_len);
	free(node->str_left);
	node->str_left = new_str;
	node->buf_len = new_size;
	return (node->str_left);
}

static void	*gc_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*destination;
	const unsigned char	*source;
	size_t				i;

	if (dest == NULL || src == NULL)
		return (NULL);
	destination = (unsigned char *)dest;
	source = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		*(destination + i) = *(source + i);
		++i;
	}
	return (dest);
}
