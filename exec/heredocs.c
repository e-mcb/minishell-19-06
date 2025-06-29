/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 22:45:52 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/29 22:20:26 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static char	*ft_strjoin_hdoc(char const *s1, char const *s2)
// {
// 	char	*str;
// 	size_t	i;
// 	size_t	j;

// 	if (!s1)
// 		s1 = ft_strdup("");
// 	if (!s2)
// 		s2 = ft_strdup("");
// 	i = 0;
// 	j = 0;
// 	str = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
// 	if (!str)
// 		return (NULL);
// 	while (s1[i] != 0)
// 	{
// 		str[i] = s1[i];
// 		i++;
// 	}
// 	while (s2[j] != 0)
// 	{
// 		str[i + j] = s2[j];
// 		j++;
// 	}
// 	str[i + j] = 0;
// 	return (str);
// }

// char *append_line_to_finale(char *finale, char *line)
// {
//     if (finale)
//         return ft_strjoin(finale, line);
//     return ft_strdup(line);
// }

// int should_exit_heredoc(t_token *token, char *line)
// {
//     return (token->value[0] == 0 && line[0] == '\n') || !line ||
//            ft_strncmp(line, token->value) == 0;
// }

// char *do_heredoc(t_token *token)
// {
//     char *finale = NULL;
//     char *line = NULL;

//     while (1)
//     {
//         write(STDOUT_FILENO, ">", 1);
//         line = get_next_line(STDIN_FILENO);
//         if (should_exit_heredoc(token, line))
//         {
//             free(line);
//             return finale;
//         }
//         finale = append_line_to_finale(finale, line);
//         free(line);
//     }
// }

char	*do_heredoc(t_token *token)
{
	char	*finale;
	char	*tmp;
	char	*line;

	finale = NULL;
	tmp = NULL;
	line = NULL;
	while (1)
	{
		write(STDOUT_FILENO, ">", 1);
		line = get_next_line(STDIN_FILENO);
		if ((token->value[0] == 0 && line[0] == '\n') || !line
			|| ft_strncmp(line, token->value, ft_strlen(token->value)) == 0) //error, finds eof in "eofsomethingelse"
			return (free(line), finale);
		if (finale)
			finale = ft_strjoin(finale, line);
		else
			finale = ft_strdup(line);
		if (tmp)
			free (tmp);	
		free (line);
		tmp = finale;
	}
}
