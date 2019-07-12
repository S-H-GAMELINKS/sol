FROM ubuntu:latest
WORKDIR ~/
EXPOSE 8080

COPY . /sol
WORKDIR /sol

RUN apt-get update && apt-get install nodejs gcc g++ git curl gnupg libsqlite3-dev -y
RUN curl -sS https://dl.yarnpkg.com/debian/pubkey.gpg | apt-key add -
RUN echo "deb https://dl.yarnpkg.com/debian/ stable main" | tee /etc/apt/sources.list.d/yarn.list
RUN apt-get update && apt-get install yarn -y

RUN git clone https://github.com/S-H-GAMELINKS/sol.git ~/.sol && \
    cd ~/.sol/cpp-httplib && git submodule init && git submodule update && \
    cd ../sqlite_orm && git submodule init && git submodule update

RUN echo 'export PATH="$HOME/.sol/bin:$PATH"' >> ~/.bashrc && exec bash

RUN chmod 777 ~/.sol/bin/sol 
