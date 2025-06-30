/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sradosav <sradosav@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:32:53 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/30 23:45:42 by sradosav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	amb_redir(char *str, t_shell *shell)
{
	char	*msg;

	msg = ft_strjoin(AMB_REDIR, str);
	if (!msg)
		ft_clean_exit(msg, shell, NULL, NULL);
	ft_putstr_fd(msg, 2);
	free(msg);
}

static void	init_vars(int *i, t_token **last, t_token **first)
{
	*i = 0;
	*last = NULL;
	*first = NULL;
}

t_token	*insert_new_nodes(t_shell *shell, t_token *prev, t_token *current,
	char **splitted)
{
	int		i;
	t_token	*new_node;
	t_token	*last;
	t_token	*first;

	init_vars(&i, &last, &first);
	while (splitted[i])
	{
		new_node = new_token_append(first, splitted[i], current->type, shell);
		if (!first)
			first = new_node;
		else
			last->next = new_node;
		last = new_node;
		i++;
	}
	if (first)
		first->next = current->next;
	if (prev)
		prev->next = first;
	else
		shell->token = first;
	return (free(current->value), free(current), last);
}

int	process_token(t_shell *shell, t_token **tmp, t_token **prev,
			char **expanded)
{
	char	**splitted;

	if (!ft_strchr((*tmp)->value, '$'))
		return (0);
	*expanded = join_chars(split_and_expand((*tmp)->value, shell), shell);
	if (*expanded && (*expanded)[0] != 0)
	{
		splitted = split_keep_separators(*expanded, is_whitespace, shell);
		if (((*tmp)->type == FILEN && count_strings(splitted) > 1)
			|| ((*tmp)->type == FILEN && splitted[0] == 0))
		{
			amb_redir(*expanded, shell);
			(*tmp)->amb_redir = true;
			return (free(*expanded), ft_free_str_array(splitted), 1);
		}
		*tmp = insert_new_nodes(shell, *prev, *tmp, splitted);
		if (ft_strchr((*tmp)->value, '$'))
		{
			*prev = *tmp;
			*tmp = (*tmp)->next;
		}
	}
	else
		(*tmp)->value[0] = 0;
	return (cleanup_token(expanded, &splitted), 1);
}

void	expand(t_shell *shell)
{
	t_token	*tmp;
	t_token	*prev;
	char	*expanded;
	int		skip;

	tmp = shell->token;
	prev = NULL;
	expanded = NULL;
	skip = 0;
	while (tmp)
	{
		skip = process_token(shell, &tmp, &prev, &expanded);
		if (skip)
			continue ;
		prev = tmp;
		tmp = tmp->next;
	}
}
