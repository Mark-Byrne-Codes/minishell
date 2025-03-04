/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:16:46 by mbyrne            #+#    #+#             */
/*   Updated: 2024/11/11 11:41:08 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* This function checks if the character is a printable character. */
int	ft_isprint(int c)
{
	if (32 <= c && c <= 126)
		return (1);
	return (0);
}
