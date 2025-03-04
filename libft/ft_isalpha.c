/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:03:20 by mbyrne            #+#    #+#             */
/*   Updated: 2024/11/11 11:37:09 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* This function checks if the character 
is an alphabetic character. */
int	ft_isalpha(int c)
{
	if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
		return (1);
	return (0);
}
