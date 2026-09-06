# -------------
# builder stage
# -------------

FROM golang:1.26-bookworm AS builder

# g++ and make for core library build
RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /project

# cache Go modules layer
COPY go.mod go.sum ./
RUN go mod download

# copy source code
COPY Makefile ./
COPY core ./core
COPY cmd ./cmd
COPY internal ./internal
COPY data ./data

# build static C++ core library and Go executables
RUN make all

# -------------
# runtime stage
# -------------

FROM debian:bookworm-slim AS runtime

# C++ runtime lib and CA certificates for HTTPS crawling
RUN apt-get update && apt-get install -y --no-install-recommends \
    libstdc++6 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /project

# binaries from builder stage
COPY --from=builder /project/bin/septcrawler /project/septcrawler
COPY --from=builder /project/bin/storage /project/storage
COPY --from=builder /project/bin/septcrawler-cli /project/septcrawler-cli

# initial seed data
COPY --from=builder /project/data /project/data

# persistent storage volume
VOLUME ["/project/data"]

EXPOSE 8080

# default command
CMD ["/project/septcrawler"]
