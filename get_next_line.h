/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcoralie <gcoralie@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/20 15:20:07 by gcoralie          #+#    #+#             */
/*   Updated: 2022/01/04 20:51:48 by gcoralie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stddef.h>

char	*get_next_line(int fd);

/* 	fd - file descriptor;
	eof_met - we set this to 1 if we met eof while reading;
	buf_len - length of the whole string buffer;
	str_len - length of the string;
	str_left - part of the string that is left if we have read too much in the
	previous time */
typedef struct s_tree
{
	int				fd;
	int				eof_met;
	size_t			buf_len;
	size_t			str_len;
	void			*str_left;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;

/*-----	tree manipulation functions -----*/

t_tree	*gc_create_node(int fd);
void	gc_insert_node(t_tree **root, t_tree *node);
t_tree	*gc_find_node(t_tree *root, int fd, t_tree **parent);
void	gc_free_nodes(t_tree *node, int delete_all_children);
void	gc_erase_node(t_tree **root, t_tree *node, t_tree *parent);

/*-----	tree manipulation functions -----*/

#endif
