ARG GID=1000
ARG UID=1000

ARG ALPINE_TAG=3.21.2
ARG ALPINE_HASH=sha256:56fa17d2a7e7f168a043a2712e63aed1f8543aeafdcee47c58dcffe38ed51099

ARG DEBIAN_TAG=12.9-slim
ARG DEBIAN_HASH=sha256:f70dc8d6a8b6a06824c92471a1a258030836b26b043881358b967bf73de7c5ab

ARG ASCIIART_VERSION=0.0.10-2
ARG BOOST_TAG=1.81
ARG BOOST_FULL_TAG=1.81.0
ARG BOOST_VERSION=1.81.0-5+deb12u1
ARG CLANG_VERSION=1:19.1.4-1~deb12u1
ARG CMAKE_VERSION=3.25.1-1
ARG CONTEXT_EZA_VERSION=0.20.12-r0
ARG LLVM_SUFFIX=-19
ARG MAGICK_VERSION=8:6.9.11.60+dfsg-1.6+deb12u2
ARG NINJA_VERSION=1.11.1-2~deb12u1


###
### check-build-context
###
FROM alpine:${ALPINE_TAG}@${ALPINE_HASH} AS check-build-context

ARG CONTEXT_EZA_VERSION

RUN apk add --no-cache eza=${CONTEXT_EZA_VERSION}
WORKDIR /opt/context
COPY . .

ENTRYPOINT ["eza"]
CMD ["--all", "--tree", "--icons"]


###
### mwfractal-base
###
FROM debian:${DEBIAN_TAG}@${DEBIAN_HASH} AS mwfractal-base

ARG BOOST_FULL_TAG
ARG BOOST_VERSION
ARG MAGICK_VERSION

RUN DEBIAN_FRONTEND=noninteractive && apt-get update && apt-get install -y --no-install-recommends \
    libboost-program-options${BOOST_FULL_TAG}=${BOOST_VERSION} \
    libmagick++-6.q16-8=${MAGICK_VERSION} \
 && rm -rf /var/lib/apt/lists/*


###
### mwfractal-builder
###
FROM mwfractal-base AS mwfractal-builder

ARG BOOST_TAG
ARG BOOST_VERSION
ARG LLVM_SUFFIX
ARG CLANG_VERSION
ARG CMAKE_VERSION
ARG MAGICK_VERSION
ARG NINJA_VERSION

RUN DEBIAN_FRONTEND=noninteractive && apt-get update && apt-get install -y --no-install-recommends \
      clang${LLVM_SUFFIX}=${CLANG_VERSION} \
      clang-tidy${LLVM_SUFFIX}=${CLANG_VERSION} \
      cmake=${CMAKE_VERSION} \
      libboost${BOOST_TAG}-dev=${BOOST_VERSION} \
      libboost-program-options${BOOST_TAG}-dev=${BOOST_VERSION} \
      libmagick++-dev=${MAGICK_VERSION} \
      ninja-build=${NINJA_VERSION} \
 && rm -rf /var/lib/apt/lists/*

ENV PATH="$PATH:/usr/lib/llvm${LLVM_SUFFIX}/bin"

WORKDIR /opt/mwfractal

COPY . .

RUN mkdir build  \
 && cd build  \
 && cmake .. -G Ninja \
 && ninja \
 && clang-tidy -p compile_commands.json ../src/main.cpp


###
### mwfractal
###
FROM mwfractal-base AS mwfractal

ARG GID
ARG UID

RUN groupadd -r mw --g ${GID} \
 && useradd -l -r mw -g mw -m -u ${UID}

COPY --from=mwfractal-builder /opt/mwfractal/build/mwfractal /usr/local/bin/mwfractal

WORKDIR /output

RUN chown mw:mw /output

USER mw

ENTRYPOINT ["/usr/local/bin/mwfractal"]


###
### mwfractal-asciiart
###
FROM mwfractal AS mwfractal-asciiart

ARG ASCIIART_VERSION

USER root

RUN DEBIAN_FRONTEND=noninteractive && apt-get update && apt-get install -y --no-install-recommends \
      asciiart=${ASCIIART_VERSION} \
 && rm -rf /var/lib/apt/lists/*

COPY ./asciiart.sh .

USER mw

ENTRYPOINT ["./asciiart.sh"]
