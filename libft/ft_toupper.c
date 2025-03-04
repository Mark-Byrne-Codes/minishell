/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:33:08 by mbyrne            #+#    #+#             */
/*   Updated: 2024/11/11 12:23:58 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* This function converts a lowercase letter to uppercase. */
int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}
