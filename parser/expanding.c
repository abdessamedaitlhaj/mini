/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:05:58 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/07 11:52:26 by ael-hara         ###   ########.fr       */
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

char *get_env_value(char *key, t_list *env)
{
	while (env != NULL)
	{
		char *env_str = (char *)env->content;
		int i = 0;
		while (env_str[i] != '\0' && env_str[i] != '=')
			i++;
		if (env_str[i] == '=' && ft_strncmp(env_str, key, i) == 0 && key[i] == '\0')
		{
			return env_str + i + 1;
		}
		env = env->next;
	}
	return NULL;
}

int ft_isnum(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int ft_isalnum(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

char *expanding_outside (char *pipe, t_data *data)
{
	
	int i = 0;
	while (pipe[i])
	{
		
						// printf("segmenddfdfdfdtation fault\n");	
		// if << skip the spaces and the next string of characters 
		if (pipe[i] == '<' &&  pipe[i + 1] && pipe[i + 1] && pipe[i + 1] == '<')
		{
			i += 2;
			while (pipe[i] && ft_isspace(pipe[i]))
				i++;
			while (pipe[i] && !ft_isspace(pipe[i]))
				i++;
		}
		else if (pipe[i] == '$')
		{
			 if ((pipe[i + 1] && !(ft_isalnum(pipe[i + 1]) || pipe[i + 1] == '_')) || !pipe[i + 1])
    		{
        		i++;
        		continue;
			}
			if (pipe[i + 1] && pipe[i + 1] && ft_isnum(pipe[i + 1]))
			{
				char *before = ft_substr(pipe, 0, i, data); // get the part before the $
				char *after = ft_substr(pipe, i + 2, ft_strlen(pipe) - (i + 2), data); // get the part after the $ and the first number
				char *tmp = ft_strjoin(before, after, data); // join before and after
				pipe = tmp;
			}
			else if (pipe[i + 1] && pipe[i + 1] && (pipe[i + 1] == '"' || pipe[i + 1] == '\''))
			{
				char *before = ft_substr(pipe, 0, i, data); // get the part before the $
                            char *after = ft_substr(pipe, i + 1, ft_strlen(pipe) - (i + 1), data); // get the part after the $ (including the quote)
                            char *tmp = ft_strjoin(before, after, data); // join before and after
                            pipe = tmp;
			}
			else if (pipe[i + 1] && pipe[i + 1] && ft_isspace(pipe[i + 1]))
				i++;		
				else 
				{
					int j = i + 1;
					while (pipe[j] && (ft_isalnum(pipe[j]) || pipe[j] == '_'))
						j++;
					char *var_name = ft_substr(pipe, i + 1, j - i - 1, data); // get the variable name
					// char *var_value = getenv(var_name); // get the value of the variable from the environment
					char *var_value = get_env_value(var_name, data->env);
					if (var_value)
					{
						char *before = ft_substr(pipe, 0, i, data); // get the part before the $
						char *after = ft_substr(pipe, j, ft_strlen(pipe) - j, data); // get the part after the variable name
						char *tmp = ft_strjoin(before, var_value, data); // join before and variable value
						tmp = ft_strjoin(tmp, after, data); // join with the part after the variable name
						pipe = tmp;
					}
					else
					{
							int flag =0;
							int z = i - 1;
							while (z > 0 && ft_isspace(pipe[z]))
								z--;
							if (pipe[z] =='>' || pipe[z] =='<')
								flag = 1;
							if (!flag)
							{
							char *before = ft_substr(pipe, 0, i, data); // Get the part before the $
							char *after = ft_substr(pipe, j, ft_strlen(pipe) - j, data); // Get the part after the variable name
							char *tmp = ft_strjoin(before, after, data); 
							pipe = tmp; // Update pipe to the new string without the variable name
							}			
					}
				}
			}
			i++;
	}
	return (pipe);
}

char *expanding_final(char *pipe, t_data *data)
{
	int i = 0;
	int inside_double_quotes = 0;
	int inside_single_quotes = 0;
	char *result = ft_strdup("", &data->allocated); // initialize result to an empty string

	while (pipe[i])
	{
		if (pipe[i] == '"')
		{
			inside_double_quotes = !inside_double_quotes; // toggle the inside_double_quotes flag
		}
		else if (pipe[i] == '\'')
		{
			inside_single_quotes = !inside_single_quotes; // toggle the inside_single_quotes flag
		}

		if (!inside_single_quotes && inside_double_quotes && pipe[i] == '$')
		{
			int j = i;
			while (pipe[j] && pipe[j] != '"' && pipe[j] != ' ') // find the end of the variable
				j++;
			char *var = ft_substr(pipe, i, j - i, data); // get the variable
			char *expanded = expanding_inside(var, data);  // expand the variable
			result = ft_strjoin(result, expanded, data); // append the expanded variable to result
			i = j - 1; // move i to the end of the expanded variable
			return result;
		}
		else
		{
			char *tmp = ft_substr(pipe, i, 1, data); // get the current character
			result = ft_strjoin(result, tmp, data); // append to result
			// printf("expanded: %s\n", result);
			i++;
		}
	}

	return (result);
}

char *expanding_inside (char *pipe, t_data *data)
{
	
	int i = 0;
	while (pipe[i])
	{
		if (pipe[i] == '$')
		{
			 if ((pipe[i + 1] && !(ft_isalnum(pipe[i + 1]) || pipe[i + 1] == '_')) || !pipe[i + 1])
    		{
        		i++;
        		continue;
			}
			if (pipe[i + 1] && ft_isnum(pipe[i + 1]))
			{
				char *before = ft_substr(pipe, 0, i, data); // get the part before the $
				char *after = ft_substr(pipe, i + 2, ft_strlen(pipe) - (i + 2), data); // get the part after the $ and the first number
				char *tmp = ft_strjoin(before, after, data); // join before and after
				pipe = tmp;
			}
			else if (pipe[i + 1] && (pipe[i + 1] == '"' || pipe[i + 1] == '\''))
			{
				char *before = ft_substr(pipe, 0, i, data); // get the part before the $
                            char *after = ft_substr(pipe, i , ft_strlen(pipe) - (i), data); // get the part after the $ (including the quote)
                            char *tmp = ft_strjoin(before, after, data); // join before and after
                            pipe = tmp;
			}
			else if (pipe[i + 1] && ft_isspace(pipe[i + 1]))
				i++;
				else 
				{
					int j = i + 1;
					while (pipe[j] && (ft_isalnum(pipe[j]) || pipe[j] == '_'))
						j++;
					char *var_name = ft_substr(pipe, i + 1, j - i - 1, data); // get the variable name
					// char *var_value = getenv(var_name); // get the value of the variable from the environment
					char *var_value = get_env_value(var_name, data->env);
					if (var_value)
					{
						char *before = ft_substr(pipe, 0, i, data); // get the part before the $
						char *after = ft_substr(pipe, j, ft_strlen(pipe) - j, data); // get the part after the variable name
						char *tmp = ft_strjoin(before, var_value, data); // join before and variable value
						tmp = ft_strjoin(tmp, after, data); // join with the part after the variable name
						pipe = tmp;
					}
					else
					{	
						char *before = ft_substr(pipe, 0, i, data); // Get the part before the $
						char *after = ft_substr(pipe, j, ft_strlen(pipe) - j, data); // Get the part after the variable name
						char *tmp = ft_strjoin(before, after, data); 
						pipe = tmp; // Update pipe to the new string without the variable name							
					}
				}
			}
			i++;
	}
	return (pipe);
}