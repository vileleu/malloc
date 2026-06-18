/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 15:28:30 by vileleu           #+#    #+#             */
/*   Updated: 2026/06/18 15:42:09 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	ft_putchar(char c) {
	write(1, &c, 1);
}

size_t	ft_strlen(const char *s) {
	size_t i;
	i = 0;
	if (!s)
		return (i);
	while (s[i] != '\0')
		i++;
	return (i);
}

void	ft_putstr_fd(char *s, int fd) {
	if (!s)
		return;
	write(fd, s, ft_strlen(s));
}

void	ft_putstr(char *s) {
	if (!s)
		return;
	write(1, s, ft_strlen(s));
}


void	ft_putsize(size_t n) {
	if (n <= 9) {
		n = n + 48;
		ft_putchar(n);
	}
	else {
		ft_putsize(n / 10);
		ft_putsize(n % 10);
	}
}

int		ft_strcmp(const char *s1, const char *s2) {
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char*)s1;
	str2 = (unsigned char*)s2;
	i = 0;
	while (str1[i] != '\0' && str2[i] != '\0') {
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	if (str1[i] == str2[i])
		return (0);
	return (str1[i] - str2[i]);
}

void	*ft_memcpy(void *dst, const void *src, size_t n) {
	size_t				i;
	const unsigned char	*source;
	unsigned char		*desti;

	i = 0;
	source = (const unsigned char *)src;
	desti = (unsigned char *)dst;
	while (i < n && dst != src) {
		desti[i] = source[i];
		i++;
	}
	return (dst);
}