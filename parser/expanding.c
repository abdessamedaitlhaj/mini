/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 04:31:02 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/30 15:18:08 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	final_double(char **pipe, int *i, char **result, t_data *data)
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

void	final_single(char **pipe, int *i, char **result, t_data *data)
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

void	final_case(char **pipe, int *i, char **result, t_data *data)
{
	char	*tmp;

	tmp = ft_substr(*pipe, *i, 1, data);
	*result = ft_strjoin(*result, tmp, data);
}

void	initialize_idx_and_result(t_indexes *idx, char **result, t_data *data)
{
	idx->i = 0;
	idx->j = 0;
	idx->k = 0;
	*result = ft_strdup("", &data->allocated);
}

char	*expanding_final(char *pipe, t_data *data)
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
