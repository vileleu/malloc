/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 23:01:15 by vileleu           #+#    #+#             */
/*   Updated: 2026/06/20 17:31:09 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	_free(void *ptr) {
	if (!g_heap.page_size)
        init_heap();
	if (ptr == NULL)
		return;
	t_block	*block = get_block_from_ptr(ptr);
	if (block == NULL) {
		print_error("invalid pointer");
        return;
    }
	block->free = TRUE;
	t_zone	*zone = join_block(block);
	if (zone_is_empty(zone) && count_empty_zones(g_heap.zones[zone->type]) > 1)
		delete_zone(zone);
}

void	free(void *ptr) {
	pthread_mutex_lock(&g_mutex);
	_free(ptr);
	pthread_mutex_unlock(&g_mutex);
}