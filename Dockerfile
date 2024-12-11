FROM gcc:latest

COPY . /usr/src/Netflix
WORKDIR /usr/src/Netflix

RUN mkdir build
WORKDIR /usr/src/Netflix/build

RUN g++ -o netflix ../src/main.cpp ../src/App.cpp ../src/addCommand.cpp ../src/helpCommand.cpp ../src/GETCommand.cpp ../src/ConsoleMenu.cpp ../src/DBFile.cpp ../src/PublicFunctions.cpp

CMD ["./netflix"]