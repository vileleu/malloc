/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 14:46:13 by vileleu           #+#    #+#             */
/*   Updated: 2026/06/15 15:55:18 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_zone	*create_zone(t_zone_type type, size_t size) {
	if (type == LARGE)
		size = ALIGN_PAGE(HEADER_ZONE_SIZE + HEADER_BLOCK_SIZE + size);
	else
		size = ALIGN_PAGE(HEADER_ZONE_SIZE + (HEADER_BLOCK_SIZE + g_heap.size_max[type]) * PREALLOCATION);
	void	*zone_ptr = mmap(
		NULL,                        // address : NULL = kernel chooses
		size,                        // size
		PROT_READ | PROT_WRITE,      // read + write
		MAP_PRIVATE | MAP_ANONYMOUS, // private, no file
		-1,                          // fd: -1 for MAP_ANONYMOUS
		0                            // offset: 0 no file
	);
	if (zone_ptr == MAP_FAILED) {
		print_errno();
		return NULL;
	}
	t_zone	*zone = (t_zone *)zone_ptr;
	zone->next = NULL;
	zone->type = type;
	zone->size = size;
	t_block	*first_block = (t_block *)((char *)zone_ptr + HEADER_ZONE_SIZE);
	first_block->parent = zone;
	first_block->prev = NULL;
	first_block->next = NULL;
	first_block->free = TRUE;
	first_block->size = size - HEADER_ZONE_SIZE - HEADER_BLOCK_SIZE;
	zone->blocks = first_block;
	return (zone);
}

void	delete_zone(t_zone *zone) {
	t_zone_type	type = zone->type;
	t_zone		*start = g_heap.zones[type];
	if (start == zone) {
		g_heap.zones[type] = start->next;
		return;
	}
	while (start->next && start->next != zone)
		start = start->next;
	if (start->next == zone) {
		start->next = zone->next;
		munmap(zone, zone->size);
	}
}

void	add_zone(t_zone_type type, t_zone *zone) {
	t_zone	*tmp = g_heap.zones[type];
	if (tmp == NULL) {
		g_heap.zones[type] = zone;
		return;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = zone;
}

t_zone_type	get_zone_type(size_t size) {
	if (size <= g_heap.size_max[TINY])
		return TINY;
	else if (size <= g_heap.size_max[SMALL])
		return SMALL;
	return LARGE;
}

t_bool	zone_is_free(t_zone *zone) {
	// crash ici
	t_block	*start = zone->blocks;
	t_bool	free = TRUE;
	while (start) {
		if (start->free == FALSE)
			free = FALSE;
		start = start->next;
	}
	return free;
}
