/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svogrig <svogrig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 12:48:40 by svogrig           #+#    #+#             */
/*   Updated: 2026/11/05 13:40:14 by svogrig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void print_title_tests(const char *title) {
    printf("\n%s%s%s\n", YELLOW, title, NC);
}

void print_title_test(const char *title) {
    printf("%-35s ... ", title);
}

void print_result(const int fail) {
    char *result;
    char *couleur;

    result = fail ? "KO" : "OK" ;
    couleur = fail ? RED : GREEN;
    printf("%s[%s]%s\n", couleur, result, NC);
}

void print_skip(void) {
    printf("%s[SKIP]%s\n", YELLOW, NC);
}

void print_failed(const char*template, const char *arg) {
    printf(template, arg);
    print_skip();
}

int display_analyze_results(t_results r, const char *fct) {
    printf("ft_%s = %-10zd, errno %-4i | %s = %-10zd, errno %-4i ", fct, r.ft_result, r.ft_errno, fct, r.libc_result, r.libc_errno);
    return ((r.ft_result != r.libc_result) || (r.ft_errno != r.libc_errno));
}
