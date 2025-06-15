#include "config.h"

#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <stddef.h>
#include <unistd.h>
#include <kcgi.h>
#include <limits.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <syslog.h>

#if HAVE_ERR
# include <err.h>
#endif

#define REPO_ROOT "/maven"

static void format_filesz(size_t len, char *out, size_t outsz) {
    const char *units[] = { "b", "kb", "mb", "gb"};
    int idx = 0;
    double sz = len;

    while(sz >= 1024 && idx < 3) {
        sz /= 1024;
        idx++;
    }
    snprintf(out, outsz, "%.2f %s", sz, units[idx]);
}

static int write_file(const char *path, char *buf, size_t len) {
    char fullpath[PATH_MAX];
    char patbuf[PATH_MAX];
    ssize_t rc;
    if((rc = snprintf(fullpath, sizeof(fullpath), "%s/%s", REPO_ROOT, *path == '/' ? path + 1 : path)) == PATH_MAX) {
        return 0;
    }

    strlcpy(patbuf, fullpath, sizeof(patbuf));
    for(char *p = patbuf; *p; ++p) {
        if(*p == '/') {
            *p = '\0';
            mkdir(patbuf, 0755);
            *p = '/';
        }
    }

    int fd = open(fullpath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd < 0) {
        syslog(LOG_ERR, "failed to open file %s (%s)", fullpath, strerror(errno));
        return 0;
    }

    size_t total = 0;
    size_t left = len;

    ssize_t ret;
    for(;;) {
        ret = write(fd, buf, left);
        if(ret < 0) {
            if(errno == EINTR) {
                continue;
            }
            close(fd);
            syslog(LOG_ERR, "failed to write file %s (%s)", fullpath, strerror(errno));
            return 0;
        }
        total += ret;
        if(total == len) {
            break;
        }
        buf += ret;
        left -= ret;
    }
    char fsz[32];
    format_filesz(total, fsz, sizeof(fsz));
    syslog(LOG_INFO, "wrote file %s (%s)", fullpath, fsz);
    return total == len;
}

static void respond(struct kreq *req, int status, const char *message) {
    khttp_head(req, kresps[KRESP_STATUS], "%s", khttps[status]);
    khttp_head(req, kresps[KRESP_CONTENT_TYPE], "%s", kmimetypes[KMIME_TEXT_PLAIN]);
    khttp_body(req);
    khttp_puts(req, message);
    khttp_free(req);
}


int main(void) {
    struct kreq req;

    struct kfcgi *fcgi;

    openlog("muppet", LOG_NDELAY | LOG_PID, LOG_DAEMON);
#if HAVE_UNVEIL
    if(unveil(REPO_ROOT, "rwc") == -1) {
        err(1, "unveil");
    }

    if(unveil(NULL, NULL) == -1) {
        err(1, "unveil lock");
    }
#endif
#if HAVE_PLEDGE
    if(pledge("unix sendfd recvfd proc stdio rpath wpath cpath", NULL) == -1) {
        err(1, "pledge 1");
    }
#endif

    if(khttp_fcgi_init(&fcgi, NULL, 0, NULL, 0, 0) != KCGI_OK) {
        err(1, "fcgi_init");
    }
#if HAVE_PLEDGE
    if(pledge("stdio recvfd rpath wpath cpath", NULL) == -1) {
        err(1, "pledge 2");
    }
#endif


    for(;;) {
        if(khttp_fcgi_parse(fcgi, &req) != KCGI_OK) {
            break;
        }

        if((!(req.method == KMETHOD_POST || req.method == KMETHOD_PUT))) {
            respond(&req, KHTTP_405, "Method not allowed\n");
            continue;
        }
        syslog(LOG_INFO, "Recieved file: %s", req.fullpath);

        if(write_file(req.fullpath, req.fields[0].val, req.fields[0].valsz)) {
            respond(&req, KHTTP_201, "Created\n");
        } else {
            respond(&req, KHTTP_500, "Internal server error\n");
        }
    }

    khttp_fcgi_free(fcgi);
    return 0;
}
