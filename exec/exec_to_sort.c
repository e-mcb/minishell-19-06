/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_to_sort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 22:39:57 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/30 22:50:24 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_exec	*init_exec(void)
{
	t_exec	*exec;

	exec = malloc(sizeof(t_exec));
	if (!exec)
		return (NULL);
	exec->arr = NULL;
	exec->fd_in = 0;
	exec->fd_out = 1;
	exec->next = NULL;
	exec->heredoc = NULL;
	exec->heredoc_bool = false;
	exec->pid = -1;
	return (exec);
}

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

	while (token != NULL && token->type != PIPE)
	{
		if (token->type == HDOC)
		{
			while (1)
			{
				write(1, ">", 1);
				line = get_next_line(0);
				if ((token->value[0] == 0 && line[0] == '\n') || !line
					|| !ft_strcmp(ft_strtrim(line, "\n"), token->value))
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

char	**add_string_to_array(char **array, char *str, t_shell *shell)
{
	int		i;
	int		j;
	char	**new_array;

	i = 0;
	j = 0;
	while (array && array[i])
		i++;
	new_array = malloc((i + 2) * sizeof(char *));
	if (!new_array)
		ft_clean_exit(NULL, shell, NULL, new_array);
	while (j < i)
	{
		new_array[j] = ft_strdup(array[j]);
		free(array[j]);
		j++;
	}
	new_array[i] = ft_strdup(str);
	if (!new_array[i])
		ft_clean_exit(NULL, shell, NULL, new_array);
	new_array[i + 1] = NULL;
	free(array);
	return (new_array);
}

void	create_exec(t_shell *shell)
{
	t_exec	*exec;
	t_token	*tmp;
	t_exec	*last;
	char	*heredoc;

	tmp = shell->token;
	exec = new_node(NULL);
	last = exec;
	heredoc = NULL;
	while (tmp)
	{
		if (tmp->type == PIPE)
		{
			last->next = new_node(exec);
			last = last->next;
			last->heredoc = heredoc;
		}
		if (is_redir(tmp) || tmp->type == HDOC)
			{
				tmp = handle_redir(last, tmp);
				if (!tmp)
					break ;
			}
		if (tmp->type == ARG || tmp->type == CMD)
			last->arr = add_string_to_array(last->arr, tmp->value, shell);
		tmp = tmp->next;
	}
	shell->exec = exec;
}
