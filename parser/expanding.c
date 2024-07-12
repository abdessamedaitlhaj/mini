/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:05:58 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/12 17:14:08 by aait-lha         ###   ########.fr       */
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
		if (env_str[i] == '=' && \
			ft_strncmp(env_str, key, i) == 0 && key[i] == '\0')
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
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || ft_isnum(c))
		return (1);
	return (0);
}

char *expanding_outside (char *pipe, t_data *data)
{
	int i = 0;
	while (pipe[i])
	{
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
		if (pipe[i + 1] &&  pipe[i + 1] == '?') // if the character following $ is ?
    	{
        char *exit_status_str = ft_itoa(data->exit_status, data); // convert the exit status to a string
        char *before = ft_substr(pipe, 0, i, data); // get the part of the string before $?
        char *after = ft_substr(pipe, i + 2, ft_strlen(pipe) - i - 2, data); // get the part of the string after $?
        char *new_pipe = ft_strjoin(before, exit_status_str, data); // join the part before $? and the exit status
        pipe = ft_strjoin(new_pipe, after, data); // join the previous string and the part after $?
        // free(pipe); // free the old string
        // pipe = new_pipe; // assign the new string to pipe
        i += ft_strlen(exit_status_str) - 1; // adjust i based on the length of the exit status string
    	}
		else if ((pipe[i + 1] && !(ft_isalnum(pipe[i + 1]) || pipe[i + 1] == '_' || pipe[i + 1] == '"' || pipe[i + 1] == '\'')) || !pipe[i + 1])
    	{
    		i++;
       		continue;
		}
			else if (pipe[i + 1] && pipe[i + 1] && ft_isnum(pipe[i + 1]))
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
							// printf("before: %s\n", before);
							// printf("after: %s\n", after);
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
							continue ;
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
			if (inside_double_quotes) // if we are inside double quotes
			{
				int j = i + 1;
				while (pipe[j] && pipe[j] != '"') // find the end of the content inside double quotes
					j++;
				char *content = ft_substr(pipe, i, j - i + 1, data); // get the content inside double quotes, including the quotes
				char *expanded = expanding_inside(content, data);  // expand the content
				result = ft_strjoin(result, expanded, data); // append the expanded content to result
				i = j; // move i to the end of the content inside double quotes
			}
		}
		else if (pipe[i] == '\'')
		{
			inside_single_quotes = !inside_single_quotes; // toggle the inside_single_quotes flag
			if (inside_single_quotes) // if we are inside single quotes
			{
				int j = i + 1;
				while (pipe[j] && pipe[j] != '\'') // find the end of the content inside single quotes
					j++;
				char *content = ft_substr(pipe, i, j - i + 1, data); // get the content inside single quotes, including the quotes
				result = ft_strjoin(result, content, data); // append the content to result
				i = j; // move i to the end of the content inside single quotes
			}
		}
		else
		{
			char *tmp = ft_substr(pipe, i, 1, data); // get the current character
			result = ft_strjoin(result, tmp, data); // append to result
		}
		if (!inside_double_quotes && !inside_single_quotes) // only increment i if we're not inside quotes
			i++;
	}
	return (result);
}

