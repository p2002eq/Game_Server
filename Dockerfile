FROM ubuntu:xenial

ENV DEBIAN_FRONTEND=noninteractive
ENV PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:${PATH}"
RUN apt-get update

RUN apt-get update -qq && apt-get install -y -qq gcc-4.8 g++-4.8 libmysqlclient-dev libperl-dev libboost-dev liblua5.1-0-dev zlib1g-dev uuid-dev libssl-dev build-essential make cmake git-core unzip curl wget nano fish software-properties-common python-software-properties python-dev python-pip python3-dev python3-pip fish golang direnv locales

RUN add-apt-repository ppa:neovim-ppa/stable && apt-get update && apt-get install neovim -y
RUN pip2 install --upgrade neovim
RUN pip3 install --upgrade neovim

RUN wget http://ftp.us.debian.org/debian/pool/main/libs/libsodium/libsodium18_1.0.11-1~bpo8+1_amd64.deb
RUN wget http://ftp.us.debian.org/debian/pool/main/libs/libsodium/libsodium-dev_1.0.11-1~bpo8+1_amd64.deb

RUN dpkg -i libsodium18_1.0.11-1~bpo8+1_amd64.deb
RUN dpkg -i libsodium-dev_1.0.11-1~bpo8+1_amd64.deb
RUN rm -f libsodium18_1.0.11-1~bpo8+1_amd64.deb

RUN mkdir temp && cd temp && curl -OL https://launchpad.net/~maarten-fonville/+archive/ubuntu/protobuf/+files/libprotobuf-dev_3.1.0-0ubuntu1~maarten0_amd64.deb && curl -OL https://launchpad.net/~maarten-fonville/+archive/ubuntu/protobuf/+files/protobuf-compiler_3.1.0-0ubuntu1~maarten0_amd64.deb &&curl -OL https://launchpad.net/~maarten-fonville/+archive/ubuntu/protobuf/+files/libprotobuf-lite10_3.1.0-0ubuntu1~maarten0_amd64.deb && curl -OL https://launchpad.net/~maarten-fonville/+archive/ubuntu/protobuf/+files/libprotoc10_3.1.0-0ubuntu1~maarten0_amd64.deb && curl -OL https://launchpad.net/~maarten-fonville/+archive/ubuntu/protobuf/+files/libprotoc-dev_3.1.0-0ubuntu1~maarten0_amd64.deb && curl -OL https://launchpad.net/~maarten-fonville/+archive/ubuntu/protobuf/+files/libprotobuf10_3.1.0-0ubuntu1~maarten0_amd64.deb && DEBIAN_FRONTEND=noninteractive dpkg -R --install . && cd .. && rm -rf temp

RUN echo 'cd /eqemu && cmake -DEQEMU_BUILD_LUA=ON -G "Unix Makefiles"' > run_cmake.sh

RUN chmod +x run_cmake.sh

RUN git clone https://github.com/asdf-vm/asdf.git ~/.asdf --branch v0.4.0

RUN echo -e '\n. $HOME/.asdf/asdf.sh' >> ~/.bashrc
RUN echo -e '\n. $HOME/.asdf/completions/asdf.bash' >> ~/.bashrc

RUN /root/.asdf/bin/asdf plugin-add ruby https://github.com/asdf-vm/asdf-ruby.git
RUN /root/.asdf/bin/asdf install ruby 2.4.2
RUN /root/.asdf/installs/ruby/2.4.2/bin/gem install rake

RUN /root/.asdf/installs/ruby/2.4.2/bin/gem install bundler

RUN git clone https://github.com/fryguy503/dotfiles.git /root/dotfiles

RUN cd /root/dotfiles && /root/.asdf/installs/ruby/2.4.2/bin/rake

RUN ln -s /root/.bashrc

RUN locale-gen en_US.UTF-8
RUN echo "en_US.UTF-8 UTF-8" > /etc/locale.gen
RUN locale-gen
