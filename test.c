/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:38:12 by vileleu           #+#    #+#             */
/*   Updated: 2026/06/17 22:16:11 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "malloc.h"

#define SHOW_MEM_ALLOC 0
#define NB_THREADS 4
#define NB_ITER 500

static int g_passed = 0;
static int g_failed = 0;
 
static void check(t_bool cond, char *msg) {
    if (cond) {
        ft_putstr("  ✓  ");
        ft_putstr(msg);
        ft_putstr("\n");
        g_passed++;
    }
    else {
        ft_putstr("  ✗  ");
        ft_putstr(msg);
        ft_putstr("\n");
        g_failed++;
    }
}
 
static void test_basic() {
    void    *p1 = malloc(1);
    check(p1 != NULL, "malloc(1) non NULL");
    void    *p2 = malloc(128);
    check(p2 != NULL, "malloc(128) non NULL");
    void    *p3 = malloc(1024);
    check(p3 != NULL, "malloc(1024) non NULL");
    void    *p4 = malloc(1025);
    check(p4 != NULL, "malloc(1025) LARGE non NULL");
    if (SHOW_MEM_ALLOC) {
        ft_putstr("\n");
        show_alloc_mem();
        ft_putstr("\n");
    }
    free(p1);
    free(p2);
    free(p3);
    free(p4);
}

static void test_null() {
    void    *p = malloc(0);
    check(p == NULL, "malloc(0) return NULL");
    free(NULL);
    check(1, "free(NULL) no crash");
}

static void test_read_write() {
    char    *s = malloc(64);
    ft_memcpy(s, "Hello, malloc!", 18);
    check(ft_strcmp(s, "Hello, malloc!") == 0, "data read");
    for (int i = 0; i < 64 - 1; i++) {
        s[i] = '2';
    }
    s[63] = 0;
    check(ft_strcmp(s, "222222222222222222222222222222222222222222222222222222222222222") == 0, "data write");
    if (SHOW_MEM_ALLOC) {
        ft_putstr("\n");
        show_alloc_mem();
        ft_putstr("\n");
    }
    free(s);
}

static void test_realloc() {
    char    *p = malloc(32);
    check(p != NULL, "malloc(32)");
    ft_memcpy(p, "ABCDEFGH", 9);
    char    *p2 = realloc(p, 256);
    check(p2 != NULL, "realloc(256) non NULL");
    check(strncmp(p2, "ABCDEFGH", 8) == 0, "data saved after realloc");
    char    *p3 = realloc(p2, 8);
    check(p3 != NULL, "realloc(8) non NULL");
    void    *p4 = realloc(NULL, 64);
    check(p4 != NULL, "realloc(NULL,64) non NULL");
    void    *p5 = realloc(p4, 0);
    check(p5 == NULL, "realloc(p,0) return NULL");
    if (SHOW_MEM_ALLOC) {
        ft_putstr("\n");
        show_alloc_mem();
        ft_putstr("\n");
    }
    free(p3);
}

static void test_big() {
#define SIZE 500
    void    *ptrs[SIZE];
    for (int i = 0; i < SIZE; i++)
        ptrs[i] = malloc((size_t)(i % 200 + 1));
    int all_distinct = 1;
    for (int i = 0; i < SIZE && all_distinct; i++)
        for (int j = i + 1; j < SIZE && all_distinct; j++)
            if (ptrs[i] == ptrs[j]) all_distinct = 0;
    check(all_distinct, "all pointer are differents");
    if (SHOW_MEM_ALLOC) {
        ft_putstr("\n");
        show_alloc_mem();
        ft_putstr("\n");
    }
    for (int i = 0; i < SIZE; i++)
        free(ptrs[i]);
    check(1, "free without crash");
#undef SIZE
}

/*
** THREADS
*/

static void *routine_malloc(void *arg) {
    int *errors = arg;
    for (int i = 0; i < NB_ITER; i++) {
        void *p = malloc(64);
        if (!p) {
            (*errors) = 1;
            continue;
        }
        ft_memset(p, 0xAB, 64);
        free(p);
    }
    return NULL;
}

static void *routine_realloc(void *arg) {
    int *errors = arg;
    for (int i = 0; i < NB_ITER; i++) {
        void *p = malloc(32);
        if (!p) {
            (*errors) = 1;
            continue;
        }
        ft_memset(p, 0xCD, 32);
        void *r = realloc(p, 128);
        if (!r) {
            free(p);
            (*errors) = 1;
            continue;
        }
        for (int k = 0; k < 32; k++) {
            if (((unsigned char *)r)[k] != 0xCD) {
                (*errors)++;
                break;
            }
        }
        free(r);
    }
    return NULL;
}

static void test_thread(void *(*routine)(void *)) {
    pthread_t t[NB_THREADS];
    int       e[NB_THREADS];
    for (int i = 0; i < NB_THREADS; i++) {
        e[i] = 0;
        pthread_create(&t[i], NULL, routine, &e[i]);
    }
    for (int i = 0; i < NB_THREADS; i++)
        pthread_join(t[i], NULL);
    int total = 0;
    for (int i = 0; i < NB_THREADS; i++)
        total += e[i];
    check(total == 0, "test thread ok");
}

int main(void) {
    ft_putstr("==========[TEST]==========\n");

    test_basic();
    test_null();
    test_read_write();
    test_realloc();
    test_big();
    test_thread(routine_malloc);
    test_thread(routine_realloc);

    ft_putstr("\n═══════════════════════════════════════\n");
    ft_putstr("  Result : ");
    ft_putnbr_fd(g_passed, 1);
    ft_putstr(" ✓  / ");
    ft_putnbr_fd(g_failed, 1);
    ft_putstr(" ✗\n");
    ft_putstr("═══════════════════════════════════════\n");
    return (g_failed > 0 ? 1 : 0);
}
