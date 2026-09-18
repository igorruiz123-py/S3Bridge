FROM ubuntu:24.04

RUN apt-get update

RUN apt-get install -y gcc make

WORKDIR /usr/local/s3bridge

COPY Makefile /usr/local/s3bridge//

COPY include/* /usr/local/s3bridge//include/

COPY src/* /usr/local/s3bridge/src/

RUN make setup && make compile

EXPOSE 8080

CMD ["./bin/server"]