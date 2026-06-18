/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 23:01:41 by vileleu           #+#    #+#             */
/*   Updated: 2026/06/18 07:03:59 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	*_realloc(void *ptr, size_t size) {
	if (!g_heap.page_size)
        init_heap();
	if (ptr == NULL)
		return _malloc(size);
    else if (size == 0 || size > SIZE_MAX - HEADER_ZONE_SIZE - HEADER_BLOCK_SIZE - (g_heap.page_size - 1)) {
        _free(ptr);
        return NULL;
	}
    t_block *old_block = get_block_from_ptr(ptr);
    if (size == old_block->size)
        return ptr;
    // new size is smaller
    if (size < old_block->size) {
        split_block(old_block, size);
        return ptr;
    }
    // new size is bigger
    else {
        size_t  copy_size = old_block->size <= size ? old_block->size : size;
        void    *new_ptr = _malloc(size);
        if (new_ptr == NULL)
            return NULL;
        // save data
        ft_memcpy(new_ptr, ptr, copy_size);
        _free(ptr);
        return new_ptr;
    }
}

void	*realloc(void *ptr, size_t size) {
    pthread_mutex_lock(&g_mutex);
    ptr = _realloc(ptr, size);
    pthread_mutex_unlock(&g_mutex);
    return ptr;
}