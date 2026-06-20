/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 22:58:54 by vileleu           #+#    #+#             */
/*   Updated: 2026/06/20 17:25:48 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
#define MALLOC_H

#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define DEBUGG(msg) ft_putstr("\n[DEBUGG] "); ft_putstr(msg); ft_putstr("\n");

#define SUCCESS 0
#define FAILURE 1

#define N 128
#define M 1024
#define PREALLOCATION 100
#define ALIGN_MAX 16

#define ALIGN(n) (((n) + (ALIGN_MAX) - 1) & ~((ALIGN_MAX) - 1))
#define ALIGN_PAGE(n) (((n) + g_heap.page_size - 1) & ~(g_heap.page_size - 1))

typedef enum	e_bool {
    FALSE = 0,
    TRUE = 1
}				t_bool;

typedef enum	e_zone_type {
    TINY,
    SMALL,
    LARGE
}				t_zone_type;

typedef struct s_zone  t_zone;

typedef struct	s_block {
	t_zone        	*parent;
	struct s_block	*prev;
	struct s_block	*next;
	size_t			size;
	t_bool			free;
}				t_block;

typedef struct	s_zone {
	t_block			*blocks;
	struct s_zone	*next;
	size_t			size;
	t_zone_type		type;
}				t_zone;

typedef struct	s_heap {
    t_zone	*zones[3];
    size_t  size_max[3];
    size_t  page_size;
}				t_heap;

extern t_heap			g_heap;
extern pthread_mutex_t	g_mutex;

#define HEADER_ZONE_SIZE ALIGN(sizeof(t_zone))
#define HEADER_BLOCK_SIZE ALIGN(sizeof(t_block))

/*
** MAIN FUNCTIONS
*/

void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);
void	free(void *ptr);

void	*_malloc(size_t size);
void	*_realloc(void *ptr, size_t size);
void	_free(void *ptr);

/*
** BLOCK FUNCTIONS
*/

t_block	*get_or_create_block(size_t size);
t_block	*get_last_block(t_block *block);
void	split_block(t_block *block, size_t size);
t_block	*get_block_from_ptr(void *ptr);
t_zone	*join_block(t_block *block);

/*
** ZONE FUNCTIONS
*/

t_zone		*create_zone(t_zone_type type, size_t size);
void		delete_zone(t_zone *zone);
void		add_zone(t_zone_type type, t_zone *zone);
t_zone_type	get_zone_type(size_t size);
t_bool		zone_is_empty(t_zone *zone);
int			count_empty_zones(t_zone *zones);

/*
** HEAP FUNCTIONS
*/

void	init_heap();

/*
** PRINT FUNCTIONS
*/

void	print_hex(unsigned long p);
void	_show_alloc_mem();
void	show_alloc_mem();


/*
** UTILS FUNCTIONS
*/

void	ft_putchar(char c);
size_t	ft_strlen(const char *s);
void	ft_putstr_fd(char *s, int fd);
void	ft_putstr(char *s);
void	ft_putsize(size_t n);
int		ft_strcmp(const char *s1, const char *s2);
void	*ft_memcpy(void *dst, const void *src, size_t n);

/*
** ERROR FUNCTIONS
*/

void	print_error(char *msg);
void	print_errno();

#endif
