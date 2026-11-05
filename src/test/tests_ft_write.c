/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_ft_write.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svogrig <svogrig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 13:27:19 by svogrig           #+#    #+#             */
/*   Updated: 2026/11/05 13:52:20 by svogrig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int compare_read_out(int fd1, int fd2, size_t count) {
    ssize_t len1;
    ssize_t len2;
    char *out1;
    char *out2;
    int fail;

    fail = 0;
    out1 = run_calloc(count, 1);
    out2 = run_calloc(count, 1);
    if ((out1 == NULL) || (out2 == NULL)) {
        fail = -1;
    }
    if (fail >= 0) {
        len1 = read(fd1, out1, count);
        len2 = read(fd2, out2, count);
        if ((len1 == -1) || (len2 == -1)) {
            printf("read failed ");
            fail = -1;
        } else {
            fail = (len1 != len2);
        }
    }
    if (fail >= 0) {
        printf ("len1 = %7zd, len2 = %7zd ", len1, len2);
        fail |= run_memcmp(out1, out2, count);
    }
    free(out1);
    free(out2);
    return (fail);
}

int test_write_results(int fd_ft, int fd_libc, const void *buf, size_t count) {
    t_results r;
    int fail;

    errno = 4242;
    r.ft_result = ft_write(fd_ft, buf, count);
    r.ft_errno = errno;
    errno = 4242;
    r.libc_result = write(fd_libc, buf, count);
    r.libc_errno = errno;

    fail = display_analyze_results(r, "write");

    return (fail);
}

int test_write_core(int fd, const void *buf, size_t count) {
    int fail;

    fail = test_write_results(fd, fd, buf, count);
    print_result(fail);

    return (fail);
}

int test_write(const char *title, int fd, const void *buf, size_t count) {
    int fail;

    print_title_test(title);
    fail = test_write_core(fd, buf, count);
    return fail;
}

int test_write_pipe_side(const char *title, const void *buf, size_t count, int side) {
    int fail;
    int fd[2];

    print_title_test(title);
    if (!create_pipe(fd)) {
        return (0);
    }
    fail = test_write_core(fd[side], buf, count);
    close_fds(fd);
    return (fail);
}

int test_write_pipe_content(const char *title, const char *str) {
    int fail;
    int fd_ft[2];
    int fd_libc[2];
    size_t  count;
    int cro;

    print_title_test(title);
    count = strlen(str) + 1;

    if (!create_pipe(fd_ft)) {
        return (0);
    }
    if (!create_pipe(fd_libc)) {
        close_fds(fd_ft);
        return (0);
    }

    fail = test_write_results(fd_ft[1], fd_libc[1], str, count);

    close(fd_ft[1]);
    close(fd_libc[1]);

    cro = compare_read_out(fd_ft[0], fd_libc[0], count + 1);
    if ((cro >= 0) || fail) {
        fail |= (cro > 0);
        print_result(fail);
    } else {
        print_skip();
    }

    close(fd_ft[0]);
    close(fd_libc[0]);

    return (fail);
}

int test_write_file(const char *title, const char *path, int flags,  const void *buf, size_t count) {
    int fail;
    int fd;

    print_title_test(title);
    fd = run_open(path, flags);
    if (fd == -1) {
        return (0);
    }
    fail = test_write_core(fd, buf, count);
    close(fd);

    return (fail);
}

int test_write_tmpfile(const char *title, const void *buf, size_t count) {
    char path_ft[] = "/tmp/libasm_ft_XXXXXX";
    char path_libc[] = "/tmp/libasm_libc_XXXXXX";
    int fd_ft;
    int fd_libc;
    int fail;
    int cro;

    print_title_test(title);
    fd_ft = run_mkstemp(path_ft);
    if (fd_ft == -1) {
        return (0);
    }
    fd_libc = run_mkstemp(path_libc);
    if (fd_libc == -1) {
        close(fd_ft);
        unlink(path_ft);
        return (0);
    }
    fail = test_write_results(fd_ft, fd_libc, buf, count);

    lseek(fd_ft, 0, SEEK_SET);
    lseek(fd_libc, 0, SEEK_SET);
    cro = compare_read_out(fd_ft, fd_libc, count + 1);
    if ((cro >= 0) || fail) {
        fail |= (cro > 0);
        print_result(fail);
    } else {
        print_skip();
    }

    close(fd_ft);
    close(fd_libc);
    unlink(path_ft);
    unlink(path_libc);

    return (fail);
}

int test_write_pipe_read_closed(const char *title) {
    int fail;
    int fd[2];

    print_title_test(title);
    if (!create_pipe(fd)) {
        return (0);
    };
    close(fd[0]);
    fail = test_write_core(fd[1], "A", 1);
    close(fd[1]);

    return (fail);
}

int test_write_pipe_full(const char *title) {
    int fail;
    int fd[2];
    char buf[4096] = {0};

    print_title_test(title);
    if (!create_pipe(fd)) {
        return (0);
    };
    while (write(fd[1], buf, sizeof buf) > 0) {
    }
    fail = test_write_core(fd[1], "A", 1);
    close_fds(fd);

    return (fail);
}

int test_write_pipe_closed(const char *title) {
    int fd[2];

    print_title_test(title);
    if (!create_pipe(fd)) {
        return (0);
    };
    close_fds(fd);

    return (test_write_core(fd[1], "A", 1));
}

int tests_write(t_strings *s) {
    int fail;

    print_title_tests("Tests ft_write");
    fail = test_write("Invalid fd=-1, error EBADF", -1, s->onechar, 1);
    fail += test_write("Invalid fd=99999, error EBADF", 99999, s->onechar, 1);

    fail += test_write_pipe_side("Buffer null, error EFAULT", NULL, 1, PIPE_WR);
    fail += test_write_pipe_side("Len 0", s->onechar, 0, PIPE_WR);
    fail += test_write_pipe_side("Buffer null, len 0", NULL, 0, PIPE_WR);
    fail += test_write_pipe_side("Bad pointer (0x1), error EFAULT", (void *)1, 1, PIPE_WR);
    fail += test_write_pipe_side("Wrong pipe side, error EBADF", s->onechar, 1, PIPE_RD);

    fail += test_write_pipe_full("Pipe full, error EAGAIN");
    fail += test_write_pipe_read_closed("Pipe closed on read, error EPIPE");
    fail += test_write_pipe_closed("Closed fd, error EBADF");

    fail += test_write_file("Disk full, error ENOSPC", "/dev/full", O_WRONLY, s->onechar, 1);
    fail += test_write_file("Read-only fd, error EBADF", "/dev/null", O_RDONLY, s->onechar, 1);
    fail += test_write_file("Long string, /dev/null", "/dev/null", O_WRONLY, s->verylong, LEN_VERYLONG);
    fail += test_write_file("Huge count, /dev/null, error EFAULT", "/dev/null", O_WRONLY, s->onechar, (size_t)-1);
    fail += test_write_file("Count 2^32+1, /dev/null", "/dev/null", O_WRONLY, s->onechar, ((size_t)1 << 32) + 1);

    fail += test_write_tmpfile("Tmpfile: null string, error EFAULT",   NULL,    1);
    fail += test_write_tmpfile("Tmpfile: 0 bytes",   s->empty,    0);
    fail += test_write_tmpfile("Tmpfile: empty string",   s->empty,    1);
    fail += test_write_tmpfile("Tmpfile: short string",   s->onechar,  1);
    fail += test_write_tmpfile("Tmpfile: all characters", s->allchar,  255);
    fail += test_write_tmpfile("Tmpfile: long string",    s->verylong, LEN_VERYLONG);

    fail += test_write_pipe_content("Pipe: empty string", s->empty);
    fail += test_write_pipe_content("Pipe: short string", s->onechar);
    fail += test_write_pipe_content("Pipe: all characters", s->allchar);
    fail += test_write_pipe_content("Pipe: long string (partial)", s->verylong);
    return (fail);
}
