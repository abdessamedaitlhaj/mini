/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 08:45:10 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/29 21:26:28 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_empty_args(t_data *data, char **args)
{
	if (!args[0])
	{
		print_env(data->env);
		return (1);
	}
	return (0);
}

int	ft_isalnm(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || \
	(c >= '0' && c <= '9'))
		return (1);
	return (0);
}

char	*extract_key(t_key_value *k_v, char *arg, t_data *data)
{
	int		i;
	char	*key;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	key = malloc(sizeof(char) * (i + 1));
	if (!key)
		fail_error("malloc", &data->allocated);
	i = 0;
	while (arg[i] && arg[i] != '=')
	{
		if (arg[i] == '+' && arg[i + 1] == '=')
		{
			k_v->append = 1;
			break ;
		}
		key[i] = arg[i];
		i++;
	}
	key[i] = '\0';
	return (key);
}

void	*ft_new_env(char *key, char	*value, t_data *data)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		fail_error("malloc", &data->allocated);
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

void	ft_add_env(t_env **env, t_env *new)
{
	t_env	*last;

	if (!*env)
	{
		*env = new;
		return ;
	}
	last = *env;
	while (last->next)
		last = last->next;
	last->next = new;
}
