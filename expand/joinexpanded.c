/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinexpanded.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:27:36 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/18 22:04:35 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	array_len(char **str)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (str[i])
	{
		len += ft_strlen(str[i]);
		i++;
	}
	return (len);
}

char	*join_chars(char **str, t_shell *shell)
{
	int		i;
	int		j;
	int		k;
	char	*expanded;

	if (!str)
		return (NULL);
	expanded = malloc(sizeof(char) * (array_len(str) + 1));
	if (!expanded)
		ft_clean_exit(NULL, shell, NULL, NULL);
	i = 0;
	k = 0;
	while (str[i])
	{
		j = 0;
		while (str[i][j])
		{
			expanded[k] = str[i][j];
			j++;
			k++;
		}
		i++;
	}
	expanded[k] = '\0';
	return (ft_free_str_array(str), expanded);
}
