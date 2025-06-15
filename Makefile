include Makefile.configure

lib_pkg != pkg-config --libs kcgi
cflag_pkg != pkg-config --cflags kcgi

LDADD += $(LDADD_STATIC) $(lib_pkg)
CFLAGS += $(cflag_pkg)

objs := muppet.o compats.o

muppet: $(objs)
	$(CC) -o $@ $(objs) $(LDFLAGS) $(LDADD)

.c.o:
	cc $(CFLAGS) -c -o $@ $<

distclean: clean
	rm Makefile.configure
	rm config.h

clean:
	rm $(objs)
	rm muppet

