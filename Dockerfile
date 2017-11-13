FROM ubuntu:14.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update

RUN apt-get install -y aptitude

RUN aptitude update

RUN aptitude safe-upgrade -y

RUN echo y | apt-get install build-essential cpp g++ git git-core libio-stringy-perl liblua5.1 liblua5.1-dev libluabind-dev libmysql++ libperl-dev libperl5i-perl libwtdbomysql-dev libmysqlclient-dev lua5.1 mariadb-client unzip uuid-dev wget zlib-bin zlibc

RUN wget http://ftp.us.debian.org/debian/pool/main/libs/libsodium/libsodium18_1.0.11-1~bpo8+1_amd64.deb
RUN wget http://ftp.us.debian.org/debian/pool/main/libs/libsodium/libsodium-dev_1.0.11-1~bpo8+1_amd64.deb

RUN dpkg -i libsodium18_1.0.11-1~bpo8+1_amd64.deb
RUN dpkg -i libsodium-dev_1.0.11-1~bpo8+1_amd64.deb
RUN rm -f libsodium18_1.0.11-1~bpo8+1_amd64.deb
