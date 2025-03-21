/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:46:19 by mbyrne            #+#    #+#             */
/*   Updated: 2025/03/21 13:38:08 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void print_env_vars(t_env *env)
{
    while (env)
    {
        ft_printf("declare -x %s", env->name);
        if (env->value)
            ft_printf("=\"%s\"", env->value);
        ft_printf("\n");
        env = env->next;
    }
}

static int is_valid_identifier(const char *name)
{
    if (!name || !*name)
        return (0);
    if (!ft_isalpha(*name) && *name != '_')
        return (0);
    name++;
    while (*name)
    {
        if (!ft_isalnum(*name) && *name != '_')
            return (0);
        name++;
    }
    return (1);
}

static char *extract_name(const char *arg)
{
    char *equal_sign;
    char *name;
    char *arg_copy;

    arg_copy = ft_strdup(arg);
    if (!arg_copy)
        return (NULL);
    equal_sign = ft_strchr(arg_copy, '=');
    if (equal_sign)
    {
        *equal_sign = '\0';
        name = ft_strdup(arg_copy);
    }
    else
        name = ft_strdup(arg_copy);
    free(arg_copy);
    return (name);
}

static char *extract_value(const char *arg)
{
    char *equal_sign;
    char *value;

    equal_sign = ft_strchr(arg, '=');
    if (equal_sign)
        value = ft_strdup(equal_sign + 1);
    else
        value = NULL;
    return (value);
}

static int set_env_var_internal(t_mini *mini, const char *arg)
{
    char *name;
    char *value;

    name = extract_name(arg);
    if (!name)
        return (1);
    if (!is_valid_identifier(name))
    {
        ft_putstr_fd("minishell: export: `", STDERR_FILENO);
        ft_putstr_fd(name, STDERR_FILENO);  // Changed from arg to name
        ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
        free(name);
        return (1);
    }
    value = extract_value(arg);
    update_env_variable(mini, name, value);
    free(name);
    free(value);
    return (0);
}

int export_builtin(t_mini *mini, char **args)
{
    int i;
    int ret;

    if (!args[1])
    {
        print_env_vars(mini->env);
        return (0);
    }
    i = 1;
    ret = 0;
    while (args[i])
    {
        if (set_env_var_internal(mini, args[i]) != 0)
            ret = 1;
        i++;
    }
    return (ret);
}
