/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 02:34:29 by vileleu           #+#    #+#             */
/*   Updated: 2026/06/17 02:34:30 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** test_thread_safety.c
** Tests thread-safety — malloc / realloc / free
**
** Compilation :
**   gcc -Wall -Wextra -pthread -g3 test_thread_safety.c -L. -lmalloc -o test
**   LD_LIBRARY_PATH=. ./test
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>
#include <stdatomic.h>

#define NB_THREADS  4
#define NB_ITER     500

#define GREEN  "\033[32m"
#define RED    "\033[31m"
#define CYAN   "\033[36m"
#define BOLD   "\033[1m"
#define RESET  "\033[0m"

static void print_result(const char *name, int ok)
{
    if (ok)
        printf("  %-45s %s[OK]%s\n", name, GREEN, RESET);
    else
        printf("  %-45s %s[FAIL]%s\n", name, RED, RESET);
}

/* ─── Test 1 : malloc / free concurrent ─────────────────────── */

static void *t1_worker(void *arg)
{
    int *errors = arg;
    for (int i = 0; i < NB_ITER; i++)
    {
        void *p = malloc(64);
        if (!p) { (*errors)++; continue; }
        memset(p, 0xAB, 64);
        free(p);
    }
    return NULL;
}

static int test_malloc_free(void)
{
    pthread_t t[NB_THREADS];
    int       e[NB_THREADS];

    for (int i = 0; i < NB_THREADS; i++) { e[i] = 0; pthread_create(&t[i], NULL, t1_worker, &e[i]); }
    for (int i = 0; i < NB_THREADS; i++) pthread_join(t[i], NULL);
    int total = 0; for (int i = 0; i < NB_THREADS; i++) total += e[i];
    return total == 0;
}

/* ─── Test 2 : realloc concurrent ───────────────────────────── */

static void *t2_worker(void *arg)
{
    int *errors = arg;
    for (int i = 0; i < NB_ITER; i++)
    {
        void *p = malloc(32);
        if (!p) { (*errors)++; continue; }
        memset(p, 0xCD, 32);
        void *q = realloc(p, 128);
        if (!q) { free(p); (*errors)++; continue; }
        /* Les premiers octets doivent être préservés */
        for (int k = 0; k < 32; k++)
            if (((unsigned char *)q)[k] != 0xCD) { (*errors)++; break; }
        free(q);
    }
    return NULL;
}

static int test_realloc(void)
{
    pthread_t t[NB_THREADS];
    int       e[NB_THREADS];

    for (int i = 0; i < NB_THREADS; i++) { e[i] = 0; pthread_create(&t[i], NULL, t2_worker, &e[i]); }
    for (int i = 0; i < NB_THREADS; i++) pthread_join(t[i], NULL);
    int total = 0; for (int i = 0; i < NB_THREADS; i++) total += e[i];
    return total == 0;
}

/* ─── Test 3 : malloc(0) et realloc(NULL, size) ─────────────── */

static void *t3_worker(void *arg)
{
    int *errors = arg;
    for (int i = 0; i < NB_ITER; i++)
    {
        /* malloc(0) : retourne NULL ou pointeur libérable */
        void *p = malloc(0);
        if (p) free(p);

        /* realloc(NULL, n) == malloc(n) */
        void *q = realloc(NULL, 64);
        if (!q) { (*errors)++; continue; }
        free(q);

        /* realloc(ptr, 0) == free */
        void *r = malloc(64);
        if (!r) { (*errors)++; continue; }
        void *s = realloc(r, 0);
        if (s) free(s);
    }
    return NULL;
}

static int test_edge_cases(void)
{
    pthread_t t[NB_THREADS];
    int       e[NB_THREADS];

    for (int i = 0; i < NB_THREADS; i++) { e[i] = 0; pthread_create(&t[i], NULL, t3_worker, &e[i]); }
    for (int i = 0; i < NB_THREADS; i++) pthread_join(t[i], NULL);
    int total = 0; for (int i = 0; i < NB_THREADS; i++) total += e[i];
    return total == 0;
}

