/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 12:34:03 by vileleu           #+#    #+#             */
/*   Updated: 2019/10/23 13:52:38 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	unsigned int	j;
	size_t			len;
	size_t			len2;

	len = ft_strlen(src);
	len2 = ft_strlen(dst);
	if (dstsize == 0)
		return (len);
	if (dstsize < len2)
		return (len + dstsize);
	j = 0;
	while (src[j] != '\0' && (len2 + j) < dstsize - 1)
	{
		dst[len2 + j] = src[j];
		j++;
	}
	dst[len2 + j] = '\0';
	return (len2 + len);
}
