/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:33:23 by mbyrne            #+#    #+#             */
/*   Updated: 2024/11/11 11:40:54 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* This function checks if the character is an ASCII character. */
int	ft_isascii(int c)
{
	if (0 <= c && c <= 127)
		return (1);
	return (0);
}
