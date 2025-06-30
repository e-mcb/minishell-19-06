/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input_token.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 23:29:25 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/30 19:16:36 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	token_error(t_shell *shell)
{
	t_token	*tmp;

	tmp = shell->token;
	while (tmp)
	{
		if (tmp->type == OUT && tmp->next->type == PIPE)
			return (ft_putstr_fd(NOCLOBBER, 2), 1);
		if ((tmp->type == IN || tmp->type == APPEND
				|| tmp->type == HDOC) && tmp->next->type == PIPE)
			return (ft_putstr_fd("syntax error near token '|'", 2), 1);
		if ((tmp->type == IN || tmp->type == APPEND
				|| tmp->type == HDOC || tmp->t)
		tmp = tmp->next;
	}
}