/* ─── Test 4 : alignement des pointeurs ─────────────────────── */

static void *t4_worker(void *arg)
{
    int *errors = arg;
    for (int i = 0; i < NB_ITER; i++)
    {
        void *p = malloc((size_t)(i % 256 + 1));
        if (!p) { (*errors)++; continue; }
        if ((uintptr_t)p % sizeof(void *) != 0) (*errors)++;
        free(p);
    }
    return NULL;
}

static int test_alignment(void)
{
    pthread_t t[NB_THREADS];
    int       e[NB_THREADS];

    for (int i = 0; i < NB_THREADS; i++) { e[i] = 0; pthread_create(&t[i], NULL, t4_worker, &e[i]); }
    for (int i = 0; i < NB_THREADS; i++) pthread_join(t[i], NULL);
    int total = 0; for (int i = 0; i < NB_THREADS; i++) total += e[i];
    return total == 0;
}

/* ─── Test 5 : free depuis un thread différent ───────────────── */

#define SLOTS 16

typedef struct {
    _Atomic(void *) ptr;
    atomic_int      ready;
} t_slot;

static t_slot g_slots[SLOTS];

static void *t5_producer(void *arg)
{
    int base = *(int *)arg * (SLOTS / (NB_THREADS / 2));
    for (int i = 0; i < SLOTS / (NB_THREADS / 2); i++)
    {
        void *p = malloc(64);
        if (p) memset(p, (int)(i & 0xFF), 64);
        atomic_store(&g_slots[base + i].ptr, p);
        atomic_store(&g_slots[base + i].ready, 1);
    }
    return NULL;
}

static void *t5_consumer(void *arg)
{
    int  base   = *(int *)arg * (SLOTS / (NB_THREADS / 2));
    int *errors = NULL; (void)errors;
    for (int i = 0; i < SLOTS / (NB_THREADS / 2); i++)
    {
        while (!atomic_load(&g_slots[base + i].ready))
            sched_yield();
        void *p = atomic_load(&g_slots[base + i].ptr);
        if (p) free(p);
    }
    return NULL;
}

static int test_cross_thread_free(void)
{
    pthread_t prod[NB_THREADS / 2];
    pthread_t cons[NB_THREADS / 2];
    int       ids[NB_THREADS / 2];

    memset(g_slots, 0, sizeof(g_slots));
    for (int i = 0; i < NB_THREADS / 2; i++)
    {
        ids[i] = i;
        pthread_create(&prod[i], NULL, t5_producer, &ids[i]);
        pthread_create(&cons[i], NULL, t5_consumer, &ids[i]);
    }
    for (int i = 0; i < NB_THREADS / 2; i++)
    {
        pthread_join(prod[i], NULL);
        pthread_join(cons[i], NULL);
    }
    return 1;
}

/* ─── Main ───────────────────────────────────────────────────── */

int main(void)
{
    printf("\n%s%s Thread-Safety Tests — malloc / realloc / free %s\n",
           BOLD, CYAN, RESET);
    printf("  %d threads  |  %d itérations/thread\n\n",
           NB_THREADS, NB_ITER);

    typedef struct { const char *name; int (*fn)(void); } t_test;
    t_test tests[] = {
        { "malloc + free concurrent",              test_malloc_free      },
        { "realloc concurrent (données préservées)", test_realloc        },
        { "cas limites (malloc(0), realloc(NULL))", test_edge_cases      },
        { "alignement des pointeurs",              test_alignment        },
        { "free depuis un autre thread",           test_cross_thread_free},
        { NULL, NULL }
    };

    int passed = 0, total = 0;
    for (int i = 0; tests[i].fn; i++)
    {
        int ok = tests[i].fn();
        print_result(tests[i].name, ok);
        passed += ok;
        total++;
    }

    printf("\n");
    if (passed == total)
        printf("  %s%s✓ %d/%d tests passent%s\n\n", BOLD, GREEN, passed, total, RESET);
    else
        printf("  %s%s✗ %d/%d tests échoués%s\n\n", BOLD, RED, total - passed, total, RESET);

    return passed == total ? 0 : 1;
}