FROM gcc:latest

RUN apt-get update && apt-get install -y cmake

COPY . /usr/src/myapp

WORKDIR /usr/src/myapp

RUN mkdir build && mkdir data
WORKDIR /usr/src/myapp/build

RUN cmake .. && make

CMD ["./runRecommendTests"]