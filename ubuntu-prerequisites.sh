#!/bin/sh

apt update -yq

apt-get -yq --no-install-suggests --no-install-recommends install \
	build-essential \
	cmake \
	curl \
	flex \
	gcc \
	lcov \
	libboost-dev \
	libboost-test-dev \
	libssl-dev \
	python3 \
	python3-dev \
	swig \
	valgrind \
	wget

