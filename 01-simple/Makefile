QUIET?=@

TARGETNAMES = \
	simple-get \
	simple-post \
	https-get
	
-include ../project.mak

$(BLDDIR)/simple-get: simple-get.o
	$(CC) -o $@ $^ -lcurl

$(BLDDIR)/simple-post: simple-post.o
	$(CC) -o $@ $^ -lcurl

$(BLDDIR)/https-get: https-get.o
	$(CC) -o $@ $^ -lcurl
