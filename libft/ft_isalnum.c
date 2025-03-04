/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbyrne <mbyrne@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:33:19 by mbyrne            #+#    #+#             */
/*   Updated: 2024/11/11 11:35:40 by mbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*This function checks if the character 
is an alphanumeric character. */
int	ft_isalnum(int c)
{
	if (('0' <= c && c <= '9') || ('a' <= c && c <= 'z')
		|| ('A' <= c && c <= 'Z'))
		return (1);
	return (0);
}
