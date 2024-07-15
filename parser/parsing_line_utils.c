/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_line_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:15:14 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/14 07:05:43 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	process_heredoc_infile(t_cmd *cmds,
				t_indexes *indexes, char **split, t_data *data)
{
	cmds[indexes->i].files[indexes->k]->file = split[indexes->j + 1];
	cmds[indexes->i].files[indexes->k]->type = HEREDOC;
	split[indexes->j] = ft_strdup("|", &data->allocated);
	split[indexes->j + 1] = ft_strdup("|", &data->allocated);
}

void	handle_heredoc_and_infile(t_cmd *cmds, t_indexes *indexes,
		char **split, t_data *data) {
	if ((ft_strcmp(split[indexes->j], "<<") == 0)
		&& split[indexes->j + 1] != NULL)
	{
		quote_replace(split[indexes->j + 1], -6, ' ');
		quote_replace(split[indexes->j + 1], -42, '\t');
		cmds[indexes->i].files[indexes->k]
			= ft_malloc(sizeof(t_redir), &data->allocated);
		process_heredoc_infile(cmds, indexes, split, data);
		indexes->k++;
	}
	else if ((ft_strcmp(split[indexes->j], "<") == 0)
		&& split[indexes->j + 1] != NULL)
	{
		quote_replace(split[indexes->j + 1], -6, ' ');
		quote_replace(split[indexes->j + 1], -42, '\t');
		cmds[indexes->i].files[indexes->k]
			= ft_malloc(sizeof(t_redir), &data->allocated);
		cmds[indexes->i].files[indexes->k]->file = split[indexes->j + 1];
		cmds[indexes->i].files[indexes->k]->type = INFILE;
		split[indexes->j] = ft_strdup("|", &data->allocated);
		split[indexes->j + 1] = ft_strdup("|", &data->allocated);
		indexes->k++;
	}
}

void	process_append(t_cmd *cmds,
				t_indexes *indexes, char **split, t_data *data)
{
	cmds[indexes->i].files[indexes->k]->file = split[indexes->j + 1];
	cmds[indexes->i].files[indexes->k]->type = APPEND;
	split[indexes->j] = ft_strdup("|", &data->allocated);
	split[indexes->j + 1] = ft_strdup("|", &data->allocated);
}

void	handle_redirections(t_cmd *cmds, t_indexes *indexes,
		char **split, t_data *data)
{
	if ((ft_strcmp(split[indexes->j], ">") == 0)
		&& split[indexes->j + 1] != NULL)
	{
		quote_replace(split[indexes->j + 1], -6, ' ');
		quote_replace(split[indexes->j + 1], -42, '\t');
		cmds[indexes->i].files[indexes->k]
			= ft_malloc(sizeof(t_redir), &data->allocated);
		cmds[indexes->i].files[indexes->k]->file = split[indexes->j + 1];
		cmds[indexes->i].files[indexes->k]->type = OUTFILE;
		split[indexes->j] = ft_strdup("|", &data->allocated);
		split[indexes->j + 1] = ft_strdup("|", &data->allocated);
		indexes->k++;
	}
	else if ((ft_strcmp(split[indexes->j], ">>") == 0)
		&& split[indexes->j + 1] != NULL)
	{
		quote_replace(split[indexes->j + 1], -6, ' ');
		quote_replace(split[indexes->j + 1], -42, '\t');
		cmds[indexes->i].files[indexes->k]
			= ft_malloc(sizeof(t_redir), &data->allocated);
		process_append(cmds, indexes, split, data);
		indexes->k++;
	}
	else
		handle_heredoc_and_infile(cmds, indexes, split, data);
}

void	handle_cmd_allocation(t_cmd *cmds, int *j, char **split, t_data *data)
{
	while (split[*j])
	{
		if (ft_strcmp(split[*j], "|") != 0)
		{
			cmds->cmd = ft_malloc(sizeof(char) * ft_strlen(split[*j]),
					&data->allocated);
			cmds->cmd = split[*j];
			(*j)++;
			break ;
		}
		(*j)++;
	}
}

void	handle_cmd_args(t_cmd *cmds, int *j, int *k, char **split)
{
	while (split[*j])
	{
		if (ft_strcmp(split[*j], "|") != 0)
		{
			cmds->args[*k] = split[*j];
			(*k)++;
		}
		(*j)++;
	}
}

void	count_cmd_args(int *j, int *k, char **split)
{
	while (split[*j])
	{
		if (ft_strcmp(split[*j], "|") != 0)
		{
			(*k)++;
		}
		(*j)++;
	}
}
