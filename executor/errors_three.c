/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_three.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:30:08 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/23 15:19:47 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

void	fail_error(char *str, t_list **garbage_collector)
{
	ft_putstr_fd("minishell: ", 2);
	perror(str);
	free_allocated(garbage_collector);
	exit(1);
}