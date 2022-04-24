FROM debian:bullseye AS dev
WORKDIR /app

# don't make this a volume to avoid https://github.com/moby/moby/issues/37965
# VOLUME /app

RUN apt-get update && \
    apt-get install -y build-essential cmake gdb git wget nano

RUN cd /tmp && \
    git clone --branch v1.0.2 https://github.com/redis/hiredis.git && \
    cd hiredis && \
    make && \
    make install && \
    ldconfig

RUN cd /tmp && \
    git clone --branch 1.3.3 https://github.com/sewenew/redis-plus-plus.git && \
    cd redis-plus-plus && \
    mkdir build && \
    cd build && \
    cmake -DREDIS_PLUS_PLUS_CXX_STANDARD=17 .. && \
    make -j4 && \
    make install && \
    ldconfig

#install bcm2835 library (needed for spi)
RUN cd /tmp && \
	wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.58.tar.gz && \
	tar xvfz bcm2835-1.58.tar.gz && \
	cd bcm2835-1.58 && \
	./configure && \
	make && \
	#make check && \
	make install && \
	ldconfig

COPY src src/
COPY Makefile .

RUN make clean && make

# wait forever, but respond to stop signals in dev mode
CMD [ "/bin/sh", "-c", "trap : TERM INT; sleep infinity & wait" ]

FROM debian:bullseye AS prod
WORKDIR /app

COPY --from=dev /app/bin/powerberry-dsp .

CMD [ "./powerberry-dsp" ]
