/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 23:01:41 by vileleu           #+#    #+#             */
/*   Updated: 2026/06/15 16:44:15 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	*_realloc(void *ptr, size_t size) {
	if (!g_heap.page_size)
        init_heap();
	if (ptr == NULL)
		return _malloc(size);
    else if (size == 0) {
        _free(ptr);
        return NULL;
	}
    void    *new_ptr = _malloc(size);
    if (new_ptr == NULL)
        return NULL;
    // copy old data
    t_block *old_block = get_block_from_ptr(ptr);
    size_t  copy_size = old_block->size <= size ? old_block->size : size;
    ft_memcpy(new_ptr, ptr, copy_size);
    _free(ptr);
    return new_ptr;
}

void	*realloc(void *ptr, size_t size) {
    pthread_mutex_lock(&g_mutex);
    ptr = _realloc(ptr, size);
    pthread_mutex_unlock(&g_mutex);
    return ptr;
}