/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 19:12:34 by vileleu           #+#    #+#             */
/*   Updated: 2026/06/18 05:43:18 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static t_bool	have_space(t_block *block, size_t size) {
	if (block->free && block->size >= size + HEADER_BLOCK_SIZE)
		return TRUE;
	return FALSE;
}

t_block	*get_last_block(t_block *block) {
	if (!block) return NULL;
	while (block->next)
		block = block->next;
	return block;
}

t_block	*get_or_create_block(size_t size) {
	t_zone		*zone = NULL;
	t_block		*block = NULL;
    t_zone_type	type = get_zone_type(size);
	zone = g_heap.zones[type];
	while (zone) {
		block = zone->blocks;
		while (block) {
			if (have_space(block, size))
				return block;
			block = block->next;
		}
		zone = zone->next;
	}
	zone = create_zone(type, size);
	if (zone == NULL)
		return NULL;
	add_zone(type, zone);
	return zone->blocks;
}

void	split_block(t_block *block, size_t size) {
	// no split if not enough space in the remaining block (need 1 byte at least)
	if (block->size - size <= HEADER_BLOCK_SIZE) return;
	t_block *new_block = (t_block *)((char *)block + HEADER_BLOCK_SIZE + size);
	size_t	new_size = block->size - size - HEADER_BLOCK_SIZE;
	new_block->parent = block->parent;
	new_block->prev = block;
	new_block->next = block->next;
	new_block->size = new_size;
	new_block->free = TRUE;
	if (block->next)
    	block->next->prev = new_block;
	block->next = new_block;
	block->size = size;
	block->free = FALSE;
}

/*
	SPLIT DIAGRAM

	4096: block = [header | data ...]
                  0       32        4096
	if we split at 4032 (we want 4000 bytes)
	4096: block = [header 1 | data 1 ... | header 2 | data 2 ...]
	              0         32           4032       4064        4096
	if we split at 4070
	4096: block = [header 1 | data 1 ... | header 2 | data 2 ...]
	              0         32           4070       4102        4102 (memory overflow)
*/

t_block	*get_block_from_ptr(void *ptr) {
	return (t_block *)((char *)ptr - HEADER_BLOCK_SIZE);
}

t_zone	*join_block(t_block *block) {
	t_zone	*zone = block->parent;
	t_block	*prev = block->prev;
	t_block	*next = block->next;
	// both block side are free
	if (prev && prev->free == TRUE && next && next->free == TRUE) {
		prev->next = next->next;
		prev->size += block->size + HEADER_BLOCK_SIZE + next->size + HEADER_BLOCK_SIZE;
		if (next->next)
			next->next->prev = prev;
	}
	// only prev is free
	else if (prev && prev->free == TRUE) {
		prev->next = next;
		prev->size += block->size + HEADER_BLOCK_SIZE;
		if (next)
			next->prev = prev;
	}
	// only next is free
	else if (next && next->free == TRUE) {
		block->next = next->next;
		block->size += next->size + HEADER_BLOCK_SIZE;
		if (next->next)
			next->next->prev = block;
	}
	return zone;
}

/*
	JOIN DIAGRAM

	START:
	block = [ header 1 | data 1 ... | header 2 | data 2 ... | header 3 | data 3 ... ]
            0          32           100        132          1000       1032         4096
	block 1 prev = NULL
	block 1 next = block 2
	block 1 size = 68

	block 2 prev = block 1
	block 2 next = block 3
	block 2 size = 868

	block 3 prev = block 2
	block 3 next = NULL
	block 3 size = 3064

	JOIN WITH BLOCK 1:
	block = [ header 1 | data 1 ... | header 3 | data 3 ...]
	        0          32           1000       1032        4096
	block 1 prev = NULL
	block 1 next = block 3
	block 1 size = 968

	block 3 prev = block 1
	block 3 next = NULL
	block 3 size = 3064

	JOIN WITH BLOCK 3:
	block = [ header 1 | data 1 ... ]
	        0          32           4096
	block 1 prev = NULL
	block 1 next = NULL
	block 1 size = 4064
*/
