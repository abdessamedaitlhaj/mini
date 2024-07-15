/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:05:58 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/15 06:51:41 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strncmp(char const *s1, char const *s2, size_t n)
{
	if (n == 0)
	{
		return (0);
	}
	while (*s1 == *s2 && *s1 && *s2 && n > 1)
	{
		s1++;
		s2++;
		n--;
	}
	return (((unsigned char)(*s1) - (unsigned char)(*s2)));
}

char	*get_env_value(char *key, t_list *env)
{
	char	*env_str;
	int		i;

	while (env != NULL)
	{
		env_str = (char *)env->content;
		i = 0;
		while (env_str[i] != '\0' && env_str[i] != '=')
			i++;
		if (env_str[i] == '=' && ft_strncmp(env_str, key, i) == 0
			&& key[i] == '\0')
		{
			return (env_str + i + 1);
		}
		env = env->next;
	}
	return (NULL);
}

int	ft_isnum(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_isalnum(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || ft_isnum(c))
		return (1);
	return (0);
}

int	skip_redirection(char *pipe, int i, int flag)
{
	if (!flag)
		i += 2;
	else 
		i++;
	while (pipe[i] && ft_isspace(pipe[i]))
		i++;
	while (pipe[i] && !ft_isspace(pipe[i]))
		i++;
	return (i);
}

char	*expand_exit(char *pipe, int *i, t_data *data)
{
	char	*exit_status_str;
	char	*before;
	char	*after;
	char	*new_pipe;

	exit_status_str = ft_itoa(data->exit_status, data);
	before = ft_substr(pipe, 0, *i, data);
	after = ft_substr(pipe, *i + 2, ft_strlen(pipe) - *i - 2, data);
	new_pipe = ft_strjoin(before, exit_status_str, data);
	pipe = ft_strjoin(new_pipe, after, data);
	*i += ft_strlen(exit_status_str) - 1;
	return (pipe);
}

void expand_num(char **pipe, int i, t_data *data)
{
	char	*before;
	char	*after;
	char	*tmp;
	
	before = ft_substr(*pipe, 0, i, data);
	after = ft_substr(*pipe, i + 2, ft_strlen(*pipe) - (i + 2), data);
	tmp = ft_strjoin(before, after, data);
	*pipe = tmp;
}

void expand_case(char **pipe, int *i, t_data *data)
{
	char	*before;
	char	*after;
	char	*tmp;

	before = ft_substr(*pipe, 0, *i, data);
	after = ft_substr(*pipe, *i + 1, ft_strlen(*pipe) - (*i + 1), data);
	tmp = ft_strjoin(before, after, data);
	*pipe = tmp;
}

void expand_exist(char **pipe, int *i, int j, char *var_value, t_data *data)
{
	char	*before;
	char	*after;
	char	*tmp;

	before = ft_substr(*pipe, 0, *i, data);
	after = ft_substr(*pipe, j, ft_strlen(*pipe) - j, data);
	tmp = ft_strjoin(before, var_value, data);
	tmp = ft_strjoin(tmp, after, data);
	*pipe = tmp;
}

void expand_flag(char **pipe, int *i, int j, t_data *data)
{
	char	*before;
	char	*after;
	char	*tmp;

	before = ft_substr(*pipe, 0, *i, data);
	after = ft_substr(*pipe, j, ft_strlen(*pipe) - j, data);
	tmp = ft_strjoin(before, after, data);
	*pipe = tmp;
	(*i)--;
}

void expand_var(char **pipe, int *i, t_data *data)
{
	char	*var_value;
	int		j;
	int		flag;
	int		z;

	j = *i + 1;
	while ((*pipe)[j] && (ft_isalnum((*pipe)[j]) || (*pipe)[j] == '_'))
		j++;
	var_value = get_env_value(ft_substr(*pipe, *i + 1, j - *i - 1, data),
			data->env);
	if (var_value)
		expand_exist(pipe, i, j, var_value, data);
	else
	{
		flag = 0;
		z = *i - 1;
		while (z > 0 && ft_isspace((*pipe)[z]))
			z--;
		if ((*pipe)[z] == '>' || (*pipe)[z] == '<')
			flag = 1;
		if (!flag)
		{
			expand_flag(pipe, i, j, data);
		}
	}
}

