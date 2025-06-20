/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_to_sort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 22:39:57 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/20 22:55:23 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_exec	*new_node(t_exec *head)
{
	t_exec	*new;
	t_exec	*last;

	new = init_exec();
	if (!new)
		return (NULL);
	if (!head)
		return (new);
	last = head;
	while (last->next)
		last = last->next;
	last->next = new;
	return (new);
}

t_token	*skip_to_pipe(t_token *token)
{
	char	*line;

	while (token->type != PIPE || token != NULL)
	{
		if (token->type == HDOC)
		{
			while (1)
			{
				write(1, ">", 1);
				line = get_next_line(0);
				if (ft_strncmp(line, token->next->value,
						ft_strlen(token->value) == 0)
					|| (token->next->value == "" && line[0] == '\n'))
				{
					free(line);
					token = token->next;
					break ;
				}
				free(line);
			}
		}
		token = token->next;
	}
	return (token);
}

void	loop_token(t_shell *shell)
{
	t_exec	*exec;
	t_token	*tmp;
	t_exec	*last;
	char	*heredoc;

	tmp = shell->token;
	exec = new_node(NULL);
	last = exec;
	while (tmp)
	{
		if (tmp->type == PIPE)
		{
			last->next == new_node(exec);
			last = last->next;
			last->heredoc == heredoc;
		}
		if (is_redir(tmp->type))
			tmp = handle_redir(last, tmp);
		if (tmp->type == ARG || tmp->type == CMD)
			exec->arr = add_string_to_array(exec->arr, tmp->value);
		tmp = tmp->next;
	}
	shell->exec = exec;
}