// char *expanding_final(char *pipe, t_data *data)
// {
// 	int i = 0;
// 	int inside_double_quotes = 0;
// 	int inside_single_quotes = 0;
// 	char *result = ft_strdup("", &data->allocated); // initialize result to an empty string
// 	printf("pipe: %s\n", pipe);
// 	while (pipe[i])
// 	{
// 		if (pipe[i] == '"')
// 		{
// 			inside_double_quotes = !inside_double_quotes; // toggle the inside_double_quotes flag
// 			if (inside_double_quotes) // if we are inside double quotes
// 			{
// 				int j = i + 1;
// 				while (pipe[j] && pipe[j] != '"') // find the end of the content inside double quotes
// 					j++;
// 				char *content = ft_substr(pipe, i, j - i + 1, data); // get the content inside double quotes, including the quotes
// 				char *expanded = expanding_inside(content, data);  // expand the content
// 				result = ft_strjoin(result, expanded, data); // append the expanded content to result
// 				i = j; // move i to the end of the content inside double quotes
// 			}
// 		}
// 		// else if (pipe[i] == '\'')
// 		// {
// 		// 	inside_single_quotes = !inside_single_quotes; // toggle the inside_single_quotes flag
// 		// 	if (inside_single_quotes) // if we are inside single quotes
// 		// 	{
// 		// 		int j = i + 1;
// 		// 		while (pipe[j] && pipe[j] != '\'') // find the end of the content inside single quotes
// 		// 			j++;
// 		// 		i = j; // move i to the end of the content inside single quotes
// 		// 	}
// 		// }
// 		else if (pipe[i] == '\'')
// 		{
// 			inside_single_quotes = !inside_single_quotes; // toggle the inside_single_quotes flag
// 			if (inside_single_quotes) // if we are inside single quotes
// 			{
// 				int j = i + 1;
// 				while (pipe[j] && pipe[j] != '\'') // find the end of the content inside single quotes
// 					j++;
// 				char *content = ft_substr(pipe, i, j - i + 1, data); // get the content inside single quotes, including the quotes
// 				result = ft_strjoin(result, content, data); // append the content to result
// 				i = j; // move i to the end of the content inside single quotes
// 			}
// 		}
// 		else
// 		{
// 			char *tmp = ft_substr(pipe, i, 1, data); // get the current character
// 			result = ft_strjoin(result, tmp, data); // append to result
// 			i++;
// 		}
// 	}
// 	printf("res: %s\n", result);
// 	return (result);
// }



// char *expanding_final(char *pipe, t_data *data)
// {
// 	int i = 0;
// 	int inside_double_quotes = 0;
// 	int inside_single_quotes = 0;
// 	char *result = ft_strdup("", &data->allocated); // initialize result to an empty string

// 	while (pipe[i])
// 	{
// 		if (pipe[i] == '"')
// 		{
// 			inside_double_quotes = !inside_double_quotes; // toggle the inside_double_quotes flag
// 			if (inside_double_quotes) // if we are inside double quotes
// 			{
// 				int j = i + 1;
// 				while (pipe[j] && pipe[j] != '"') // find the end of the content inside double quotes
// 					j++;
// 				char *content = ft_substr(pipe, i + 1, j - i - 1, data); // get the content inside double quotes
// 				char *expanded = expanding_inside(content, data);  // expand the content
// 				// char *old_result = result;
// 				result = ft_strjoin(result, expanded, data); // append the expanded content to result
// 				// free(old_result); // free the old result
// 				i = j; // move i to the end of the content inside double quotes
// 			}
// 		}
// 		else if (pipe[i] == '\'')
// 		{
// 			inside_single_quotes = !inside_single_quotes; // toggle the inside_single_quotes flag
// 			if (inside_single_quotes) // if we are inside single quotes
// 			{
// 				int j = i + 1;
// 				while (pipe[j] && pipe[j] != '\'') // find the end of the content inside single quotes
// 					j++;
// 				i = j; // move i to the end of the content inside single quotes
// 			}
// 		}
// 		else
// 		{
// 			char *tmp = ft_substr(pipe, i, 1, data); // get the current character
// 			char *old_result = result;
// 			result = ft_strjoin(result, tmp, data); // append to result
// 			free(old_result); // free the old result
// 			i++;
// 		}
// 	}
// 	printf("result: %s\n", result);
// 	return (result);
// }


char *expanding_inside (char *pipe, t_data *data)
{
	int i = 0;
	while (pipe[i])
	{
		if (pipe[i] == '$')
		{
			if (pipe[i + 1] &&  pipe[i + 1] == '?') // if the character following $ is ?
    		{
        	char *exit_status_str = ft_itoa(data->exit_status, data); // convert the exit status to a string
        	char *before = ft_substr(pipe, 0, i, data); // get the part of the string before $?
        	char *after = ft_substr(pipe, i + 2, ft_strlen(pipe) - i - 2, data); // get the part of the string after $?
        	char *new_pipe = ft_strjoin(before, exit_status_str, data); // join the part before $? and the exit status
        	pipe = ft_strjoin(new_pipe, after, data); // join the previous string and the part after $?
        	// free(pipe); // free the old string
        	// pipe = new_pipe; // assign the new string to pipe
        	i += ft_strlen(exit_status_str) - 1; // adjust i based on the length of the exit status string
    		}
			else if ((pipe[i + 1] && !(ft_isalnum(pipe[i + 1]) || pipe[i + 1] == '_')) || !pipe[i + 1])
    		{
        		i++;
        		continue;
			}
			else if (pipe[i + 1] && ft_isnum(pipe[i + 1]))
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
