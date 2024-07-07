/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:37:48 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/07 09:53:11 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *ft_strdup_quotes(char *s1, t_data *data)
{
	int len = ft_strlen(s1);
	char *dup = ft_malloc(sizeof(char) * (len + 1), &data->allocated);
	if (dup == NULL)
		return NULL;
	int i = 0;
	while (i < len) {
		dup[i] = s1[i];
		i++;
	}
	dup[len] = '\0';
	return dup;
}
char *remove_q(char* str, t_data *data)
{
	int len = ft_strlen(str);
	char *new_str = ft_malloc(sizeof(char) * (len + 1), &data->allocated);
	int i = 0, j = 0;

	while (i < len) {
		char c = str[i];
		if (str[i] == '"' || str[i] == '\'') 
		{
			c = str[i];
			i++;
			while (str[i]  && str[i] != c) 
			{  
				new_str[j] = str[i];  // copy to new string
				j++;
				i++;
			}
			if (str[i] == c) // skip the closing quote
			{
				i++;
			}
		}
		else
		{
			new_str[j] = str[i];  // copy to new string
			j++;
			i++;
		}
	}
	new_str[j] = '\0';  // null terminate the new string
	// printf("new_str: %s\n", new_str);
	return new_str;
}



// char *remove_q(char* str, t_data *data)
// {
// 	int len = ft_strlen(str);
// 	char *new_str = ft_malloc(sizeof(char) * (len + 1), &data->allocated);
// 	int i = 0, j = 0;

// 	while (i < len) {
// 		char c = str[i];
// 		if (str[i] == '"' || str[i] == '\'') 
// 		{
// 			c = str[i];
// 			i++;
// 			while (str[i]  && str[i] != c) 
// 			{  
// 			new_str[j] = str[i];  // copy to new string
// 			j++;
// 			i++;
// 			}
// 		}
// 	}
// 	new_str[j] = '\0';  // null terminate the new string

// 	return new_str;
// }

