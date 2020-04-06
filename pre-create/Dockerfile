FROM ubuntu

RUN apt-get update
RUN apt-get install -y g++
RUN apt-get install -y build-essential

COPY . /opt/source-code

WORKDIR /opt/source-code
RUN make clean
RUN make

ENTRYPOINT ["./server"]
CMD ["512"]