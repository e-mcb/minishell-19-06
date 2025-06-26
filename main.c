/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 11:16:07 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/26 20:17:14 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "includes/minishell.h"

// static char	*prompt(t_shell *shell)
// {
// 	char	*input;

// 	input = readline("minishell>");
// 	if (input == NULL)
// 	{
// 		ft_putstr_fd("Goodbye\n", 2);
// 		ft_clean_exit(NULL, shell, NULL, NULL);
// 	}
// 	if (input[0] != '\0')
// 		add_history(input);
// 	if (ft_has_invalid_quotes(input))
// 	{
// 		ft_putstr_fd(OPEN_QUOTES, 2);
// 		free(input);
// 		return (NULL);
// 	}
// 	return (input);
// }

// static t_shell	*init_shell(t_shell *shell, char **envp)
// {
// 	shell = malloc(sizeof(t_shell));
// 	if (shell == NULL)
// 		ft_clean_exit(NULL, NULL, NULL, NULL);
// 	shell->env_arr = NULL;
// 	shell->splitted = NULL;
// 	shell->token = NULL;
// 	shell->env = ft_env_to_list(envp);
// 	shell->exit_status = 0;
// 	if (shell->env == NULL)
// 		ft_clean_exit(NULL, shell, NULL, NULL);
// 	return (shell);
// }

// static void	ft_parsing(char *input, t_shell *shell)
// {
// 	int	i;

// 	i = 0;
// 	whitespace_to_space(input);
// 	shell->splitted = ft_split2(input, ' ');
// 	if (shell->splitted == NULL)
// 		ft_clean_exit(input, shell, NULL, NULL);
// 	free (input);
// 	while (shell->splitted[i])
// 	{
// 		tokenizer(shell, i);
// 		i++;
// 	}
// 	ft_free_str_array(shell->splitted);
// 	refine_token_type(shell->token);
// 	expand(shell);
// 	second_refine_token_type(shell->token);
// 	shell->splitted = NULL;
// }

// static void	minishell_loop(t_shell *shell)
// {
// 	char	*input;

// 	while (1)
// 	{
// 		input = prompt(shell);
// 		if (input == NULL)
// 			continue ;
// 		ft_parsing(input, shell);
// 		// [EXEC PLACEHOLDER]
// 		free_list(&shell->token);
// 	}
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	t_shell	*shell;

// 	(void)argc;
// 	(void)argv;
// 	shell = NULL;
// 	shell = init_shell(shell, envp);
// 	minishell_loop(shell);
// }

#include "includes/minishell.h"

static char	*prompt(t_shell *shell)
{
	char	*input;

	input = readline("minishell>");
	if (input == NULL)
	{
		ft_putstr_fd("Goodbye\n", 2);  //a voir avec les signaux
		ft_clean_exit(NULL, shell, NULL, NULL);
	}
	if (input[0] != '\0')
		add_history(input);
	if (ft_has_invalid_quotes(input))
	{
		ft_putstr_fd(OPEN_QUOTES, 2);
		free(input);
		return (NULL);
	}
	return (input);
}

static t_shell	*init_shell(t_shell *shell, char **envp)
{
	shell = malloc(sizeof(t_shell));
	if (shell == NULL)
		ft_clean_exit(NULL, NULL, NULL, NULL);
	shell->env_arr = NULL;
	shell->splitted = NULL;
	shell->token = NULL;
	shell->env = ft_env_to_list(envp, shell);
	shell->exit_status = 0;
	if (shell->env == NULL)
		ft_clean_exit(NULL, shell, NULL, NULL);
	shell->exec = NULL;
	return (shell);
}

static void	ft_parsing(char *input, t_shell *shell)
{
	int		i;

	i = 0;
	whitespace_to_space(input);
	shell->splitted = ft_split2(input, ' ');
	if (shell->splitted == NULL)
		ft_clean_exit(input, shell, NULL, NULL);
	free (input);
	while (shell->splitted[i])
	{
		tokenizer(shell, i);
		i++;
	}
	if (shell->splitted != NULL)
		ft_free_str_array(shell->splitted);
	refine_token_type(shell->token);
	expand(shell);
	second_refine_token_type(shell->token);
	shell->splitted = NULL;
}

void print_str_array(char **arr)
{
    int i = 0;

    if (!arr)
        return;

    while (arr[i])
    {
        printf("[%d]: %s\n", i, arr[i]);
        i++;
    }
}

// static void	minishell_loop(t_shell *shell)
// {
// 	char	*input;
// 	t_exec	*tmp;


// 	while (1)
// 	{
// 		input = prompt(shell);
// 		if (input == NULL)
// 			continue ;
// 		ft_parsing(input, shell);
// 		create_exec(shell);
// 		env_list_to_arr(shell);
// 		tmp = shell->exec;
// 		while(tmp)
// 		{
// 			print_str_array(tmp->arr);
// 			tmp = tmp->next;
// 		}
// 		// exec_loop(shell);
// 		// [EXEC PLACEHOLDER]
// 		free_list(&shell->token);
// 	}
// }

static void	minishell_loop(t_shell *shell)
{
	char	*input;
	int		i;

	i = 3;
	while (1)
	{
		input = prompt(shell);
		if (input == NULL)
			continue ;
		if (input[0] == 0)
		{
			free(input);
			continue ;
		}
		ft_parsing(input, shell);
		create_exec(shell);
		env_list_to_arr(shell);
		exec_loop(shell);
		// [EXEC PLACEHOLDER]
		free_list(&shell->token);
		free_exec_list(&(shell->exec));
		ft_free_str_array(shell->env_arr);
		while (i++ < 1023)
		{
			close(i);
		}
		shell->env_arr = NULL;
	}
}

int	main(int argc, char **argv, char **envp)
{
	printf("Welcome to minishell\n");
	t_shell	*shell;

	(void)argc;
	(void)argv;
	shell = NULL;
	shell = init_shell(shell, envp);
	minishell_loop(shell);
	
}

// int main()
// {
// 	char **splitted;
// 	char init[] = "echo idiot";
// 	char *cpy;

// 	cpy = strdup(init);
// 	whitespace_to_space(cpy);
// 	splitted = ft_split2(cpy, ' ');
// 	printf("%s\n", splitted[0]);
// 	printf("%s\n", splitted[1]);
// 	ft_free_str_array(splitted);
// 	free(cpy);
// }