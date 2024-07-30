/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_line_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 02:51:00 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/30 04:27:07 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

char	*remove_q(char* str, t_data *data)
{
	int len = ft_strlen(str);
	char *new_str = ft_malloc(sizeof(char) * (len + 1), &data->allocated);
	int i = 0, j = 0;
	while (i < len) 
	{
		char c = str[i];
		if ((str[i] == '"' || str[i] == '\''))
		{
			c = str[i];
			i++;
			while (str[i] && str[i] != c)
			{ 
				new_str[j] = str[i];
				j++;
				i++;
			}
			if (str[i] == c)
			{
				i++;
			}
		}
		else
		{
			new_str[j] = str[i];
			j++;
			i++;
		}
	}
	new_str[j] = '\0';
	return (new_str);
}

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

	i = 0;
	if (!str || !str[0])
		return (0);
	while (str[i])
	{
		if (str[i] == ' ' && str[i] == '\n' && str[i] == '\t' && str[i] == '*')
			return (0);
		i++;
	}
	return (1);
}

void ambigious (t_cmd *cmd, t_data *data)
{
	int i = 0;
	while (i < data->counter_command)
	{
		int j = 0;
		while (j < cmd[i].outfile + cmd[i].infile + cmd[i].heredoc + cmd[i].append)
		{
			if (cmd[i].files[j]->type != HEREDOC)
				cmd[i].files[j]->file = expanding_outside(cmd[i].files[j]->file, data);
			if (cmd[i].files[j] && check_null(cmd[i].files[j]->file) == 0 && cmd[i].files[j]->type != HEREDOC)
			{
				cmd[i].files[j]->expand_error = 1;
			}
			else
			{
				cmd[i].files[j]->expand_error = 0;
			}
			quote_replace(cmd[i].files[j]->file, -11, '$');
			if (cmd[i].files[j]->file && cmd[i].files[j]->type != HEREDOC)
			{
				cmd[i].files[j]->file = expanding_final(cmd[i].files[j]->file, data);
			}
			j++;
		}
		j = 0;
		while (j < cmd[i].args_number)
		{
			quote_replace(cmd[i].args[j], -11, '$');
			if (cmd[i].args[j])
				cmd[i].args[j] = expanding_final(cmd[i].args[j], data);
			j++;
		}
		if(cmd[i].cmd)
		{
		quote_replace(cmd[i].cmd, -11, '$');
		
		if (cmd[i].cmd)
			cmd[i].cmd = expanding_final(cmd[i].cmd, data);
		}
		i++;
	}
}
