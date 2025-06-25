/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 22:56:11 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/25 23:46:12 by mzutter          ###   ########.fr       */
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

void	call_builtin(t_shell *shell, t_exec *cur_exec, char *cmd)
{
	//pipe???
	//dup2 placeholder
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

// void	call_execve(t_shell *shell, t_exec *cur_exec, int *pipe_fd)
// {
// 	//pipe
// 	//fork
// 	// {
// 	// 	dup2
// 	// 	find path
// 	// 	execve
// 	// }
	
// }

// void	exec(t_shell *shell)
// {
// 	t_exec	*tmp;
// 	int		pipe_fd[2];

// 	tmp = shell->exec;
// 	if (ft_execsize == 1)
// 		update_or_add("_",
// 			shell->exec->arr[count_strings(shell->exec->arr)], shell, 0);
// 	while (tmp->next)
// 	{
// 		//pipe
// 		if (is_builtin(tmp->arr[0]), pipe_fd)
// 			call_builtin(shell, tmp, tmp->arr[0]);
// 		else
// 		{
// 			call_execve(shell, tmp, pipe_fd);
// 		}
		
// 		tmp = tmp->next;
// 	}
// 	//dup2 du dernier
// 	//wait
// 	//waitpid
// 	//gestion des closes
// }


//builtins s'appellent quand meme dans un fork
//si builtin seul, on l'appel tel quel
//modif requises pour que les builtins prennent le fd de sortie
//autrement on appelle la fonction d'appel de builtins
//comme on appelerait execve avec les pipe & les dup2 etc

// void ft_jsp(t_shell *shell)
// {
// 	int	exec_size;

// 	exec_size = ft_execsize(shell->exec);
// 	if (exec_size == 1 && ft_is_builtin)
// 	{
// 		//pas de dup2, il faudra modifier les builtins 
// 		//de maniere a ce qu'ils prennent le fd de sortie en parametre
// 	}
// }



// void	exec_loop(t_shell *shell)
// {
// 	t_exec	*tmp;
// 	char	*path;
// 	pid_t	pid;
// 	// int		pipe_fd[2];

// 	tmp = shell->exec;
// 	printf("%d\n", STDIN_FILENO);
// 	printf("%d\n", STDOUT_FILENO);
// 	printf("%d\n", tmp->fd_in);
// 	printf("%d\n", tmp->fd_out);
// 	if (ft_execsize(tmp) == 1)
// 	{
// 		// update_or_add("_",
// 			// shell->exec->arr[count_strings(shell->exec->arr)], shell, 0);
// 		if (ft_is_builtin(tmp->arr[0]))
// 			call_builtin(shell, tmp, tmp->arr[0]);
// 		else
// 		{
// 			pid = fork();
// 			if (pid == 0)
// 			{
// 				dup2(tmp->fd_in, STDIN_FILENO);
// 				dup2(tmp->fd_out, STDOUT_FILENO);
// 				printf("%dIN\n", STDIN_FILENO);
// 				printf("%dOUT\n", STDOUT_FILENO);
// 				path = pathfinder(shell);
// 				execve(path, tmp->arr, shell->env_arr);
// 			}
// 				waitpid(pid, &(shell->exit_status), 0);
// 		}
// 	}
// }

void	exec_loop(t_shell *shell)
{
	t_exec	*tmp;
	char	*path;
	pid_t	pid;
	int		pipe_fd[2];
	int		status;
	// int		prev_fd_in = 0;

	tmp = shell->exec;
	if (ft_execsize(tmp) == 1 && ft_is_builtin(shell->exec->arr[0]))
			call_builtin(shell, tmp, tmp->arr[0]);
	else
	{
		while (tmp)
		{
			if(pipe(pipe_fd) < 0)
				ft_clean_exit(NULL, shell, NULL, NULL);
			if (tmp->next != NULL && tmp->next->fd_in == 0 && tmp->next->heredoc_bool == false) // heredoc a check aussi | on remplace stdin par read_pipe
					tmp->next->fd_in = pipe_fd[0];
			pid = fork();
			if (pid < 0)
				ft_clean_exit(NULL, shell, NULL, NULL);
			if (pid == 0)
			{
				if (tmp->heredoc_bool == true)
					ft_putstr_fd(tmp->heredoc, STDIN_FILENO);
			if (tmp->fd_in != STDIN_FILENO)
			{
				dup2(tmp->fd_in, STDIN_FILENO);
				close(tmp->fd_in);
			}

			// Handle pipe output
			if (tmp->next && tmp->fd_out == STDOUT_FILENO)
			{
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
			}

			// Handle output redirection (overwrite pipe redirection if it exists)
			if (tmp->fd_out != STDOUT_FILENO)
			{
				dup2(tmp->fd_out, STDOUT_FILENO);
				close(tmp->fd_out);
			}		
				if (ft_is_builtin(tmp->arr[0]))
					call_builtin(shell, tmp, tmp->arr[0]);
				else
				{
					path = pathfinder(shell, tmp);
					execve(path, tmp->arr, shell->env_arr);
				}
				exit (1);
			}
			close(pipe_fd[1]);
			close(pipe_fd[0]);
			tmp = tmp->next;
		}
		while(wait(&status) > 0);
		//waitpid
	}
}




// while(tmp)
// {
// 	if (tmp->heredoc_bool == true)
// 		ft_putstr_fd(tmp->heredoc, 0);
// 	if (tmp->in_fd != 0) // que pour le 1er parce que on modifie in_fd de next s'il m'a pas changer en fin de loop
// 		dup2(tmp->in_fd, STDIN_FILENO);
// 	if (tmp->next && tmp->out_fd == 1)// on remplace la sortie par l'ecriture du pipe
// 		dup2(tmp->out_fd, pipe_fd[1]);
// 	if (tmp->out_fd != 1) //on remplace la sortie par le fichier qu'on a ouvert
// 		dup2(tmp->out_fd, STDOUT_FILENO);
// 	// else if (tmp->next == NULL && tmp->out_fd == 1) // si on est en fin de chaine, etre sur d'ecrire dans stdout
// 	// 	// dup2(STDOUT_FILENO, STDOUT_FILENO); || probblement pas utile
// 	if (tmp->next->in_fd == 0  && tmp->next != NULL && tmp->next->heredoc_bool == false) // heredoc a check aussi | on remplace stdin par read_pipe
// 		tmp->next->in_fd = pipe_fd[0];
// 	tmp = tmp->next;
// }