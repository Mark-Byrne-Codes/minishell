/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:34:10 by mbyrne            #+#    #+#             */
/*   Updated: 2024/11/11 12:24:39 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* This function converts a uppercase letter to lowercase. */
int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}
