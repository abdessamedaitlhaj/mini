/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 01:55:49 by ael-hara          #+#    #+#             */
/*   Updated: 2024/08/04 01:57:06 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ascii_to_quotes(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == -45)
			str[i] = '"';
		if (str[i] == -46)
			str[i] = '\'';
		i++;
	}
}

void	quotes_to_ascii(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '"')
			str[i] = -45;
		if (str[i] == '\'')
			str[i] = -46;
		i++;
	}
}
