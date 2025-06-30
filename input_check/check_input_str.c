/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input_str.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 23:19:50 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/30 19:15:23 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	string_error(char *input)
{
	int	i;

	i = 0;
	if (input[0] == '|' || input[ft_strlen(input - 1)] == '|')
	{
		ft_putstr_fd(PIPE_FIRST_LAST, 2);
		return (1);
	}
	while (input[i++])
	{
		
	}
	// what to add?
}
