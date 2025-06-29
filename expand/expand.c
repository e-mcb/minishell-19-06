/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:32:53 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/29 20:31:09 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*new_token_append(t_token *head, char *str, t_token_type type, t_shell *shell)
{
	t_token	*new_node;
	t_token	*last;

	new_node = create_token(str, shell);
	new_node->type = type;
	if (!head)
		return (new_node);
	last = head;
	while (last->next)
		last = last->next;
	last->next = new_node;
	return (head);
}

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

void	cleanup_token(char **expanded, char ***splitted)
{
	if (*splitted)
	{
		ft_free_str_array(*splitted);
		*splitted = NULL;
	}
	if (*expanded)
	{
		free(*expanded);
		*expanded = NULL;
	}
}


int	process_token(t_shell *shell, t_token **tmp, t_token **prev,
			char **expanded, char ***splitted)
{
	if (!ft_strchr((*tmp)->value, '$'))
		return 0;
	*expanded = join_chars(split_and_expand((*tmp)->value, shell), shell);
	if (*expanded && (*expanded)[0] != 0)
	{
		*splitted = split_keep_separators(*expanded, is_whitespace, shell);
		if (((*tmp)->type == FILEN && count_strings(*splitted) > 1)
			|| ((*tmp)->type == FILEN && splitted[0] == 0))
		{
			amb_redir(*expanded, shell);
			(*tmp)->amb_redir = true;
			return (free(*expanded), ft_free_str_array(*splitted), 1);
		}
		*tmp = insert_new_nodes(shell, *prev, *tmp, *splitted);
		if (ft_strchr((*tmp)->value, '$'))
		{
			*prev = *tmp;
			*tmp = (*tmp)->next;
		}
	}
	else
		(*tmp)->value[0] = 0;
	return (cleanup_token(expanded, splitted), 1);
}

// char *remove_invalid_dollars(const char *input)
// {
//     int i = 0, j = 0;
//     int in_single_quote = 0;
//     size_t len = strlen(input);
//     char *result = malloc(len + 1); // Allocate max possible length

//     if (!result)
//         return NULL; // Handle malloc failure as you like

//     while (input[i])
//     {
//         if (input[i] == '\'')
//             in_single_quote = !in_single_quote;  // Toggle single quote state

//         if (!in_single_quote && input[i] == '$')
//         {
//             // Check if next char is invalid (not letter or '_')
//             char next = input[i + 1];
//             if (next && !isalpha((unsigned char)next) && next != '_')
//             {
//                 // Skip both $ and next character
//                 i += 2;
//                 continue;
//             }
//         }

//         // Otherwise copy current character
//         result[j++] = input[i++];
//     }
//     result[j] = '\0';

//     return result;
// }

void	expand(t_shell *shell)
{
	t_token *tmp;
	t_token *prev;
	char *expanded;
	char **splitted;
	int skip;

	// CHECK A FAIRE ICI J'AI FAIT DE LA :ERDE

	tmp = shell->token;
	prev = NULL;
	expanded = NULL;
	splitted = NULL;
	skip = 0;

	while (tmp)
	{
		// printf("tmp;%s\n", tmp->value);
		// char	*idk;
		// idk = remove_invalid_dollars(tmp->value);
		// free(tmp->value);
		// tmp->value = idk;
		skip = process_token(shell, &tmp, &prev, &expanded, &splitted);
		if (skip)
			continue;
		prev = tmp;
		tmp = tmp->next;
	}
}
