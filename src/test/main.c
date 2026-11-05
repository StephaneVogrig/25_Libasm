/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svogrig <svogrig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 19:03:14 by svogrig           #+#    #+#             */
/*   Updated: 2026/11/05 17:40:26 by svogrig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static t_strings *init_strings(void) {
    t_strings *s;

    s = run_calloc(1, sizeof(t_strings));
    if (s == NULL) {
        return (NULL);
    }
    s->onechar[0] = '1';
    for(int i = 0; i < 255; i++) {
        s->allchar[i] = i + 1;
    }
    memset(s->verylong, 'A', LEN_VERYLONG);

    return (s);
}

int main(void) {
    t_strings *strs_test;
    int fail;

    strs_test = init_strings();
    if (strs_test == NULL) {
        return (1);
    }
    // for no buffering stdout
    setvbuf(stdout, NULL, _IONBF, 0);
    // for no crash when SIGPIPE
    signal(SIGPIPE, SIG_IGN);

    fail = 0;
    fail += tests_strlen(strs_test);
    fail += tests_strcpy(strs_test);
    fail += tests_strcmp(strs_test);
    fail += tests_write(strs_test);
    fail += tests_read();
    fail += tests_strdup(strs_test);
    fail += tests_calloc();

    printf("\n%sTests failed :%s %i\n", YELLOW, NC, fail);
    free(strs_test);
    return (fail != 0);
}