// char *remove_q(char* str, t_data *data)
// {
// 	int len = ft_strlen(str);
// 	if (len >= 2 && (str[0] == '"' || str[0] == '\'') && str[0] == str[len-1])   //quote error
// 	{
// 		char *new_str = ft_strdup_quotes(str + 1, data);
// 		new_str[len - 2] = '\0';
// 		return new_str;
// 	}
// 	return ft_strdup_quotes(str, data);
// }
void remove_quotes(t_cmd *cmd, t_data *data)
{
	int i = 0;
	while (i < data->counter_command)
	{
		int j = 0;
		while (j < cmd[i].outfile + cmd[i].infile + cmd[i].heredoc + cmd[i].append)
		{
			if ((cmd[i].files[j]->file[0] == '"' || cmd[i].files[j]->file[0] == '\'') 
				&& cmd[i].files[j]->type == HEREDOC)
				cmd[i].files[j]->expanding_heredoc = 1;
			else
				cmd[i].files[j]->expanding_heredoc = 0;	
			cmd[i].files[j]->file = remove_q(cmd[i].files[j]->file, data);
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

void ambigious (t_cmd *cmd, t_data *data)
{
	int i = 0;
	while (i < data->counter_command)
	{
		int j = 0;
		while (j < cmd[i].outfile + cmd[i].infile + cmd[i].heredoc + cmd[i].append)
		{
			
			if (cmd[i].files[j]->file[0] == '$')
				cmd[i].files[j]->expand_error = 1;
			else
				cmd[i].files[j]->expand_error = 0;
			quote_replace(cmd[i].files[j]->file, -11, '$');
			if (cmd[i].files[j]->file && cmd[i].files[j]->type != HEREDOC)
				cmd[i].files[j]->file = expanding_final(cmd[i].files[j]->file, data);
			j++;
		}
		j = 0;
		while (j < cmd[i].args_number)
		{
			quote_replace(cmd[i].args[j], -11, '$');
			if (cmd[i].args[j])
				cmd[i].args[j] = expanding_final(cmd[i].args[j], data);    //quote error
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

void print_cmd (t_cmd *cmd)
{
	int i = 0;
	printf("cmd: %s\n", cmd->cmd);
	printf("args: ");
	while (i < cmd->args_number)
	{
		printf("%s ", cmd->args[i]);
		i++;
	}
	printf("\n");
	i = 0;
	printf("files: ");
	while (i < cmd->outfile + cmd->infile + cmd->heredoc + cmd->append)
	{
		if (cmd->files[i] != NULL) // Check if cmd->files[i] is not NULL
		{
			if (cmd->files[i]->type == OUTFILE)
				printf("OUTFILE: %s    %d", cmd->files[i]->file, cmd->files[i]->expand_error);
			else if (cmd->files[i]->type == APPEND)
				printf("APPEND: %s     %d", cmd->files[i]->file, cmd->files[i]->expand_error);
			else if (cmd->files[i]->type == HEREDOC)
				printf("HEREDOC: %s    %d", cmd->files[i]->file, cmd->files[i]->expand_error);
			else if (cmd->files[i]->type == INFILE)
				printf("INFILE: %s     %d", cmd->files[i]->file, cmd->files[i]->expand_error);
		}
		i++;
	}
	printf("\n");
}

void	initialize_cmd_and_set_flags(t_cmd *cmds,
		t_indexes *indexes, char **split, t_data *data)
{
	initialize_cmd(&cmds[indexes->i]);
	cmds[indexes->i].outfile = count_str_in_2d_array(">", split);
	cmds[indexes->i].append = count_str_in_2d_array(">>", split);
	cmds[indexes->i].heredoc = count_str_in_2d_array("<<", split);
	cmds[indexes->i].infile = count_str_in_2d_array("<", split);
	cmds[indexes->i].flag_outfiles = find_last_str("<", "<<", split);
	cmds[indexes->i].flag_infiles = find_last_str(">", ">>", split);
	cmds[indexes->i].files = ft_malloc(sizeof(t_redir *)
			* (cmds[indexes->i].heredoc + cmds[indexes->i].append
				+ cmds[indexes->i].outfile + cmds[indexes->i].infile + 1),
			&data->allocated);
	cmds[indexes->i].files[cmds[indexes->i].heredoc + cmds[indexes->i].append
		+ cmds[indexes->i].outfile + cmds[indexes->i].infile] = NULL;
}

void	fill_command(t_data *data)
{
	// t_cmd		cmds[data->counter_command];
	char		**split;
	t_indexes	indexes;
	t_cmd* cmds = ft_malloc(sizeof(t_cmd) * data->counter_command, &data->allocated);
	indexes = (t_indexes){0, 0, 0, 0};
	while (indexes.i < data->counter_command)
	{
		indexes.j = 0;
		indexes.l = 0;
		indexes.k = 0;
		split = ft_split_str(data->pipes[indexes.i], " \t", data);
		initialize_cmd_and_set_flags(cmds, &indexes, split, data);
		while (split[indexes.j])
		{
			quote_replace(split[indexes.j], -6, ' ');
			quote_replace(split[indexes.j], -42, '\t');
			handle_redirections(cmds, &indexes, split, data);
			indexes.j++;
		}
		indexes.j = 0;
		handle_cmd_allocation(&cmds[indexes.i], &indexes.j, split, data);
		indexes.l = indexes.j;
		indexes.k = 0;
		count_cmd_args(&indexes.j, &indexes.k, split);
		cmds[indexes.i].args
			= ft_malloc((sizeof(char *) * (indexes.k +1)), &data->allocated);
		cmds[indexes.i].args[indexes.k] = NULL;
		cmds[indexes.i].args_number = indexes.k;
		indexes.k = 0;
		indexes.j = indexes.l;
		handle_cmd_args(&cmds[indexes.i], &indexes.j, &indexes.k, split);
		indexes.i++;
	}
	
	ambigious(cmds, data);
	remove_quotes(cmds, data);
	// printf("ttttt%s\n", cmds[0].cmd);
	data->cmds = cmds;
	// indexes.i = 0;
	// while (indexes.i < data->counter_command)
	// {
	// 	print_cmd(&cmds[indexes.i]);
	// 	indexes.i++;
	// }
}
