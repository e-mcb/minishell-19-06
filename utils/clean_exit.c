/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 21:18:02 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/18 21:17:53 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_list(t_token **head)
{
	t_token	*current;
	t_token	*next;

	current = *head;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	*head = NULL;
}

void	ft_free_str_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	ft_clean_exit(char *input, t_shell *shell,
	char *str_to_free, char **arr_to_free)
{
	if (str_to_free)
		free(str_to_free);
	if (arr_to_free)
		ft_free_str_array(arr_to_free);
	if (input)
		free(input);
	if (shell->env)
		free_env_list(&(shell->env));
	if (shell->env_arr)
		ft_free_str_array(shell->env_arr);
	if (shell->splitted)
		ft_free_str_array(shell->splitted);
	if (shell->token)
		free_list(&(shell->token));
	if (shell)
		free(shell);
		
	exit(0);
}
