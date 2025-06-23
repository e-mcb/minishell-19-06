/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfinder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 16:16:08 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/23 21:05:34 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	free_possible_paths(char **possible_paths)
{
	int	i;

	i = 0;
	while (possible_paths[i])
	{
		free(possible_paths[i]);
		i++;
	}
	free(possible_paths);
}

static char	*construct_final_path(char *path, char *cmd)
{
	char	*tmp_path;
	char	*final_path;

	if (!cmd)
		return (NULL);
	tmp_path = ft_strjoin(path, "/");
	final_path = ft_strjoin(tmp_path, cmd);
	// ft_putstr_fd(final_path, 0); // c'etait pour quoi ca??????????
	free(tmp_path);
	return (final_path);
}

char	*pathfinder(t_shell *shell)
{
	char	**poss_paths;
	char	*final_path;
	char	*path_var;
	int		i;

	path_var = ft_getenv("PATH", shell);
	if (!path_var)
		return (NULL);
	poss_paths = ft_split(path_var, ':');
	i = 0;
	while (poss_paths[i])
	{
		final_path = construct_final_path(poss_paths[i], shell->exec->arr[0]);
		if (final_path != NULL && access (final_path, F_OK) == 0)
		{
			free_possible_paths(poss_paths);
			return (final_path);
		}
		free(final_path);
		i++;
	}
	free_possible_paths(poss_paths);
	return (NULL);
}