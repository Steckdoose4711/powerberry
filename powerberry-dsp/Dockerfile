FROM gcc:11 AS builder

WORKDIR /app

COPY src src/
COPY Makefile .

RUN make

FROM ubuntu:21.10

WORKDIR /app

COPY --from=builder /app/bin/powerberry-dsp .

CMD ["./powerberry-dsp"]
