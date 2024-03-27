cmd_miscutils/setfattr.o := arm-linux-gnueabihf-gcc -Wp,-MD,miscutils/.setfattr.o.d  -std=gnu99 -Iinclude -Ilibbb  -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.29.0)" -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wno-format-security -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-builtin-printf -Os    -D"KBUILD_STR(s)=#s" -D"KBUILD_BASENAME=KBUILD_STR(setfattr)"  -D"KBUILD_MODNAME=KBUILD_STR(setfattr)" -c -o miscutils/setfattr.o miscutils/setfattr.c

deps_miscutils/setfattr.o := \
  miscutils/setfattr.c \
    $(wildcard include/config/setfattr.h) \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/stdc-predef.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/sys/xattr.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/features.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/sys/cdefs.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/wordsize.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/gnu/stubs.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/gnu/stubs-hard.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/sys/types.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/types.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/typesizes.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/time.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.9.4/include/stddef.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/endian.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/endian.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/byteswap.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/byteswap-16.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/sys/select.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/select.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/sigset.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/time.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/sys/sysmacros.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/pthreadtypes.h \
  include/libbb.h \
    $(wildcard include/config/feature/shadowpasswds.h) \
    $(wildcard include/config/use/bb/shadow.h) \
    $(wildcard include/config/selinux.h) \
    $(wildcard include/config/feature/utmp.h) \
    $(wildcard include/config/locale/support.h) \
    $(wildcard include/config/use/bb/pwd/grp.h) \
    $(wildcard include/config/lfs.h) \
    $(wildcard include/config/feature/buffers/go/on/stack.h) \
    $(wildcard include/config/feature/buffers/go/in/bss.h) \
    $(wildcard include/config/feature/verbose.h) \
    $(wildcard include/config/feature/etc/services.h) \
    $(wildcard include/config/feature/ipv6.h) \
    $(wildcard include/config/feature/seamless/xz.h) \
    $(wildcard include/config/feature/seamless/lzma.h) \
    $(wildcard include/config/feature/seamless/bz2.h) \
    $(wildcard include/config/feature/seamless/gz.h) \
    $(wildcard include/config/feature/seamless/z.h) \
    $(wildcard include/config/feature/check/names.h) \
    $(wildcard include/config/feature/prefer/applets.h) \
    $(wildcard include/config/long/opts.h) \
    $(wildcard include/config/feature/pidfile.h) \
    $(wildcard include/config/feature/syslog.h) \
    $(wildcard include/config/feature/individual.h) \
    $(wildcard include/config/echo.h) \
    $(wildcard include/config/printf.h) \
    $(wildcard include/config/test.h) \
    $(wildcard include/config/test1.h) \
    $(wildcard include/config/test2.h) \
    $(wildcard include/config/kill.h) \
    $(wildcard include/config/killall.h) \
    $(wildcard include/config/killall5.h) \
    $(wildcard include/config/chown.h) \
    $(wildcard include/config/ls.h) \
    $(wildcard include/config/xxx.h) \
    $(wildcard include/config/route.h) \
    $(wildcard include/config/feature/hwib.h) \
    $(wildcard include/config/desktop.h) \
    $(wildcard include/config/feature/crond/d.h) \
    $(wildcard include/config/feature/setpriv/capabilities.h) \
    $(wildcard include/config/run/init.h) \
    $(wildcard include/config/feature/securetty.h) \
    $(wildcard include/config/pam.h) \
    $(wildcard include/config/use/bb/crypt.h) \
    $(wildcard include/config/feature/adduser/to/group.h) \
    $(wildcard include/config/feature/del/user/from/group.h) \
    $(wildcard include/config/ioctl/hex2str/error.h) \
    $(wildcard include/config/feature/editing.h) \
    $(wildcard include/config/feature/editing/history.h) \
    $(wildcard include/config/feature/editing/savehistory.h) \
    $(wildcard include/config/feature/tab/completion.h) \
    $(wildcard include/config/feature/username/completion.h) \
    $(wildcard include/config/feature/editing/vi.h) \
    $(wildcard include/config/feature/editing/save/on/exit.h) \
    $(wildcard include/config/pmap.h) \
    $(wildcard include/config/feature/show/threads.h) \
    $(wildcard include/config/feature/ps/additional/columns.h) \
    $(wildcard include/config/feature/topmem.h) \
    $(wildcard include/config/feature/top/smp/process.h) \
    $(wildcard include/config/pgrep.h) \
    $(wildcard include/config/pkill.h) \
    $(wildcard include/config/pidof.h) \
    $(wildcard include/config/sestatus.h) \
    $(wildcard include/config/unicode/support.h) \
    $(wildcard include/config/feature/mtab/support.h) \
    $(wildcard include/config/feature/clean/up.h) \
    $(wildcard include/config/feature/devfs.h) \
  include/platform.h \
    $(wildcard include/config/werror.h) \
    $(wildcard include/config/big/endian.h) \
    $(wildcard include/config/little/endian.h) \
    $(wildcard include/config/nommu.h) \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.9.4/include-fixed/limits.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.9.4/include-fixed/syslimits.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/limits.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/posix1_lim.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/local_lim.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/linux/limits.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/posix2_lim.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/xopen_lim.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/stdio_lim.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/byteswap.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.9.4/include/stdint.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/stdint.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/wchar.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.9.4/include/stdbool.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/unistd.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/posix_opt.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/environments.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/confname.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/getopt.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/ctype.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/xlocale.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/dirent.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/dirent.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/errno.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/errno.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/linux/errno.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/asm/errno.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/asm-generic/errno.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/asm-generic/errno-base.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/fcntl.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/fcntl.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/fcntl-linux.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/uio.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/stat.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/inttypes.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/netdb.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/netinet/in.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/sys/socket.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/sys/uio.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/socket.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/socket_type.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/sockaddr.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/asm/socket.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/asm-generic/socket.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/asm/sockios.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/asm-generic/sockios.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/in.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/rpc/netdb.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/siginfo.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/netdb.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/setjmp.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/setjmp.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/signal.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/signum.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/sigaction.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/sigcontext.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/asm/sigcontext.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/sigstack.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/sys/ucontext.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/sigthread.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/paths.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/stdio.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/libio.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/_G_config.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/wchar.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.9.4/include/stdarg.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/sys_errlist.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/stdlib.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/waitflags.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/waitstatus.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/alloca.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/stdlib-float.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/string.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/libgen.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/poll.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/sys/poll.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/poll.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/sys/ioctl.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/ioctls.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/asm/ioctls.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/asm-generic/ioctls.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/linux/ioctl.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/asm/ioctl.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/asm-generic/ioctl.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/ioctl-types.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/sys/ttydefaults.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/sys/mman.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/mman.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/mman-linux.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/sys/resource.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/resource.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/sys/stat.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/sys/time.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/sys/wait.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/termios.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/termios.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/timex.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/sys/param.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/param.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/linux/param.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/asm/param.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/asm-generic/param.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/pwd.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/grp.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/mntent.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/sys/statfs.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/statfs.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/utmp.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/utmp.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/utmpx.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/bits/utmpx.h \
  /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/arpa/inet.h \
  include/pwd_.h \
  include/grp_.h \
  include/shadow_.h \
  include/xatonum.h \

miscutils/setfattr.o: $(deps_miscutils/setfattr.o)

$(deps_miscutils/setfattr.o):
