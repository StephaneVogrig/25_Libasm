/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svogrig <svogrig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 13:37:53 by svogrig           #+#    #+#             */
/*   Updated: 2026/11/05 13:47:34 by svogrig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int create_pipe(int *fds) {
    int ok;

    ok = (pipe2(fds, O_NONBLOCK) == 0);
    if (!ok) {
        printf("Open pipe failed ");
        print_skip();
    }
    return (ok);
}

void close_fds(int *fd) {
        close(fd[0]);
        close(fd[1]);
}

void *run_calloc(size_t nmemb, size_t size) {
    void *ptr;

    ptr = calloc(nmemb, size);
    if (ptr == NULL){
        printf("Calloc failed ");
    }
    return (ptr);
}

int run_memcmp(const void *buf1, const void *buf2, size_t n) {
    int diff;

    diff = memcmp(buf1, buf2, n) != 0;
    printf("ft_buf %c= libc_buf ", diff ? '!' : '=');
    return (diff);
}

int run_open(const char *path, int flags) {
    int fd;

    fd = open(path, flags);
    if (fd == -1) {
        printf("failed to open %s ", path);
        print_skip();
    }
    return (fd);
}

int run_mkstemp(char *path){
    int fd;

    fd = mkstemp(path);
    if (fd == -1) {
        printf("failed to create %s ", path);
        print_skip();
    }
    return (fd);
}
