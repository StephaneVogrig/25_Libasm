/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_ft_strlen.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svogrig <svogrig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 13:27:55 by svogrig           #+#    #+#             */
/*   Updated: 2026/11/05 13:30:33 by svogrig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int test_strlen(const char *title, const char *string) {
    size_t result_ft;
    size_t result;
    int fail;

    print_title_test(title);
    result_ft = ft_strlen(string);
    result = strlen(string);
    printf("ft_strlen = %-10zu | strlen = %-10zu ", result_ft, result);
    fail = result_ft != result;
    print_result(fail);
    return fail;
}

int tests_strlen(t_strings *s) {
    int fail;

    print_title_tests("Tests ft_strlen");
    fail = test_strlen("Empty string", s->empty);
    fail += test_strlen("Short string", s->onechar);
    fail += test_strlen("All characters", s->allchar);
    fail += test_strlen("Long string", s->verylong);
    return (fail);
}