int expand_help(char **pipe, int *i, t_data *data)
{
	if ((*pipe)[*i + 1] && (*pipe)[*i + 1] == '?')
	{
		*pipe = expand_exit(*pipe, i, data);
	}
	else if (((*pipe)[*i + 1] && !((*pipe)[*i + 1] == '_'
		|| ft_isalnum((*pipe)[*i + 1]) || (*pipe)[*i + 1] == '"'
			|| (*pipe)[*i + 1] == '\'')) || !(*pipe)[*i + 1])
	{
		(*i)++;
		return (-1);
	}
	else if ((*pipe)[*i + 1] && ft_isnum((*pipe)[*i + 1]))
	{
		expand_num(pipe, *i, data);
	}
	else if ((*pipe)[*i + 1] && ((*pipe)[*i + 1] == '"'
		|| (*pipe)[*i + 1] == '\''))
	{
		expand_case(pipe, i, data);
	}
	else if ((*pipe)[*i + 1] && ft_isspace((*pipe)[*i + 1]))
		(*i)++;
	else
		expand_var(pipe, i, data);
	return (0);
}

char *expanding_outside(char *pipe, t_data *data)
{
	int	i;

	i = -1;
	while (pipe[++i])
	{
		if ((pipe[i] == '<' || pipe[i] =='>') && pipe[i + 1] &&( pipe[i + 1] == '<' || pipe[i + 1] == '>'))
	{			
	i = skip_redirection(pipe, i, 0);
			printf("pipe[i]: %s\n", pipe + i);
	}		
			else if (pipe[i] == '<'  || pipe[i] == '>')
			{
			i = skip_redirection(pipe, i, 1);
			}
		else if (pipe[i] == '$')
		{
			if (expand_help(&pipe, &i, data) == -1)
			{
				continue ;
			}
		}
	}
	return (pipe);
}

void inside_exit(char **pipe, int *i, t_data *data)
{
	char	*exit_status_str;
	char	*before;
	char	*after;
	char	*new_pipe;

	exit_status_str = ft_itoa(data->exit_status, data);
	before = ft_substr(*pipe, 0, *i, data);
	after = ft_substr(*pipe, *i + 2, ft_strlen(*pipe) - *i - 2, data);
	new_pipe = ft_strjoin(before, exit_status_str, data);
	*pipe = ft_strjoin(new_pipe, after, data);
	*i += ft_strlen(exit_status_str) - 1;
}

void inside_number(char **pipe, int *i, t_data *data)
{
	char	*before;
	char	*after;
	char	*tmp;

	before = ft_substr(*pipe, 0, *i, data);
	after = ft_substr(*pipe, *i + 2, ft_strlen(*pipe) - (*i + 2), data);
	tmp = ft_strjoin(before, after, data);
	*pipe = tmp;
}

void inside_quote_ex(char **pipe, int i, t_data *data)
{
	char	*before;
	char	*after;
	char	*tmp;

	before = ft_substr(*pipe, 0, i, data);
	after = ft_substr(*pipe, i , ft_strlen(*pipe) - i, data);
	tmp = ft_strjoin(before, after, data);
	*pipe = tmp;
}

void normal_notfound(char **pipe, int i, int j, t_data *data)
{
	char	*before;
	char	*after;
	char	*tmp;

	before = ft_substr(*pipe, 0, i, data);
	after = ft_substr(*pipe, j, ft_strlen(*pipe) - j, data);
	tmp = ft_strjoin(before, after, data);
	*pipe = tmp;
}

char *normal_expand(char *pipe, int i, t_data *data)
{
	int		j;
	char	*var_value;
	char	*before;
	char	*after;
	char	*tmp;

	j = i + 1;
	while (pipe[j] && (ft_isalnum(pipe[j]) || pipe[j] == '_'))
		j++;
	var_value = get_env_value(ft_substr(pipe, i + 1, j - i - 1, data),
			data->env);
	if (var_value)
	{
		before = ft_substr(pipe, 0, i, data);
		after = ft_substr(pipe, j, ft_strlen(pipe) - j, data);
		tmp = ft_strjoin(before, var_value, data);
		tmp = ft_strjoin(tmp, after, data);
		pipe = tmp;
	}
	else
		normal_notfound(&pipe, i, j, data);
	return (pipe);
}

