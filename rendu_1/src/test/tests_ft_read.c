/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_ft_read.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svogrig <svogrig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 13:00:06 by svogrig           #+#    #+#             */
/*   Updated: 2026/11/05 13:37:20 by svogrig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

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
