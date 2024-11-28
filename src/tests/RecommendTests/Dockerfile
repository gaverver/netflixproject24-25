FROM gcc:latest

RUN apt-get update && apt-get install -y cmake

# Copy the entire project into the container
COPY ../../.. /usr/src/Netflix

# Set the working directory
WORKDIR /usr/src/Netflix/src/tests/testsForRecommend

# Create a build directory
RUN mkdir build

# Switch to the build directory
WORKDIR /usr/src/Netflix/src/tests/testsForRecommend/build

# Run CMake and build the project
RUN cmake .. && make

# Run the tests
CMD ["./runRecommendTests"]
