/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_line_utils3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 15:21:23 by ael-hara          #+#    #+#             */
/*   Updated: 2024/08/05 22:04:44 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	quote_loop(t_cmd *cmd, int i, int j, t_data *data)
{
	int	k;

	k = 0;
	while (cmd[i].files[j]->file[k] != '\0')
	{
		if ((cmd[i].files[j]->file[k] == '"'
				|| cmd[i].files[j]->file[k] == '\'')
			&& cmd[i].files[j]->type == HEREDOC)
			cmd[i].files[j]->expanding_heredoc = 1;
		else
			cmd[i].files[j]->expanding_heredoc = 0;
		k++;
	}
	cmd[i].files[j]->file = remove_q(cmd[i].files[j]->file, data);
}

void	remove_quotes(t_cmd *cmd, t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->counter_command)
	{
		j = 0;
		while (j < cmd[i].outfile + cmd[i].infile
			+ cmd[i].heredoc + cmd[i].append)
		{
			quote_loop(cmd, i, j, data);
			j++;
		}
		j = 0;
		while (j < cmd[i].args_number)
		{
			cmd[i].args[j] = remove_q(cmd[i].args[j], data);
			j++;
		}
		cmd[i].cmd = remove_q(cmd[i].cmd, data);
		i++;
	}
}

int	check_null(char *str)
{
	int	i;
	int	len;

	i = 0;
	if (!str || !str[0])
		return (0);
	len = ft_strlen(str);
	while (str[len - 1] == ' ' || str[len - 1] == '\n' || str[len - 1] == '\t')
		len--;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t')
		i++;
	while (i < len)
	{
		if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t')
			return (0);
		i++;
	}
	return (1);
}

char	*ft_strdup_quotes(char *s1, t_data *data)
{
	int		len;
	char	*dup;
	int		i;

	len = ft_strlen(s1);
	dup = ft_malloc(sizeof(char) * (len + 1), &data->allocated);
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s1[i];
		i++;
	}
	dup[len] = '\0';
	return (dup);
}

void	init_fill_command(t_cmd *cmds, t_indexes *indexes,
		char **split, t_data *data)
{
	indexes->j = 0;
	indexes->l = 0;
	indexes->k = 0;
	split = ft_split_str(data->pipes[indexes->i], " \t", data);
	initialize_cmd_and_set_flags(cmds, indexes, split, data);
}
