/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:07:17 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/25 20:24:52 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}

int	ft_isalpha(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

void	ft_putendl_fd(char *s, int fd)
{
	ft_putstr_fd(s, fd);
	ft_putstr_fd("\n", fd);
}

char	*ft_strjoin2(char *s1, char *s2, char *s3, t_data *data)
{
	int		len;
	int		i;
	char	*str;

	i = 0;
	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	str = ft_malloc(len + 1, &data->allocated);
	while (*s1)
		str[i++] = *s1++;
	while (*s2)
		str[i++] = *s2++;
	while (*s3)
		str[i++] = *s3++;
	str[i] = '\0';
	return (str);
}
