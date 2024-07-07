/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 17:50:45 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/07 09:46:53 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (1);
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (1);
		s1++;
		s2++;
	}
	if (*s1 != *s2)
		return (1);
	return (0);
}

int	node_compare(t_list *lst, char *str)
{
	while (lst)
	{
		if (ft_strcmp(lst->content, str) == 0)
			return (1);
		lst = lst->next;
	}
	return (0);
}

void	replace_node_content(t_list *lst, char *new_str)
{
	if (lst)
	{
		lst->content = new_str;
	}
}

int	count_content_lst(t_list *lst, char *str)
{
	int	count;

	count = 0;
	while (lst)
	{
		if (lst->content && ft_strcmp(lst->content, str) == 0)
			count++;
		lst = lst->next;
	}
	return (count);
}

void	initialize_cmd(t_cmd *cmd)
{
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->files = NULL;
	cmd->outfile = 0;
	cmd->heredoc = 0;
	cmd->append = 0;
	cmd->infile = 0;
	cmd->flag_infiles = 0;
	cmd->flag_outfiles = 0;
	cmd->args_number = 0;
}
