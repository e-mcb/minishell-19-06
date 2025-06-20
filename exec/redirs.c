/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 22:47:29 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/20 22:48:42 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_in(t_exec *exec, t_token *tmp)
{
	if (exec->fd_in > 0)
		close(exec->fd_in);
	exec->fd_in = open(tmp->next->value, O_RDONLY);
	if (exec->fd_in < 0)
	{
		perror("Error (infile)");
		return (1);
	}
	exec->heredoc_bool = false;
	return (0);
}

static int	handle_out(t_exec *exec, t_token *tmp)
{
	if (exec->fd_out > 1)
		close(exec->fd_out);
	exec->fd_out = open(tmp->next->value, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (exec->fd_out < 0)
	{
		perror("Error (outfile)");
		return (1);
	}
	return (0);
}

static int	handle_append(t_exec *exec, t_token *tmp)
{
	if (exec->fd_out > 1)
		close(exec->fd_out);
	exec->fd_out = open(tmp->next->value, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (exec->fd_out < 0)
	{
		perror("Error (append)");
		return (1);
	}
	return (0);
}

static int	handle_heredoc(t_exec *exec, t_token *tmp)
{
	if (exec->fd_in > 0)
		close(exec->fd_in);
	exec->fd_in = 0;
	exec->heredoc = heredoc(tmp->next);
	exec->heredoc_bool = true;
	exec->fd_in = 0;
	return (0);
}

t_token	*handle_redir(t_exec *exec, t_token *tmp)
{
	int	skip;

	skip = 0;
	if (tmp->type == IN)
		skip = handle_in(exec, tmp);
	else if (tmp->type == OUT)
		skip = handle_out(exec, tmp);
	else if (tmp->type == APPEND)
		skip = handle_append(exec, tmp);
	else if (tmp->type == HDOC)
		skip = handle_heredoc(exec, tmp);
	if (skip)
	{
		if (exec->fd_in > 0)
			close(exec->fd_in);
		if (exec->fd_out > 1)
			close(exec->fd_out);
		tmp = skip_to_pipe(tmp);
	}
	return (tmp);
}
