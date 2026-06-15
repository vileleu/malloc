/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 22:57:43 by vileleu           #+#    #+#             */
/*   Updated: 2026/06/15 16:49:22 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

void    *_malloc(size_t size) {
	if (!g_heap.page_size)
        init_heap();
    if (size <= 0)
        return NULL;
    size = ALIGN(size);
    t_block     *block = get_or_create_block(size);
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
