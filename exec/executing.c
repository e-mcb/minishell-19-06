/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 22:56:11 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/21 01:17:19 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_execsize(t_exec *exec)
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

static int	ft_is_builtin(char *str)
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

void	call_builtin(t_shell *shell, t_exec *cur_exec, char *cmd, int *pipe_fd)
{
	//pipe???
	//dup2 placeholder
	if (ft_strcmp(cmd, "echo") == 0)
		ft_echo(cur_exec->arr, shell);
	if (ft_strcmp(cmd, "cd") == 0)
		t_cd(cur_exec->arr, shell);
	if (ft_strcmp(cmd, "pwd") == 0)
		ft_pwd(cur_exec->arr, shell);
	if (ft_strcmp(cmd, "export") == 0)
		ft_export(cur_exec->arr, shell, ft_execsize(shell->exec));
	if (ft_strcmp(cmd, "unset") == 0)
		ft_unset(cur_exec->arr, shell, ft_execsize(shell->exec));
	if (ft_strcmp(cmd, "env") == 0)
		ft_env(cur_exec->arr, shell);
	if (ft_strcmp(cmd, "exit") == 0)
		ft_exit(cur_exec->arr, shell);
}

void	call_execve(t_shell *shell, t_exec *cur_exec, int *pipe_fd)
{
	//pipe
	//fork
	// {
	// 	dup2
	// 	find path
	// 	execve
	// }
	
}

void	exec(t_shell *shell)
{
	t_exec	*tmp;
	int		pipe_fd[2];

	tmp = shell->exec;
	if (ft_execsize == 1)
		update_or_add("_",
			shell->exec->arr[count_strings(shell->exec->arr)], shell, 0);
	while (tmp->next)
	{
		//pipe
		if (is_builtin(tmp->arr[0]), pipe_fd)
			call_builtin(shell, tmp, tmp->arr[0]);
		else
		{
			call_execve(shell, tmp, pipe_fd)
		}
		
		tmp = tmp->next;
	}
	//dup2 du dernier
	//wait
	//waitpid
	//gestion des closes
}
