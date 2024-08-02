/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:37:48 by ael-hara          #+#    #+#             */
/*   Updated: 2024/08/01 20:41:03 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	initialize_cmd_and_set_flags(t_cmd *cmds,
		t_indexes *indexes, char **split, t_data *data)
{
	initialize_cmd(&cmds[indexes->i]);
	cmds[indexes->i].outfile = count_str_in_array(">", split);
	cmds[indexes->i].append = count_str_in_array(">>", split);
	cmds[indexes->i].heredoc = count_str_in_array("<<", split);
	cmds[indexes->i].infile = count_str_in_array("<", split);
	cmds[indexes->i].flag_outfiles = find_last_str("<", "<<", split);
	cmds[indexes->i].flag_infiles = find_last_str(">", ">>", split);
	cmds[indexes->i].files = ft_malloc(sizeof(t_redir *)
			* (cmds[indexes->i].heredoc + cmds[indexes->i].append
				+ cmds[indexes->i].outfile + cmds[indexes->i].infile + 1),
			&data->allocated);
	cmds[indexes->i].files[cmds[indexes->i].heredoc + cmds[indexes->i].append
		+ cmds[indexes->i].outfile + cmds[indexes->i].infile] = NULL;
}

void	check_null_command(t_cmd *cmds, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->counter_command)
	{
		if (cmds[i].cmd == NULL)
		{
			cmds[i].flag_command = 1;
		}
		i++;
	}
}

char	*change_delimiter(char *str, t_data *data)
{
	int		i;
	char	hold;
	char	*tmp2;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1]
			&& (str[i + 1] == '\'' || str[i + 1] == '"'))
		{
			hold = str[i + 1];
			tmp2 = ft_substr(str, i + 1, ft_strlen(str) - i - 1, data);
			str = ft_strjoin(ft_substr(str, 0, i, data), tmp2, data);
		}
		if (str[i] && (str[i] == '"' || str[i] == '\''))
		{
			hold = str[i];
			i++;
			while (str[i] && str[i] != hold)
				i++;
		}
		i++;
	}
	return (str);
}

void	heredoc_loop(t_cmd *cmds, t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->counter_command)
	{
		j = 0;
		while (j < cmds[i].outfile + cmds[i].infile + cmds[i].heredoc
			+ cmds[i].append)
		{
			if (cmds[i].files[j]->type == HEREDOC)
			{
				cmds[i].files[j]->file
					= change_delimiter(cmds[i].files[j]->file, data);
			}
			j++;
		}
		i++;
	}
}

void	fill_command(t_data *data)
{
	char		**split;
	t_indexes	indexes;
	t_cmd		*cmds;

	indexes = (t_indexes){0, 0, 0, 0, 0};
	cmds = ft_malloc(sizeof(t_cmd) * data->counter_command, &data->allocated);
	while (indexes.i < data->counter_command)
	{
		split = ft_split_str(data->pipes[indexes.i], " \t", data);
		init_fill_command(cmds, &indexes, split, data);
		while (split[indexes.j])
		{
			quote_replace(split[indexes.j], -6, ' ');
			quote_replace(split[indexes.j], -42, '\t');
			handle_redirections(cmds, &indexes, split, data);
			indexes.j++;
		}
		fill_command_index(cmds, &indexes, split, data);
		cmds[indexes.i].args_number = indexes.k;
		indexes.k = 0;
		indexes.j = indexes.l;
		handle_cmd_args(&cmds[indexes.i], &indexes.j, &indexes.k, split);
		indexes.i++;
	}
	fill_command_help(cmds, data);
}
