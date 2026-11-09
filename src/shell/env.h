#ifndef VOS_ENV_H
#define VOS_ENV_H

#define ENV_MAX 64

void env_init(void);
const char *env_get(const char *key);
int env_set(const char *key, const char *value);  /* 성공 0 */
char **env_get_envp(void);                        /* execve용 */
void env_print_all(void);

#endif
