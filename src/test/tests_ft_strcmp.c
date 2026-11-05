/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_ft_strcmp.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svogrig <svogrig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 13:27:38 by svogrig           #+#    #+#             */
/*   Updated: 2026/11/05 13:35:37 by svogrig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"


/* return 1 or 0 or -1*/
static int sign(int n) {
    return (n > 0) - (n < 0);
}

int test_strcmp(const char *title, const char *s1, const char *s2) {
    int fail;
    int result_ft;
    int result;

    print_title_test(title);
    result_ft = ft_strcmp(s1, s2);
    result = strcmp(s1, s2);
    printf("ft_strcmp = %-10i | strcmp = %-10i ", result_ft, result);
    fail = (sign(result_ft) != sign(result));
    print_result(fail);

    return fail;
}

int tests_strcmp(t_strings *s) {
    int fail;
    char title[64];
    long unsigned int n;

    print_title_tests("Tests ft_strcmp");
    fail = 0;
    const char *names[] = {"Empty string", "Short string", "All characters", "Long string", "abc", "abcd", "12345", "12045"};
    const char *strings[]  = {s->empty, s->onechar, s->allchar, s->verylong, "abc", "abcd", "12345", "12045"};
    n = sizeof(names) / sizeof(names[0]);
    for (long unsigned int i = 0; i < n; i++) {
        for (long unsigned int j = 0; j < n; j++) {
            snprintf(title, sizeof title, "%s vs %s", names[i], names[j]);
            fail += test_strcmp(title, strings[i], strings[j]);
        }
    }

    fail += test_strcmp("unsigned > 127",   "\xff", "\x01");
    fail += test_strcmp("unsigned reverse", "\x01", "\xff");

    return fail;
}
