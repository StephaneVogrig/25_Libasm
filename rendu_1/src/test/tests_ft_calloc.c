/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_ft_calloc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svogrig <svogrig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 12:20:38 by svogrig           #+#    #+#             */
/*   Updated: 2026/11/05 17:38:06 by svogrig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int test_calloc(size_t nmemb, size_t size, int alloc) {
    int fail;
    char *ft;
    char *libc;
    int ft_errno;
    int libc_errno;
    struct rlimit lim_old;
    struct rlimit lim;
    char title[TITLE_LEN];

    bzero(title, TITLE_LEN);
    snprintf(title, TITLE_LEN, "calloc(%zu, %zu) alloc %s", nmemb, size, alloc ? "fail" : "ok");
    print_title_test(title);
    if (alloc == ALLOC_FAIL) {
        getrlimit(RLIMIT_AS, &lim_old);
        lim = lim_old;
        lim.rlim_cur = 0;
        setrlimit(RLIMIT_AS, &lim);
    }
    errno = 4242;
    ft = ft_calloc(nmemb, size);
    ft_errno = errno;
    errno = 4242;
    libc = calloc(nmemb, size);
    libc_errno = errno;
    if (alloc == ALLOC_FAIL) {
        setrlimit(RLIMIT_AS, &lim_old);
    }
    printf("ft : str* = %-10p,  errno = %-4d | libc: str* = %-10p,  errno = %-4d ", (void *)ft, ft_errno, (void *)libc, libc_errno);
    fail = (ft_errno != libc_errno);
    if ((ft == NULL) || (libc == NULL)) {
        fail |= (ft != libc);
    } else {
        fail |= (memcmp(ft, libc, nmemb * size) != 0);
    }
    free(ft);
    free(libc);
    print_result(fail);

    return (fail);
}

int tests_calloc(void) {
    int fail;

    print_title_tests("Tests ft_calloc");
    fail = test_calloc(1, 10, ALLOC_OK);
    fail = test_calloc(8, 10, ALLOC_OK);
    fail = test_calloc(8, 0, ALLOC_OK);
    fail = test_calloc(0, 10, ALLOC_OK);
    fail = test_calloc(0, 10, ALLOC_OK);
    fail = test_calloc(MAX_SIZE, 2, ALLOC_OK);
    fail = test_calloc(1000, 1000, ALLOC_FAIL);

    return (fail);
}
