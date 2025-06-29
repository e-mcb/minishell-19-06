/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 21:51:01 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/29 22:02:45 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_command(t_shell *shell, t_exec *tmp)
{
	int		i;
	char	*path;

	i = 2;
	path = pathfinder(shell, tmp);
	if (path == NULL)
		ft_clean_exit(NULL, shell, NULL, NULL);
	while (++i < 1023)
		close(i);
	execve(path, tmp->arr, shell->env_arr);
	ft_clean_exit(NULL, shell, NULL, NULL);
}

pid_t	safe_fork(t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		ft_clean_exit(NULL, shell, NULL, NULL);
	return (pid);
}

int	safe_pipe(int *pipe_fd, t_shell *shell)
{
	if (pipe(pipe_fd) < 0)
		ft_clean_exit(NULL, shell, NULL, NULL);
	return (0);
}

int	is_valid_command(t_exec *tmp)
{
	return (tmp->arr != NULL && tmp->arr[0] != NULL);
}

void	handle_child_process(t_shell *shell, t_exec *tmp, int *pipe_fd)
{
	setup_redirection(tmp, pipe_fd, shell);
	handle_heredoc_input(tmp, pipe_fd);
	if (ft_is_builtin(tmp->arr[0]))
	{
		call_builtin(shell, tmp, tmp->arr[0]);
		ft_clean_exit(NULL, shell, NULL, NULL);
	}
	else
		execute_command(shell, tmp);
}
