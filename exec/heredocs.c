/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 22:45:52 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/23 21:13:50 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*do_heredoc(t_token *token)
{
	char	*finale;
	char	*tmp;
	char	*line;

	finale = NULL;
	tmp = NULL;
	line = NULL;
	while (1)
	{
		write(1, ">", 1);
		if (tmp)
			free (tmp);
		line = get_next_line(0);
		if ((token->value[0] == 0 && line[0] == '\n')
			|| ft_strncmp(line, token->value, ft_strlen(token->value) == 0))
		{
			if (line)
				free (line);
			return (finale);
		}
		finale = ft_strjoin(line, finale);
		free (line);
		tmp = finale;
	}
}
