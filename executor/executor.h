#include "../minishell.h"


// mine
void	ft_lstadd_back(t_list **list, t_list *new);
void	*ft_lstnew(void *content);
char	*ft_pwd(t_data *data);
int		ft_echo(char **args, int n, int status);
int		ft_env(t_list *env);
int		ft_cd(char *path, t_data *data);

int		envp_size(char **envp);
void	init_envs(char **envp, t_data *data);

// utils
void	free_allocated(t_list **garbage_collector);
void	ft_putstr_fd(char *s, int fd);
int		ft_isalpha(char c);
void	ft_putendl_fd(char *s, int fd);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strjoin2(char *s1, char *s2, char *s3);
char	**ft_split(char const *s, char c);

char	*ft_getenv(char *key, t_list *env);
int		ft_get_key_index(char *key, t_list *env);
int		ft_export(char **args, t_data *data);
int		ft_setenv(char *key, char *value, t_data *data);
int		ft_unsetenv(char *key, t_data *data);
int		env_size(t_list *env);
char	*ft_strchr(const char *s, int c);

char	*find_cmd(t_data *data, char *cmd);


// excution
int	execute_cmds(t_data *data);
void	ft_exit(char *number);
void	exec_cmd(t_data *data, char *cmd, char **args);

void	set_last_cmd(char *outfile, int *fd, int her);
int	ft_close(int fd);
void	set_first_cmd(char *infile, int *fd);
int	dup_cmd_in(int *fd, int prev_fd);
int	dup_cmd_out(int *fd);
char **allocate_cmd_args(t_data *data, t_cmd *cmd);
void	ft_memset(void *b, int c, size_t len);
int	execute_cmds(t_data *data);
void	free_allocated(t_list **garbage_collector);
int execute_one_node(t_data *data);
int ft_strcmp(const char *s1, const char *s2);