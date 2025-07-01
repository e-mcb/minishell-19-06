/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 22:56:11 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/29 22:27:34 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_execsize(t_exec *exec)
{
	int		i;

	i = 0;
	while (exec)
	{
		exec = exec->next;
		i++;
	}
	return (i);
}

void	close_parent_fds(t_exec *tmp, int *pipe_fd, int *prev_fd_in)
{
	if (tmp->fd_in != STDIN_FILENO && tmp->fd_in != *prev_fd_in)
	{
		close(tmp->fd_in);
		tmp->fd_in = STDIN_FILENO;
	}
	if (tmp->fd_out != STDOUT_FILENO)
	{
		close(tmp->fd_out);
		tmp->fd_out = STDOUT_FILENO;
	}
	if (*prev_fd_in != STDIN_FILENO && *prev_fd_in != tmp->fd_in)
	{
		close(*prev_fd_in);
		*prev_fd_in = STDIN_FILENO;
	}
	if (tmp->next != NULL)
	{
		close(pipe_fd[1]);
		if (tmp->next->fd_in == STDIN_FILENO)
			tmp->next->fd_in = pipe_fd[0];
		else
			close(pipe_fd[0]);
	}
	*prev_fd_in = tmp->fd_in;
}

void	wait_for_children_to_exit(t_shell *shell)
{
	int	status;
	//comment check pour le bon pid?
	while (wait(&status) > 0)
	{
		if (WIFSIGNALED(status))
		{
			int sig = WTERMSIG(status);
			if (sig == SIGQUIT)
			{
				write(1, "Quit (core dumped)\n", 19);
				if (shell->token)
					free_list(&(shell->token));
				if (shell->exec)
					free_exec_list(&(shell->exec));
				shell->exit_status = 130;
			}
			if (sig == SIGINT)
			{
				write(1, "\n", 1);
				if (shell->token)
					free_list(&(shell->token));
				if (shell->exec)
					free_exec_list(&(shell->exec));
				shell->exit_status = 130;
			}
		}
	};
}

void	exec_loop(t_shell *shell)
{
	t_exec	*tmp;
	pid_t	pid;
	int		pipe_fd[2];
	int		prev_fd_in;

	prev_fd_in = STDIN_FILENO;
	tmp = shell->exec;
	if (tmp->arr && ft_execsize(tmp) == 1 && ft_is_builtin(tmp->arr[0]))
	{
		handle_builtin(shell, tmp);
		return ;
	}
	while (tmp)
	{
		if (!is_valid_command(tmp))
		{
			tmp = tmp->next;
			continue ;
		}
		if (tmp->next != NULL)
			safe_pipe(pipe_fd, shell);
		g_status = 2;
		pid = safe_fork(shell);
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
    		signal(SIGQUIT, SIG_DFL);
			handle_child_process(shell, tmp, pipe_fd);
		}
		else
			close_parent_fds(tmp, pipe_fd, &prev_fd_in);
		tmp = tmp->next;
	}
	if (prev_fd_in != STDIN_FILENO)
		close(prev_fd_in);
	wait_for_children_to_exit(shell);
	g_status = 0;
}
