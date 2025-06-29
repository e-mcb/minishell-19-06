/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 21:54:14 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/29 22:01:56 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_is_builtin(char *str)
{
	if (!str)
		return (0);
	if (ft_strcmp(str, "echo") == 0
		|| ft_strcmp(str, "cd") == 0
		|| ft_strcmp(str, "pwd") == 0
		|| ft_strcmp(str, "export") == 0
		|| ft_strcmp(str, "unset") == 0
		|| ft_strcmp(str, "env") == 0
		|| ft_strcmp(str, "exit") == 0)
		return (1);
	return (0);
}

void	call_builtin(t_shell *shell, t_exec *cur_exec, char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		ft_echo(cur_exec->arr, shell, cur_exec->fd_out);
	if (ft_strcmp(cmd, "cd") == 0)
		ft_cd(cur_exec->arr, shell);
	if (ft_strcmp(cmd, "pwd") == 0)
		ft_pwd(cur_exec->arr, shell, cur_exec->fd_out);
	if (ft_strcmp(cmd, "export") == 0)
		ft_export(cur_exec->arr, shell,
			ft_execsize(shell->exec), cur_exec->fd_out);
	if (ft_strcmp(cmd, "unset") == 0)
		ft_unset(cur_exec->arr, shell, ft_execsize(shell->exec));
	if (ft_strcmp(cmd, "env") == 0)
		ft_env(cur_exec->arr, shell, cur_exec->fd_out);
	if (ft_strcmp(cmd, "exit") == 0)
		ft_exit(cur_exec->arr, shell);
}

void	handle_builtin(t_shell *shell, t_exec *tmp)
{
	if (ft_is_builtin(tmp->arr[0]))
	{
		call_builtin(shell, tmp, tmp->arr[0]);
	}
}

void	setup_redirection(t_exec *tmp, int *pipe_fd, t_shell *shell)
{
	if (tmp->fd_in != STDIN_FILENO)
	{
		if (dup2(tmp->fd_in, STDIN_FILENO) < 0)
			ft_clean_exit(NULL, shell, NULL, NULL);
		close(tmp->fd_in);
	}
	if (tmp->next != NULL && tmp->fd_out == STDOUT_FILENO)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
			ft_clean_exit(NULL, shell, NULL, NULL);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (tmp->fd_out != STDOUT_FILENO)
	{
		if (dup2(tmp->fd_out, STDOUT_FILENO) < 0)
			ft_clean_exit(NULL, shell, NULL, NULL);
		close(tmp->fd_out);
	}
}

void	handle_heredoc_input(t_exec *tmp, int *pipe_fd)
{
	if (tmp->heredoc_bool == true)
	{
		pipe(pipe_fd);
		ft_putstr_fd(tmp->heredoc, pipe_fd[1]);
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
	}
}
