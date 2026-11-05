/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svogrig <svogrig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 19:03:14 by svogrig           #+#    #+#             */
/*   Updated: 2026/11/05 03:40:35 by svogrig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
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

#define LEN_VERYLONG   1000000

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

t_strings *init_strings(void) {
    t_strings *s;

    s = calloc(1, sizeof(t_strings));
    if (s == NULL) {
        printf("Calloc failed for strings test !\n");
        return (NULL);
    }
    s->onechar[0] = '1';
    for(int i = 0; i < 255; i++) {
        s->allchar[i] = i + 1;
    }
    memset(s->verylong, 'A', LEN_VERYLONG);

    return (s);
}

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
/* return 1 or 0 or -1*/
static int sign(int n) {
    return (n > 0) - (n < 0);
}

char *create_string(size_t len, char c) {
    char *string;

    string = malloc(len + 1);
    if (string == NULL) {
        printf("An error occurred on creating string !\n");
        return NULL;
    }
    memset(string, c, len);
    string[len] = '\0';
    return string;
}

int create_pipe(int *fds) {
    int ok;

    ok = (pipe2(fds, O_NONBLOCK) == 0);
    if (!ok) {
        printf("Open pipe failed ");
        print_skip();
    }
    return (ok);
}

static void close_fds(int *fd) {
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

int display_analyze_results(t_results r, const char *fct) {
    printf("ft_%s = %-10zd, errno %-4i | %s = %-10zd, errno %-4i ", fct, r.ft_result, r.ft_errno, fct, r.libc_result, r.libc_errno);
    return ((r.ft_result != r.libc_result) || (r.ft_errno != r.libc_errno));
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

int open_pairfd(const char *path, int flags, t_pairfd *fd) {
    fd->ft = run_open(path, flags);
    if (fd->ft == -1) {
        return (FAILURE);
    }
    fd->libc = run_open(path, flags);
    if (fd->libc == -1) {
        close(fd->ft);
        fd->ft = -1;
        return (FAILURE);
    }
    return (SUCCESS);
}

void close_pairfd(t_pairfd *fd) {
    if (fd->closed){
        return;
    }
    if (fd->ft >= 0) {
        close(fd->ft);
    }
    if (fd->libc >= 0) {
        close(fd->libc);
    }
    fd->closed = 1;
}

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

int test_strcpy(const char *title, const char *src) {
    size_t len_src;
    char *dest_ft;
    char *dest;
    char *result_ft;
    char *result;
    int fail;

    print_title_test(title);
    fail = 0;
    len_src = strlen(src);
    dest_ft = create_string(len_src, 'X');
    if (dest_ft == NULL) {
        return (fail);
    }
    dest = create_string(len_src, 'X');
    if (dest != NULL) {
        dest_ft[len_src] = 'X';
        dest[len_src] = 'X';
        result_ft = ft_strcpy(dest_ft, src);
        result = strcpy(dest, src);
        fail = (result_ft != dest_ft) || (memcmp(result, result_ft, len_src + 1) != 0);
        print_result(fail);
    }
    free(dest_ft);
    free(dest);
    return (fail);
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

int tests_strlen(t_strings *s) {
    int fail;

    print_title_tests("Tests ft_strlen");
    fail = test_strlen("Empty string", s->empty);
    fail += test_strlen("Short string", s->onechar);
    fail += test_strlen("All characters", s->allchar);
    fail += test_strlen("Long string", s->verylong);
    return (fail);
}

int tests_strcpy(t_strings *s) {
    int fail;

    print_title_tests("Tests ft_strcpy");
    fail = test_strcpy("Empty string", s->empty);
    fail += test_strcpy("Short string", s->onechar);
    fail += test_strcpy("All characters", s->allchar);
    fail += test_strcpy("Long string", s->verylong);
    return (fail);
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

/* return 1 if test failed, 0 if success */
int test_read_core(int ft_fd, int libc_fd, void *ft_buf, void *libc_buf, size_t count, size_t size_buf) {
    int fail;
    t_results r;

    errno = 4242;
    r.ft_result = ft_read(ft_fd, ft_buf, count);
    r.ft_errno = errno;
    errno = 4242;
    r.libc_result = read(libc_fd, libc_buf, count);
    r.libc_errno = errno;

    fail = display_analyze_results(r, "read");
    if (size_buf) {
        fail |= run_memcmp(ft_buf, libc_buf, size_buf);
    }
    print_result(fail);

    return (fail);
}
int fill_pipes(t_pairfd fds) {
    ssize_t len1;
    ssize_t len2;
    char txt[] = "A test in a file for libasm of school 42!";

    len1 = write(fds.ft, txt, sizeof(txt));
    len2 = write(fds.libc, txt, sizeof(txt));
    if ((len1 == -1) || (len2 == -1)) {
        printf("%s", "write in pipe failed ");
        print_skip();
        return (FAILURE);
    }
    return (SUCCESS);
}

int open_tempfile(t_pairfd *pairfd, char *mem_path, int flags) {
    char path[] = "/tmp/libasm_ft_XXXXXX";
    char txt[] = "A test in a file for libasm of school 42!";
    int fd;
    ssize_t len;

    fd = run_mkstemp(path);
    if (fd == -1) {
        return (FAILURE);
    }

    len = write(fd, txt, sizeof(txt));
    close(fd);
    if (len == -1) {
        printf("write in file %s failed ", path);
        print_skip();

        unlink(path);
        return (FAILURE);
    }
    if (open_pairfd(path, flags, pairfd) == FAILURE) {
        unlink(path);
        return (FAILURE);
    }
    strcpy(mem_path, path);
    return (SUCCESS);
}

int open_pipe(t_pairfd *fds_read, t_pairfd *fds_write) {
    int fd_ft[2];
    int fd_libc[2];

    if (!create_pipe(fd_ft)) {
        return (FAILURE);
    }
    if (!create_pipe(fd_libc)) {
        close_fds(fd_ft);
        return (FAILURE);
    }
    fds_read->ft = fd_ft[0];
    fds_read->libc = fd_libc[0];
    fds_write->ft = fd_ft[1];
    fds_write->libc = fd_libc[1];
    return (SUCCESS);
}

int source_open(t_srctype src_type, t_source *s) {
    switch (src_type) {
        case SRC_INVALID:
            s->fd.ft = -1;
            s->fd.libc = -1;
            break;
        case SRC_DIR:
            return (open_pairfd(".", O_RDONLY, &s->fd));
        case SRC_CLOSED:
            if (open_pairfd("/dev/null", O_RDONLY, &s->fd) == FAILURE) {
                return (FAILURE);
            }
            close_pairfd(&s->fd);
            break;
        case SRC_DEV_NULL:
            return (open_pairfd("/dev/null", O_RDONLY, &s->fd));
        case SRC_DEV_ZERO:
            return (open_pairfd("/dev/zero", O_RDONLY, &s->fd));
        case SRC_FILE:
            return (open_tempfile(&s->fd, s->path, O_RDONLY));
        case SRC_FILE_WRONLY:
            return (open_tempfile(&s->fd, s->path, O_WRONLY));
        case SRC_PIPE_DATA:
            if (open_pipe(&s->fd, &s->pipe) == FAILURE) {
                return (FAILURE);
            }
            if (fill_pipes(s->pipe) == FAILURE) {
                close_pairfd(&s->fd);
                close_pairfd(&s->pipe);
                return (FAILURE);
            }
            break;
        case SRC_PIPE_EMPTY:
            return (open_pipe(&s->fd, &s->pipe));
        case SRC_PIPE_WR_CLOSED:
            if (open_pipe(&s->fd, &s->pipe) == FAILURE) {
                return (FAILURE);
            }
            close_pairfd(&s->pipe);
            break;
        default:
            print_failed("%s", "No source type selected ");
            return (FAILURE);

    }
    return (SUCCESS);
}

void source_close(t_source *s) {
    close_pairfd(&s->fd);
    close_pairfd(&s->pipe);
    if (s->path[0] != '\0') {
        unlink(s->path);
    }
}

void source_init(t_source *s) {
    bzero(s, sizeof(*s));
    s->fd.ft = -1;
    s->fd.libc = -1;
    s->pipe.ft = -1;
    s->pipe.libc = -1;
}

/* return 1 if test failed, 0 if success or skip */
int test_read(const char *title, t_srctype src_type, t_buf buf, size_t count) {
    int fail;
    char buf1[LEN_BUF];
    char buf2[LEN_BUF];
    char *ft_buf;
    char *libc_buf;
    t_source s;
    size_t size_buf;

    print_title_test(title);

    ft_buf   = NULL;
    libc_buf = NULL;
    size_buf = 0;
    switch(buf) {
        case BUF_OK:
            ft_buf   = buf1;
            libc_buf = buf2;
            size_buf = LEN_BUF;
            break;
        case BUF_NULL:
            break;
        case BUF_BAD:
            ft_buf   = (void *)1;
            libc_buf = (void *)1;
            break;
        default:
            print_failed("%s", "No buffer selected ");
            return (0);
    }

    source_init(&s);
    if (source_open(src_type, &s) == FAILURE) {
        return (0);
    }
    memset(buf1, 'X', LEN_BUF);
    memset(buf2, 'X', LEN_BUF);

    fail = test_read_core(s.fd.ft, s.fd.libc, ft_buf, libc_buf, count, size_buf);

    source_close(&s);
    return (fail);
}

/* return the number of failed test */
int tests_read(void) {
    int fail;

    print_title_tests("Tests ft_read");
    fail =  test_read("Invalid fd, EBADF",      SRC_INVALID,        BUF_OK,   5);
    fail += test_read("Directory, EISDIR",      SRC_DIR,            BUF_OK,   5);
    fail += test_read("Buffer NULL, EFAULT",    SRC_DEV_ZERO,       BUF_NULL, 5);
    fail += test_read("Buffer bad ptr, EFAULT", SRC_DEV_ZERO,       BUF_BAD,  5);
    fail += test_read("Partial /dev/zero",      SRC_DEV_ZERO,       BUF_OK,   5);
    fail += test_read("Huge count, EFAULT",     SRC_DEV_ZERO,       BUF_NULL, (size_t)1 << 32);
    fail += test_read("EOF",                    SRC_DEV_NULL,       BUF_OK,   5);
    fail += test_read("File closed, EBADF",     SRC_CLOSED,         BUF_OK,   5);
    fail += test_read("Regular file partial",   SRC_FILE,           BUF_OK,   5);
    fail += test_read("Regular file",           SRC_FILE,           BUF_OK,   LEN_BUF);
    fail += test_read("Count 0",                SRC_FILE,           BUF_OK,   0);
    fail += test_read("File wronly, EBADF",     SRC_FILE_WRONLY,    BUF_OK,   5);
    fail += test_read("Regular pipe partial",   SRC_PIPE_DATA,      BUF_OK,   5);
    fail += test_read("Regular pipe",           SRC_PIPE_DATA,      BUF_OK,   LEN_BUF);
    fail += test_read("Empty pipe, EAGAIN",     SRC_PIPE_EMPTY,     BUF_OK,   5);
    fail += test_read("Pipe wr closed, EOF",    SRC_PIPE_WR_CLOSED, BUF_OK,   5);

    return (fail);
}

int test_strdup(const char *title, const char *s, int alloc) {
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

    fail = tests_strlen(strs_test);
    fail += tests_strcpy(strs_test);
    fail += tests_strcmp(strs_test);
    fail += tests_write(strs_test);
    fail += tests_read();
    fail += tests_strdup(strs_test);

    printf("\n%sTests failed :%s %i\n", YELLOW, NC, fail);
    free(strs_test);
    return (fail != 0);
}
