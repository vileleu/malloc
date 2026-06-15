/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 12:18:05 by vileleu           #+#    #+#             */
/*   Updated: 2019/10/14 12:49:17 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t				i;
	const unsigned char	*source;
	unsigned char		*desti;

	i = 0;
	source = (const unsigned char *)src;
	desti = (unsigned char *)dst;
	while (i < n && dst != src)
	{
		desti[i] = source[i];
		i++;
	}
	return (dst);
}
