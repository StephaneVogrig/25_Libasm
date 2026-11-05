/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svogrig <svogrig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 12:21:10 by svogrig           #+#    #+#             */
/*   Updated: 2026/11/05 14:00:43 by svogrig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

#define _GNU_SOURCE // pipe2
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include "libasm.h"

#define RED		    "\033[31m"
#define GREEN	    "\033[32m"
#define YELLOW	    "\033[33m"
#define NC			"\033[0m"

#define PIPE_RD 0
#define PIPE_WR 1

#define SUCCESS 0
#define FAILURE 1

#define LEN_BUF 100
#define TITLE_LEN 40

#define LEN_VERYLONG   1000000

#define MAX_SIZE 0xFFFFFFFFFFFFFFFF

typedef enum e_buf {
    BUF_OK,
    BUF_NULL,
    BUF_BAD
} t_buf;

typedef enum e_source_type {
    SRC_INVALID,
    SRC_CLOSED,
    SRC_DIR,
    SRC_DEV_NULL,
    SRC_DEV_ZERO,
    SRC_FILE,
    SRC_FILE_WRONLY,
    SRC_PIPE_DATA,
    SRC_PIPE_EMPTY,
    SRC_PIPE_WR_CLOSED
} t_srctype;

typedef enum e_alloc {
    ALLOC_OK,
    ALLOC_FAIL
} t_alloc;

typedef struct s_strings{
    char empty[1];
    char onechar[2];
    char allchar[256];
    char verylong[LEN_VERYLONG + 1];
} t_strings;

typedef struct s_results {
    ssize_t ft_result;
    int ft_errno;
    ssize_t libc_result;
    int libc_errno;
} t_results;

typedef struct s_pair_fd {
    int ft;
    int libc;
    int closed;
} t_pairfd;

typedef struct s_source {
    t_pairfd fd;
    t_pairfd pipe;
    char     path[32];
} t_source;

// tests functions -----------------------------------//
int tests_strlen(t_strings *s);
int tests_strcpy(t_strings *s);
int tests_strcmp(t_strings *s);
int tests_write(t_strings *s);
int tests_read(void);
int tests_strdup(t_strings *s);
int tests_calloc(void);

// display functions ---------------------------------//
void print_title_tests(const char *title);
void print_title_test(const char *title);
void print_result(const int fail);
void print_skip(void);
void print_failed(const char*template, const char *arg);
int display_analyze_results(t_results r, const char *fct);

// utils --------------------------------------------//
int create_pipe(int *fds);
void close_fds(int *fd);
void *run_calloc(size_t nmemb, size_t size);
int run_memcmp(const void *buf1, const void *buf2, size_t n);
int run_open(const char *path, int flags);
int run_mkstemp(char *path);

#endif
