/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 02:34:27 by ael-hara          #+#    #+#             */
/*   Updated: 2024/08/01 20:21:41 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int	ft_numlen(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n, t_data *data)
{
	char	*str;
	long	nbr;
	int		len;

	nbr = n;
	len = ft_numlen(nbr);
	str = ft_malloc(len + 1, &data->allocated);
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (nbr < 0)
	{
		str[0] = '-';
		nbr = -nbr;
	}
	if (nbr == 0)
		str[0] = '0';
	while (nbr > 0)
	{
		str[--len] = (nbr % 10) + '0';
		nbr /= 10;
	}
	return (str);
}

t_data	init_main(char **envp)
{
	t_data	data;

	data = (t_data){NULL, NULL, 0, NULL, NULL, NULL, 0,
		NULL, 0, 0, -2, -2, envp, 0, NULL};
	init_envs(envp, &data);
	signal(SIGINT, int_handler);
	signal(SIGQUIT, SIG_IGN);
	return (data);
}
