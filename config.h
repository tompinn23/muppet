#ifndef OCONFIGURE_CONFIG_H
#define OCONFIGURE_CONFIG_H

#define HAVE_ARC4RANDOM 1
#define HAVE_BLOWFISH 1
#define HAVE_B64_NTOP 1
#define HAVE_CAPSICUM 0
#define HAVE_CRYPT 1
#define HAVE_CRYPT_NEWHASH 1
#define HAVE_ENDIAN_H 1
#define HAVE_ERR 1
#define HAVE_EXPLICIT_BZERO 1
#define HAVE_FTS 1
#define HAVE_GETEXECNAME 0
#define HAVE_GETPROGNAME 1
#define HAVE_INFTIM 1
#define HAVE_LANDLOCK 0
#define HAVE_MD5 1
#define HAVE_MEMMEM 1
#define HAVE_MEMRCHR 1
#define HAVE_MEMSET_S 0
#define HAVE_MKFIFOAT 1
#define HAVE_MKNODAT 1
#define HAVE_OSBYTEORDER_H 0
#define HAVE_PASSWORD_LEN 1
#define HAVE_PATH_MAX 1
#define HAVE_PLEDGE 1
#define HAVE_PROGRAM_INVOCATION_SHORT_NAME 0
#define HAVE_READPASSPHRASE 1
#define HAVE_REALLOCARRAY 1
#define HAVE_RECALLOCARRAY 1
#define HAVE_SANDBOX_INIT 0
#define HAVE_SCAN_SCALED 1
#define HAVE_SECCOMP_HEADER 0
#define HAVE_SETRESGID 1
#define HAVE_SETRESUID 1
#define HAVE_SHA2 1
#define HAVE_SHA2_H 1
#define HAVE_SOCK_NONBLOCK 1
#define HAVE_STRLCAT 1
#define HAVE_STRLCPY 1
#define HAVE_STRNDUP 1
#define HAVE_STRNLEN 1
#define HAVE_STRTONUM 1
#define HAVE_SYS_BYTEORDER_H 0
#define HAVE_SYS_ENDIAN_H 1
#define HAVE_SYS_MKDEV_H 0
#define HAVE_SYS_QUEUE 1
#define HAVE_SYS_SYSMACROS_H 0
#define HAVE_SYS_TREE 1
#define HAVE_SYSTRACE 0
#define HAVE_UNVEIL 1
#define HAVE_TERMIOS 1
#define HAVE_TIMINGSAFE_BCMP 1
#define HAVE_WAIT_ANY 1
#define HAVE___PROGNAME 1

#ifdef __cplusplus
# error "Do not use C++: this is a C application."
#endif
#if !defined(__GNUC__) || (__GNUC__ < 4)
# define __attribute__(x)
#endif
#if defined(__linux__) || defined(__MINT__) || defined(__wasi__)
# define _GNU_SOURCE /* memmem, memrchr, setresuid... */
# define _DEFAULT_SOURCE /* le32toh, crypt, ... */
#endif
#if defined(__NetBSD__)
# define _OPENBSD_SOURCE /* reallocarray, etc. */
#endif
#if defined(__sun)
# ifndef _XOPEN_SOURCE /* SunOS already defines */
#  define _XOPEN_SOURCE /* XPGx */
# endif
# define _XOPEN_SOURCE_EXTENDED 1 /* XPG4v2 */
# ifndef __EXTENSIONS__ /* SunOS already defines */
#  define __EXTENSIONS__ /* reallocarray, etc. */
# endif
#endif
#if !defined(__BEGIN_DECLS)
# define __BEGIN_DECLS
#endif
#if !defined(__END_DECLS)
# define __END_DECLS
#endif
/*
 * Handle the various major()/minor() header files.
 * Use sys/mkdev.h before sys/sysmacros.h because SunOS
 * has both, where only the former works properly.
 */
#if HAVE_SYS_MKDEV_H
# define COMPAT_MAJOR_MINOR_H <sys/mkdev.h>
#elif HAVE_SYS_SYSMACROS_H
# define COMPAT_MAJOR_MINOR_H <sys/sysmacros.h>
#else
# define COMPAT_MAJOR_MINOR_H <sys/types.h>
#endif
/*
 * Make it easier to include endian.h forms.
 */
#if HAVE_ENDIAN_H
# define COMPAT_ENDIAN_H <endian.h>
#elif HAVE_SYS_ENDIAN_H
# define COMPAT_ENDIAN_H <sys/endian.h>
#elif HAVE_OSBYTEORDER_H
# define COMPAT_ENDIAN_H <libkern/OSByteOrder.h>
#elif HAVE_SYS_BYTEORDER_H
# define COMPAT_ENDIAN_H <sys/byteorder.h>
#else
# warning No suitable endian.h could be found.
# warning Please e-mail the maintainers with your OS.
# define COMPAT_ENDIAN_H <endian.h>
#endif
#define HAVE_SECCOMP_FILTER 0
#endif /*!OCONFIGURE_CONFIG_H*/
