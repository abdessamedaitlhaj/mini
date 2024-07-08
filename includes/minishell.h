/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:13:05 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/07 10:00:52 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef enum e_aa
{
	OUTFILE,
	INFILE,
	HEREDOC,
	APPEND
}		t_aa;

typedef struct s_redir
{
	char	*file;
	int		type;
	int		fd;
	int		expand_error;
	int		expanding_heredoc;
}	t_redir;

typedef struct s_parsing_status
{
	int			error;
	int			index;
	int			heredoc;
	char		**delimiters;
}		t_parsing_status;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	t_redir			**files;
	int				outfile;
	int				infile;
	int				heredoc;
	int				append;
	int				args_number;
	int				flag_infiles;
	int				flag_outfiles;
}	t_cmd;

typedef struct s_data
{
	char	*line;
	char	**pipes;
	int		counter_command;
	t_cmd	*cmds;
	t_list	*allocated;
	char	**heredoc;
	int		heredoc_error;
	t_list	*env;
	int		delimiter_count;
	char	**envp;
	int		exit_status;
	
}	t_data;

typedef struct s_indexes
{
	int	i;
	int	j;
	int	k;
	int	l;
}	t_indexes;

t_parsing_status	pair_quotes(char *line);
t_parsing_status	parsing_pipe(char *str);
t_parsing_status	parsing_redir(char *line);
char				*ft_strchr(const char *s, int c);
char				**ft_split_str(char const *s, char const *delimiters,
						t_data *data);
void				skip_quotes(int *i, char *str);
void				quote_replace(char *str, char find, char replace);
int					ft_isspace(char c);
int					parsing(char *line, t_data *data);
int					ft_strcmp(const char *s1, const char *s2);
int					node_compare(t_list *lst, char *str);
void				replace_node_content(t_list *lst, char *new_str);
int					count_content_lst(t_list *lst, char *str);
void				initialize_cmd(t_cmd *cmd);
void				repalce_char_in_linked_list(t_list *lst,
						char c, char new_c);
void				free_array(char **array);
void				allocate_memory_for_files(t_cmd *cmds, t_data *data,
						char **split, int i);

int					ft_strlen(char *str);
char				*ft_strdup(char *str, t_list **allocated);
int					count_str_in_2d_array(char *find, char **array);
t_list				*init_malloc(void);
void				*ft_malloc(size_t size, t_list **garbage_collector);
char				*ft_substr(char *s, unsigned int start,
						size_t len, t_data *data);
char				*ft_strjoin(char *s1, char *s2, t_data *data);
void				quote_replace(char *str, char find, char replace);
char				*add_space_redir(char *line, t_data *data);
void				fill_command(t_data *data);
void				open_heredoc(t_data *data);
int					find_last_str(char *str1, char *str2, char **array);
void				initialize_cmd_and_set_flags(t_cmd *cmds,
						t_indexes *indexes, char **split, t_data *data);
void				handle_redirections(t_cmd *cmds, t_indexes *indexes,
						char **split, t_data *data);
void				handle_cmd_allocation(t_cmd *cmds, int *j,
						char **split, t_data *data);
void				handle_cmd_args(t_cmd *cmds, int *j, int *k, char **split);
void				count_cmd_args(int *j, int *k, char **split);
int					ft_min(int a, int b, int c);
char				*ft_strndup( char *s, size_t n);
char* 				expanding_outside (char *pipe, t_data *data);
void				ambigious (t_cmd *cmd, t_data *data);
char				*expanding_inside (char *pipe, t_data *data);
char				*remove_q(char* str, t_data *data);
void				empty_line(char *limiter);
void				push_line_expand(int fd, char *limiter, t_data *data);
void				push_line(int fd, char *limiter);
char				*expanding_final(char *pipe, t_data *data);
void				*ft_lstnew(void *content);
void				ft_lstadd_back(t_list **list, t_list *new);

//=============================================================================ME==================================================================================================

void				ft_lstadd_back(t_list **list, t_list *new);
void				*ft_lstnew(void *content);
char				*ft_pwd(t_data *data);
int					ft_echo(char **args, int n, int status);
int					ft_env(t_list *env);
int					ft_cd(char *path, t_data *data);

int					envp_size(char **envp);
void				init_envs(char **envp, t_data *data);

void				free_allocated(t_list **garbage_collector);
void				ft_putstr_fd(char *s, int fd);
int					ft_isalpha(char c);
void				ft_putendl_fd(char *s, int fd);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_strcmp(const char *s1, const char *s2);
char				*ft_strjoin2(char *s1, char *s2, char *s3);
char				**ft_split(char const *s, char c);

char				*ft_getenv(char *key, t_list *env);
int					ft_get_key_index(char *key, t_list *env);
int					ft_export(char **args, t_data *data);
int					ft_setenv(char *key, char *value, t_data *data);
int					ft_unsetenv(char *key, t_data *data);
int					env_size(t_list *env);
char				*ft_strchr(const char *s, int c);

char				*find_cmd(t_data *data, char *cmd);
int					execute_cmds(t_data *data);
void				ft_exit(char *number);
void				exec_cmd(t_data *data, char *cmd, char **args);

void				set_last_cmd(char *outfile, int *fd, int her);
int					ft_close(int fd);
void				set_first_cmd(char *infile, int *fd);
int					dup_cmd_in(int *fd, int prev_fd);
int					dup_cmd_out(int *fd);
char				**allocate_cmd_args(t_data *data, t_cmd *cmd);
void				ft_memset(void *b, int c, size_t len);
int					execute_cmds(t_data *data);
void				free_allocated(t_list **garbage_collector);
int					execute_one_node(t_data *data);
int					ft_strcmp(const char *s1, const char *s2);

#endif