/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 12:45:07 by vileleu           #+#    #+#             */
/*   Updated: 2019/10/14 12:48:56 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	const char	*s;
	char		*d;

	s = src;
	d = dst;
	while (n--)
	{
		*d = *s;
		if (*s == (char)c)
		{
			d++;
			return (d);
		}
		d++;
		s++;
	}
	return (NULL);
}