int inside_cases(char **pipe, int *i, t_data *data)
{
	if ((*pipe)[*i + 1] && (*pipe)[*i + 1] == '?')
		inside_exit(pipe, i, data);
	else if (((*pipe)[*i + 1] && !(ft_isalnum((*pipe)[*i + 1])
		|| (*pipe)[*i + 1] == '_')) || !(*pipe)[*i + 1])
	{
		(*i)++;
		return -1;
	}
	else if ((*pipe)[*i + 1] && ft_isnum((*pipe)[*i + 1]))
		inside_number(pipe, i, data);
	else if ((*pipe)[*i + 1] && ((*pipe)[*i + 1] == '"'
		|| (*pipe)[*i + 1] == '\''))
		inside_quote_ex(pipe, *i, data);
	else if ((*pipe)[*i + 1] && ft_isspace((*pipe)[*i + 1]))
		(*i)++;
	else
		*pipe = normal_expand(*pipe, *i, data);
	return (0);
}

char	*expanding_inside(char *pipe, t_data *data)
{
	int	i;

	i = -1;
	while (pipe[++i])
	{
		if (pipe[i] == '$')
		{
			if (inside_cases(&pipe, &i, data) == -1)
				continue ;
		}
	}
	return (pipe);
}

// char *expanding_final(char *pipe, t_data *data)
// {
// 	int		arr[3];
// 	char	*result;

// 	(1) && (arr[0] = 0) && (arr[1] = 0) && (arr[2] = 0);
// 	result = ft_strdup("", &data->allocated);
// 	while (pipe[arr[0]])
// 	{
// 		if (pipe[arr[0]] == '"')
// 		{
// 			arr[1] = !arr[1];
// 			if (arr[1])
// 				expand_inside_2(&pipe, &arr[0], &result, data);
// 		}
// 		else if (pipe[arr[0]] == '\'')
// 		{
// 			arr[2] = !arr[2];
// 			if (arr[2])
// 				expand_inside_3(&pipe, &arr[0], &result, data);
// 		}
// 		else
// 		{
// 			result = ft_strjoin(result, ft_substr(pipe, arr[0], 1, data), data);
// 		}
// 		if (!arr[1] && !arr[2])
// 			arr[0]++;
// 	printf("pipe: %s\n", pipe);
// 	}
// 	return (result);
// }

void final_double(char **pipe, int *i, char **result, t_data *data)
{
	int		j;
	char	*content;
	char	*expanded;

	j = *i + 1;
	while ((*pipe)[j] && (*pipe)[j] != '"')
		j++;
	content = ft_substr(*pipe, *i, j - *i + 1, data);
	expanded = expanding_inside(content, data);
	*result = ft_strjoin(*result, expanded, data);
	*i = j;
}

void final_single(char **pipe, int *i, char **result, t_data *data)
{
	int		j;
	char	*content;

	j = *i + 1;
	while ((*pipe)[j] && (*pipe)[j] != '\'')
		j++;
	content = ft_substr(*pipe, *i, j - *i + 1, data);
	*result = ft_strjoin(*result, content, data);
	*i = j;
}

void final_case(char **pipe, int *i, char **result, t_data *data)
{
	char *tmp;

	tmp = ft_substr(*pipe, *i, 1, data);
	*result = ft_strjoin(*result, tmp, data);
}

void initialize_idx_and_result(t_indexes *idx, char **result, t_data *data)
{
	idx->i = 0;
	idx->j = 0;
	idx->k = 0;
	*result = ft_strdup("", &data->allocated);
}

// j is for double quotes
// k is for single quotes

char *expanding_final(char *pipe, t_data *data)
{
	t_indexes	idx;
	char		*result;

	initialize_idx_and_result(&idx, &result, data);
	while (pipe[idx.i])
	{
		if (pipe[idx.i] == '"')
		{
			idx.j = !idx.j;
			if (idx.j)
				final_double(&pipe, &idx.i, &result, data);
		}
		else if (pipe[idx.i] == '\'')
		{
			idx.k = !idx.k;
			if (idx.k)
				final_single(&pipe, &idx.i, &result, data);
		}
		else
			final_case(&pipe, &idx.i, &result, data);
		if (!idx.j && !idx.k)
			idx.i++;
	}
	return (result);
}
