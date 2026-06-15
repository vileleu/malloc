/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 14:57:36 by vileleu           #+#    #+#             */
/*   Updated: 2026/06/12 17:31:43 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	init_heap() {
	g_heap.zones[TINY] = NULL;
	g_heap.zones[SMALL] = NULL;
	g_heap.zones[LARGE] = NULL;
	g_heap.page_size = sysconf(_SC_PAGESIZE);
	g_heap.size_max[TINY] = N;
	g_heap.size_max[SMALL] = M;
	g_heap.size_max[LARGE] = 0;
}
