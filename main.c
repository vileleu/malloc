/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:38:12 by vileleu           #+#    #+#             */
/*   Updated: 2026/06/15 16:28:25 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "libft.h"
#include "malloc.h"

static int g_passed = 0;
static int g_failed = 0;
 
#define TEST(name) ft_putstr("\n[TEST]"); ft_putstr(name); ft_putstr("\n");
#define OK(cond, msg) \
    do { \
        if (cond) { ft_putstr("  ✓  "); ft_putstr(msg); ft_putstr("\n"); g_passed++; } \
        else      { ft_putstr("  ✗  "); ft_putstr(msg); ft_putstr("\n"); g_failed++; } \
    } while (0)
 
static void test_basic_alloc(void) {
    TEST("Allocation basique");
    void    *p1 = malloc(1);
    OK(p1 != NULL, "malloc(1) non NULL");
    void    *p2 = malloc(128);         /* TINY max */
    OK(p2 != NULL, "malloc(128) non NULL");
    OK(p1 != p2,   "deux pointeurs distincts");
    void    *p3 = malloc(1024);        /* SMALL max */
    OK(p3 != NULL, "malloc(1024) non NULL");
    void    *p4 = malloc(1025);        /* LARGE */
    OK(p4 != NULL, "malloc(1025) LARGE non NULL");
    show_alloc_mem();
    free(p1);
    free(p2);
    free(p3);
    free(p4);
}

static void test_null_cases(void) {
    TEST("Cas limites NULL");
    void    *p = malloc(0);
    OK(p == NULL, "malloc(0) renvoie NULL");
    free(NULL);
    OK(1, "free(NULL) sans crash");
}

static void test_write_read(void) {
    TEST("Écriture / lecture");
    char    *s = malloc(64);
    OK(s != NULL, "allocation réussie");
    memcpy(s, "Hello, malloc!", 18);
    OK(strcmp(s, "Hello, malloc!") == 0, "données correctement lues");
    show_alloc_mem();
    free(s);
}

static void test_alignment(void) {
    TEST("Alignement 16 octets");
    for (size_t sz = 1; sz <= 2048; sz += 7) {
        void    *p = malloc(sz);
        if (!p) continue;
        OK(((uintptr_t)p % 16) == 0, "pointeur aligné sur 16");
        show_alloc_mem();
        free(p);
        break;
    }
}

static void test_realloc(void) {
    TEST("realloc");
    char    *p = malloc(32);
    OK(p != NULL, "malloc(32)");
    memcpy(p, "ABCDEFGH", 9);
    char    *p2 = realloc(p, 256);
    OK(p2 != NULL, "realloc(256) non NULL");
    OK(strncmp(p2, "ABCDEFGH", 8) == 0, "données préservées après agrandissement");
    char    *p3 = realloc(p2, 8);
    OK(p3 != NULL, "realloc(8) non NULL");
    void    *p4 = realloc(NULL, 64);
    OK(p4 != NULL, "realloc(NULL,64) ~ malloc");
    void    *p5 = realloc(p4, 0);
    OK(p5 == NULL, "realloc(p,0) renvoie NULL");
    show_alloc_mem();
    free(p3);
}

static void test_many_allocs(void) {
    TEST("Nombreuses allocations / libérations");
#define SIZE 500
    void    *ptrs[SIZE];
    for (int i = 0; i < SIZE; i++)
        ptrs[i] = malloc((size_t)(i % 200 + 1));
    int all_distinct = 1;
    for (int i = 0; i < SIZE && all_distinct; i++)
        for (int j = i + 1; j < SIZE && all_distinct; j++)
            if (ptrs[i] == ptrs[j]) all_distinct = 0;
    OK(all_distinct, "tous les pointeurs sont distincts");
    show_alloc_mem();
    for (int i = 0; i < SIZE; i++)
        free(ptrs[i]);
    OK(1, "libérations sans crash");
#undef SIZE
}

static void test_show(void) {
    TEST("show_alloc_mem");
    void    *a = malloc(10);
    void    *b = malloc(500);
    void    *c = malloc(8000);
    show_alloc_mem();
    free(a);
    free(b);
    free(c);
    OK(1, "show_alloc_mem sans crash");
}

int main(void) {
    ft_putstr("═══════════════════════════════════════\n");
    ft_putstr("       malloc – suite de tests\n");
    ft_putstr("═══════════════════════════════════════\n");

    test_basic_alloc();
    test_null_cases();
    test_write_read();
    test_alignment();
    test_realloc();
    test_many_allocs();
    test_show();

    ft_putstr("\n═══════════════════════════════════════\n");
    ft_putstr("  Résultat : ");
    ft_putnbr_fd(g_passed, 1);
    ft_putstr(" ✓  / ");
    ft_putnbr_fd(g_failed, 1);
    ft_putstr(" ✗\n");
    ft_putstr("═══════════════════════════════════════\n");
    return (g_failed > 0 ? 1 : 0);
}
