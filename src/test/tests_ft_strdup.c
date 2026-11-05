/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_ft_strdup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svogrig <svogrig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 12:53:38 by svogrig           #+#    #+#             */
/*   Updated: 2026/11/05 12:54:51 by svogrig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int test_strdup(const char *title, const char *s, int alloc) {
    int fail;
    char *ft;
    char *libc;
    size_t len;
    int ft_errno;
    int libc_errno;
    struct rlimit lim_old;
    struct rlimit lim;
    print_title_test(title);

    len = strlen(s);
    
    if (alloc == ALLOC_FAIL) {
        getrlimit(RLIMIT_AS, &lim_old);
        lim = lim_old;
        lim.rlim_cur = 0;
        setrlimit(RLIMIT_AS, &lim);
    }
    errno = 4242;
    ft = ft_strdup(s);
    ft_errno = errno;
    errno = 4242;
    libc = strdup(s);
    libc_errno = errno;
    if (alloc == ALLOC_FAIL) {
        setrlimit(RLIMIT_AS, &lim_old);
    }

    printf("s* = %p | ft : str* = %-10p,  errno = %-4d | libc: str* = %-10p,  errno = %-4d ", (void *)s, (void *)ft, ft_errno, (void *)libc, libc_errno);
    fail = (ft_errno != libc_errno) || (ft == s);
    if ((ft == NULL) || (libc == NULL)) {
        fail |= (ft != libc);
    } else {
        fail |= (memcmp(ft, libc, len + 1) != 0);
    }

    free(ft);
    free(libc);
    print_result(fail);
    return (fail);
}

int tests_strdup(t_strings *s) {
    int fail;

    print_title_tests("Tests ft_strdup");
    fail = test_strdup("Empty string", s->empty, ALLOC_OK);
    fail += test_strdup("One char", s->onechar, ALLOC_OK);
    fail += test_strdup("All char", s->allchar, ALLOC_OK);
    fail += test_strdup("Very long string", s->verylong, ALLOC_OK);
    fail += test_strdup("Very long string", s->verylong, ALLOC_FAIL);
    return (fail);
}
