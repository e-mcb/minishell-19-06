/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cases2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:35:10 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/27 18:14:48 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_isspace(int c)
{
	c = (unsigned char)c;
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ')
		return (1);
	return (0);
}

void	case_substitute(t_expand *ex, char *input, t_shell *shell)
{
	(void) shell;
	while (input[ex->i] && !ft_isspace(input[ex->i])
		&& input[ex->i] != '"' && input[ex->i] != '\'' && input[ex->i] != '$'
		&& (ft_isalnum(input[ex->i]) || input[ex->i] == '_'))
		ex->i++;
	ex->result[ex->j + 1] = ft_strndup(input + ex->start + 2, ex->i - ex->start - 2);
	printf("%s\n", ex->result[ex->j + 1]);
	ex->j++;
	// if (!ex->result[ex->j])
	// 	ft_clean_exit(NULL, shell, NULL, NULL);
	// printf("%d\n", ex->j);
	ex->start = ex->i;
}
