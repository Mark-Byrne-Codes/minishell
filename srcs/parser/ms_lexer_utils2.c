/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lexer_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elehtone <elehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:53:13 by elehtone          #+#    #+#             */
/*   Updated: 2025/03/20 18:57:03 by elehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * Checks if a character is either a single or double quote.
 * Param: char
 * Return: 0 if not, 1 if single, 2 if double
 */
int	fun_check_any_quote(unsigned int c)
{
	if (c == 39)
		return (1);
	else if (c == 34)
		return (2);
	return (0);
}
