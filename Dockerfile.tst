FROM gcc:latest

RUN apt-get update && apt-get install -y cmake

COPY . /usr/src/NetflixTests

WORKDIR /usr/src/NetflixTests

RUN mkdir build
WORKDIR /usr/src/NetflixTests/build

RUN cmake .. && make

CMD ["./runTests"]