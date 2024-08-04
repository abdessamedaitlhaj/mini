/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 04:30:21 by ael-hara          #+#    #+#             */
/*   Updated: 2024/08/04 01:58:28 by ael-hara         ###   ########.fr       */
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
# include <sys/stat.h>
# include <termios.h>
# include <dirent.h>
# include <signal.h>
# include <dirent.h>
# include <sys/wait.h>

extern int	g_signal_flag;

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
	int				flag_command;
}	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_data
{
	char	*line;
	char	**pipes;
	int		counter_command;
	t_cmd	*cmds;
	t_list	*allocated;
	char	**heredoc;
	int		heredoc_error;
	t_env	*env;
	int		empty_env;
	int		delimiter_count;
	int		fd_in;
	int		fd_out;
	char	**envp;
	int		exit_status;
	char	*help;
}	t_data;

typedef struct s_split_args
{
	char const	*s;
	char const	*delimiters;
}		t_split_args;

typedef struct s_indexes
{
	int	i;
	int	j;
	int	k;
	int	l;
	int	index;
}	t_indexes;

typedef struct s_key_value
{
	char	*key;
	char	*value;
	int		err;
	int		append;
}	t_key_value;

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
int					node_compare(t_list *lst, char *str);
void				replace_node_content(t_list *lst, char *new_str);
int					count_content_lst(t_list *lst, char *str);
void				initialize_cmd(t_cmd *cmd);
void				repalce_char_in_linked_list(t_list *lst,
						char c, char new_c);
void				allocate_memory_for_files(t_cmd *cmds, t_data *data,
						char **split, int i);
int					ft_strlen(char *str);
char				*ft_strdup(char *str, t_list **allocated);
int					count_str_in_array(char *find, char **array);
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
char				*ft_strndup( char *s, size_t n, t_data *data);
void				ambigious(t_cmd *cmd, t_data *data);
char				*expanding_inside(char *pipe, t_data *data);
char				*remove_q(char *str, t_data *data);
void				empty_line(char *limiter, t_data *data);
void				push_line_expand(int fd, char *limiter, t_data *data);
void				push_line(int fd, char *limiter, t_data *data);
char				*expanding_final(char *pipe, t_data *data);
char				*ft_itoa(int n, t_data *data);
void				*ft_lstnew(void *content);
void				ft_lstadd_back(t_list **list, t_list *new);
void				sig_her_child(int sig);
int					ft_isalnum(char c);
int					ft_isnum(char c);
int					skip_redirection(char *pipe, int i, int flag);
char				*expand_exit(char *pipe, int *i, t_data *data);
void				expand_num(char **pipe, int i, t_data *data);
void				expand_case(char **pipe, int *i, t_data *data);
void				expand_exist(char **pipe, int *i, int j, t_data *data);
void				expand_flag(char **pipe, int *i, int j, t_data *data);
void				expand_var(char **pipe, int *i, t_data *data);
int					expand_help(char **pipe, int *i, t_data *data);
void				inside_exit(char **pipe, int *i, t_data *data);
void				inside_number(char **pipe, int *i, t_data *data);
void				expand_var(char **pipe, int *i, t_data *data);
int					expand_help(char **pipe, int *i, t_data *data);
char				*expanding_outside(char *pipe, t_data *data);
void				inside_exit(char **pipe, int *i, t_data *data);
void				inside_number(char **pipe, int *i, t_data *data);
int					ft_numlen(int n);
int					empty_line_parse(char *line);
void				process_pipes(char **pipes, t_data *data);
void				process_line(char **line, t_data *data);
void				process_line_history(char *line);
int					process_errors(char *line, t_data *data);
void				herdoc_delimiters(char *line, int min, t_data *data);
void				process_heredoc_quotes(char **heredoc, t_data *data);
void				process_heredoc_line(char *line, int min, t_data *data);
void				process_heredoc_line(char *line, int min, t_data *data);
void				herdoc_delimiters(char *line, int min, t_data *data);
char				*ft_strdup_quotes(char *s1, t_data *data);
char				*remove_q(char *str, t_data *data);
void				remove_quotes(t_cmd *cmd, t_data *data);
int					check_null(char *str);
void				ambigious(t_cmd *cmd, t_data *data);
void				fill_command_help(t_cmd *cmds, t_data *data);
void				fill_command_index(t_cmd *cmds, t_indexes *indexes,
						char **split, t_data *data);
void				init_fill_command(t_cmd *cmds, t_indexes *indexes,
						char **split, t_data *data);
void				check_null_command(t_cmd *cmds, t_data *data);
void				heredoc_loop(t_cmd *cmds, t_data *data);
int					expanded(char *str);
int					ft_min(int a, int b, int c);
void				init_while(t_data *data, struct termios *term);
void				execute_and_free(t_data *data, char *line,
						struct termios *term);
void				line_exit(t_data *data, char *line);
void				check_signal(t_data *data);
int					parsing_signal(t_data *data, char *line);
t_data				init_main(char **envp);
void				int_handler(int sig);
void				quotes_to_ascii(char *str);
void				ascii_to_quotes(char *str);

