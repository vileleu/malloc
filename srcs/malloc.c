/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 22:57:43 by vileleu           #+#    #+#             */
/*   Updated: 2026/06/20 21:41:41 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_heap			g_heap;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

void    *_malloc(size_t size) {
	if (!g_heap.page_size)
        init_heap();
    if (size == 0 || size > SIZE_MAX - HEADER_ZONE_SIZE - HEADER_BLOCK_SIZE - (g_heap.page_size - 1))
        return NULL;
    t_block     *block = get_or_create_block(ALIGN(size));
    if (block == NULL)
        return NULL;
    split_block(block, size);
    return ((char *)block + HEADER_BLOCK_SIZE);
}

void	*malloc(size_t size) {
    pthread_mutex_lock(&g_mutex);
    void    *ptr = _malloc(size);
    pthread_mutex_unlock(&g_mutex);
    return ptr;
}
