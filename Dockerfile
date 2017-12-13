FROM ubuntu:14.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update

RUN apt-get install -y aptitude

RUN aptitude update

RUN aptitude safe-upgrade -y

RUN echo y | apt-get install cmake build-essential cpp g++ git git-core libio-stringy-perl liblua5.1 liblua5.1-dev libluabind-dev libmysql++ libperl-dev libperl5i-perl libwtdbomysql-dev libmysqlclient-dev lua5.1 mariadb-client unzip uuid-dev wget zlib-bin zlibc \
      automake autoconf libreadline-dev libncurses-dev libssl-dev libyaml-dev libxslt-dev libffi-dev libtool unixodbc-dev curl

RUN wget http://ftp.us.debian.org/debian/pool/main/libs/libsodium/libsodium18_1.0.11-1~bpo8+1_amd64.deb
RUN wget http://ftp.us.debian.org/debian/pool/main/libs/libsodium/libsodium-dev_1.0.11-1~bpo8+1_amd64.deb

RUN dpkg -i libsodium18_1.0.11-1~bpo8+1_amd64.deb
RUN dpkg -i libsodium-dev_1.0.11-1~bpo8+1_amd64.deb
RUN rm -f libsodium18_1.0.11-1~bpo8+1_amd64.deb

RUN echo 'cd /eqemu && cmake -DEQEMU_BUILD_LUA=ON -G "Unix Makefiles"' > run_cmake.sh

RUN chmod +x run_cmake.sh

RUN git clone https://github.com/asdf-vm/asdf.git ~/.asdf --branch v0.4.0

RUN echo -e '\n. $HOME/.asdf/asdf.sh' >> ~/.bashrc
RUN echo -e '\n. $HOME/.asdf/completions/asdf.bash' >> ~/.bashrc

RUN /root/.asdf/bin/asdf plugin-add ruby https://github.com/asdf-vm/asdf-ruby.git
RUN /root/.asdf/bin/asdf install ruby 2.4.2
RUN /root/.asdf/installs/ruby/2.4.2/bin/gem install rake

RUN /root/.asdf/installs/ruby/2.4.2/bin/gem install bundler

RUN git clone https://github.com/scottdavis/dotfiles.git /root/dotfiles

RUN cd /root/dotfiles && /root/.asdf/installs/ruby/2.4.2/bin/rake dotfiles

RUN ln -s /root/.bashrc 

RUN locale-gen en_US.UTF-8
RUN echo "en_US.UTF-8 UTF-8" > /etc/locale.gen
RUN locale-gen
