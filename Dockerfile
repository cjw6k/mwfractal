ARG GID=1000
ARG UID=1000

ARG ALPINE_TAG=3.21.2
ARG ALPINE_HASH=sha256:56fa17d2a7e7f168a043a2712e63aed1f8543aeafdcee47c58dcffe38ed51099

ARG DEBIAN_TAG=12.9
ARG DEBIAN_HASH=sha256:321341744acb788e251ebd374aecc1a42d60ce65da7bd4ee9207ff6be6686a62

ARG ASCIIART_VERSION=0.0.10-2
ARG AUTOCONF_VERSION=2.71-3
ARG AUTOMAKE_VERSION=1:1.16.5-1.3
ARG BOOST_TAG=1.81
ARG BOOST_VERSION=1.81.0-5+deb12u1
ARG CONTEXT_EZA_VERSION=0.20.12-r0
ARG GPP_VERSION=4:12.2.0-3
ARG MAGICK_VERSION=8:6.9.11.60+dfsg-1.6+deb12u2
ARG MAKE_VERSION=4.3-4.1
ARG PKG_CONFIG_VERSION=1.8.1-1


#
# check-build-context
#
FROM alpine:${ALPINE_TAG}@${ALPINE_HASH} AS check-build-context

ARG CONTEXT_EZA_VERSION

RUN apk add --no-cache eza=${CONTEXT_EZA_VERSION}
WORKDIR /opt/context
COPY . .

ENTRYPOINT ["eza"]
CMD ["--all", "--tree", "--icons"]


###
### mwfractal
###
FROM debian:${DEBIAN_TAG}@${DEBIAN_HASH} AS mwfractal

ARG GID
ARG UID

ARG AUTOCONF_VERSION
ARG AUTOMAKE_VERSION
ARG BOOST_TAG
ARG BOOST_VERSION
ARG GPP_VERSION
ARG MAGICK_VERSION
ARG MAKE_VERSION
ARG PKG_CONFIG_VERSION

RUN apt-get update && apt-get install -y --no-install-recommends \
      autoconf=${AUTOCONF_VERSION} \
      automake=${AUTOMAKE_VERSION} \
      g++=${GPP_VERSION} \
      libboost${BOOST_TAG}-dev=${BOOST_VERSION} \
      libboost-program-options${BOOST_TAG}-dev=${BOOST_VERSION} \
      libmagick++-dev=${MAGICK_VERSION} \
      make=${MAKE_VERSION} \
      pkg-config=${PKG_CONFIG_VERSION} \
 && rm -rf /var/lib/apt/lists/*

RUN groupadd -r mw --g ${GID} \
 && useradd -r mw -g mw -m -u ${UID}

WORKDIR /opt/mwfractal

COPY . .

RUN chown -R mw:mw /opt/mwfractal

USER mw

RUN ./bootstrap && ./configure && make "-j$(nproc)"

ENTRYPOINT ["/opt/mwfractal/mwfractal"]

###
### asciiart
###
FROM mwfractal AS asciiart

ARG ASCIIART_VERSION

USER root

RUN apt-get update && apt-get install -y --no-install-recommends \
      asciiart=${ASCIIART_VERSION} \
 && rm -rf /var/lib/apt/lists/*

USER mw

ENTRYPOINT ["./demo.sh"]