//===================================================

void				*ft_lstnew(void *content);
void				init_envs(char **envp, t_data *data);
void				free_allocated(t_list **garbage_collector);
void				ft_putstr_fd(char *s, int fd);
int					ft_isalpha(char c);
void				ft_putendl_fd(char *s, int fd);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strjoin2(char *s1, char *s2, t_data *data);
char				*ft_getenv(char *key, t_env *env);
int					ft_get_key_index(char *key, t_env *env);
int					ft_unset(char **args, t_data *data, char *cmd);
int					ft_unsetenv(char *key, t_data *data);
char				*ft_strchr(const char *s, int c);
char				*find_cmd(t_data *data, char *cmd);
int					execute_cmds(t_data *data);
int					ft_exit(char **args, int arg_number, t_data *data);
void				exec_cmd(t_data *data, t_cmd *cmd);
void				set_last_cmd(char *outfile, int *fd, int her);
void				ft_close(int *fd, t_data *data);
void				set_first_cmd(char *infile, int *fd);
int					dup_cmd_in(int prev_fd, t_data *data);
int					dup_cmd_out(int *fd, t_data *data);
char				**allocate_cmd_args(t_data *data, t_cmd *cmd);
void				ft_memset(void *b, int c, size_t len);
int					execute_cmds(t_data *data);
int					execute_one_node(t_data *data);
int					ft_strcmp(const char *s1, const char *s2);
int					common_error(char *str1, char *str2, char *str3);
int					init_fd_out(t_data *data, t_cmd *cmd);
int					init_fd_in(t_data *data, t_cmd *cmd);
int					is_builtin(t_cmd *cmd);
int					other_builtins(t_data *data, t_cmd *cmd);
int					ft_exec_builtin(t_cmd *cmd, t_data *data);
int					dup_file(t_aa file, int fd, t_data *data);
int					dup_redir(t_data *data, int i, int *fd, int prev_fd);
int					open_files(char *file, int index);
char				**allocate_cmd_args(t_data *data, t_cmd *cmd);
int					init_fds(t_data *data, t_cmd *cmd);
void				perm_denied(char *cmd);
void				cmd_not_found(char *cmd, t_data *data);
void				sig_handler(int signo);
int					is_dir(char *path);
void				not_dir(t_data *data, char *cmd);
void				dir_error(t_data *data, char *cmd);
int					execute_one_node(t_data *data);
void				save_last_pipe(t_data *data, int i, int *fd, int *prev_fd);
int					execute_multiple_nodes(t_data *data);
int					create_process(t_data *data, t_cmd *cmd);
int					fork_process(t_data *data, int i, int *fd, int *prev_fd);
void				no_such_file(t_data *data, char *cmd);
char				*check_access(t_data *data, char *path, char *cmd);
void				print_env(t_env *env);
int					check_empty_args(t_data *data, char **args);
int					ft_export(char **args, t_data *data, char *cmd);
int					ft_env(t_env *env, int args_number);
int					ft_cd(char **args, t_data *data);
int					ft_echo(t_data *data, char **args, int n);
int					is_valid(t_key_value *k_v, char *key, t_data *data);
int					is_key_valid(char *arg, char *cmd);
int					ft_isdigit(int c);
int					not_valid_identifier(char *str, char *cmd);
int					close_streams(int *fd1, int *fd2, t_data *data);
void				close_pipe(int *fd, t_data *data);
void				fail_error(char *str, t_list **garbage_collector);
void				free_env(t_env **env);
void				get_status(t_data *data, int status);
char				*ft_strdup2(char *str, t_data *data);
void				exit_error(char *number, t_data *data);
int					ft_chdir(char *path);
int					set_home(t_data *data);
int					set_prev(t_data *data);
void				print_args(t_data *data, int i, int n, char **args);
int					ft_atoi(char *str, int *f);
int					is_num(char *str);
char				**get_paths(t_data *data, char *cmd);
void				ft_remove_env(t_env **env, char	*key);
int					ft_isalnm(int c);
char				*extract_key(t_key_value *k_v, char *arg, t_data *data);
void				replace_env(t_key_value *k_v, t_data *data);
void				init(int *fd, t_data *data);
char				*home_set(t_data *data);
char				*oldpwd_set(t_data *data);
int					special_path(char *path, t_data *data);
void				*ft_new_env(char	*key, char	*value, t_data *data);
void				ft_add_env(t_env **env, t_env *new);
void				ft_setenv(char *key, char *value, t_data *data);
char				*copy_cwd(char *cwd, t_data *data);
char				*ft_substr2(char *s, unsigned int start, size_t len,
						t_data *data);
void				not_dir(t_data *data, char *cmd);
void				ft_remove_env(t_env **env, char	*key);
char				*home_set(t_data *data);
char				*oldpwd_set(t_data *data);
int					ft_isdigit(int c);
void				ft_skip_spaces(int *sign, char **str);

#endif
