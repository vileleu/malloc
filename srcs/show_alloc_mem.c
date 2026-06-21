/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 23:34:32 by vileleu           #+#    #+#             */
/*   Updated: 2026/06/21 02:18:11 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	print_hex(unsigned long p) {
	char	*base = "0123456789abcdef";
	char	buf[17];
	int		i = 16;
	buf[i] = '\0';
	while (i > 0) {
		buf[--i] = base[p % 16];
		p /= 16;
	}
	ft_putstr("0x");
	ft_putstr(buf);
}

void	_show_alloc_mem() {
	if (!g_heap.page_size)
        init_heap();
	t_zone		*zone = NULL;
	t_zone_type	type = TINY;
	size_t		bytes = 0;
	while (type < 3) {
		zone = g_heap.zones[type];
		if (zone) {
			ft_putstr(type == TINY ? "TINY : " : (type == SMALL ? "SMALL : " : "LARGE : "));
			print_hex((unsigned long)((char *)zone));
			ft_putstr("\n");
		}
		while (zone) {
			t_block	*block = zone->blocks;
			while (block) {
				if (!block->free) {
					print_hex((unsigned long)((char *)block + HEADER_BLOCK_SIZE));
					ft_putstr(" - ");
					print_hex((unsigned long)((char *)block + HEADER_BLOCK_SIZE + block->requested_size));
					ft_putstr(" : ");
					ft_putsize(block->requested_size);
					ft_putstr(" bytes\n");
					bytes += block->requested_size;
				}
				/*
				else {
					print_hex((unsigned long)((char *)block + HEADER_BLOCK_SIZE));
					ft_putstr(" - ");
					print_hex((unsigned long)((char *)block + HEADER_BLOCK_SIZE + block->requested_size));
					ft_putstr(" : ");
					ft_putsize(block->requested_size);
					ft_putstr(" bytes (free space)\n");
				}
				*/
				block = block->next;
			}
			zone = zone->next;
		}
		type++;
	}
	ft_putstr("Total : ");
	ft_putsize(bytes);
	ft_putstr(" bytes\n");
}

void	show_alloc_mem() {
	pthread_mutex_lock(&g_mutex);
	_show_alloc_mem();
	pthread_mutex_unlock(&g_mutex);
}