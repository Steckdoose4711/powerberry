FROM gcc:11.2 AS dev
WORKDIR /app

# don't make this a volume to avoid https://github.com/moby/moby/issues/37965
# VOLUME /app

RUN apt-get update && \
    apt-get install -y gdb

COPY src src/
COPY Makefile .

RUN make

# wait forever, but respond to stop signals in dev mode
CMD [ "/bin/sh", "-c", "trap : TERM INT; sleep infinity & wait" ]

FROM debian:stable-slim AS prod
WORKDIR /app

COPY --from=dev /app/bin/powerberry-dsp .

CMD [ "./powerberry-dsp" ]
