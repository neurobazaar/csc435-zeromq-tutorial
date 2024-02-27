## CSC435 ZeroMQ Sockets Tutorial
**Jarvis College of Computing and Digital Media - DePaul University**

### Requirements

To run the C++ programs you will need to have GCC 12.x and CMake 3.22.x installed on your system. You will also need to install the C/C++ ZeroMQ libraries and development files. On Ubuntu 22.04 you can install GCC, CMake and ZeroMQ, and set GCC as default compiler using the following commands:

```
sudo apt install g++-12 gcc-12 cmake
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 110
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 120
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 110
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 120
sudo apt install libzmq3-dev libzmq5
sudo apt install libczmq-dev libczmq4
git submodule init
git submodule update
```

To run the Java programs you will need to have Java 1.7.x and Maven 3.6.x installed on your systems. You will also need to install the JeroMQ (Java ZeroMQ) library and development jar. On Ubuntu 22.04 you can install Java, Maven and JeroMQ using the following commands:

```
sudo apt install openjdk-17-jdk maven libjeromq-java

```

### C++ solution
#### How to build/compile

To build the C++ solution use the following commands:
```
cd app-cpp
mkdir build
cmake -S . -B build
cmake --build build
```

#### How to run application

To run the C++ server (after you build the project) use the following command:
```
./build/server <IP address> <port> <number of workers>
```

To run the C++ client (after you build the project) use the following command:
```
./build/client <IP address> <port>
```

#### Example

Server
```
./build/server 127.0.0.1 12345 2
```

Client 1
```
./build/client 127.0.0.1 12345
Client connected to 127.0.0.1!
2+2=4
2x2=4
```

Client 2
```
./build/client 127.0.0.1 12345
Client connected to 127.0.0.1!
2+2=4
2x2=4
```

### Java solution
#### How to build/compile

To build the Java solution use the following commands:
```
cd app-java
mvn compile
mvn package
```

#### How to run application

To run the Java Server (after you build the project) use the following command:
```
java -cp target/app-java-1.0-SNAPSHOT-jar-with-dependencies.jar csc435.app.Server <IP address> <port> <number of workers>
```

To run the Java Client (after you build the project) use the following command:
```
java -cp target/app-java-1.0-SNAPSHOT-jar-with-dependencies.jar csc435.app.Client <IP address> <port>
```

#### Example

Server
```
java -cp target/app-java-1.0-SNAPSHOT-jar-with-dependencies.jar csc435.app.Server 127.0.0.1 12345 2
```

Client 1
```
java -cp target/app-java-1.0-SNAPSHOT-jar-with-dependencies.jar csc435.app.Client 127.0.0.1 12345
Client connected to 127.0.0.1!
2+2=4
2x2=4
```

Client 2
```
java -cp target/app-java-1.0-SNAPSHOT-jar-with-dependencies.jar csc435.app.Client 127.0.0.1 12345
Client connected to 127.0.0.1!
2+2=4
2x2=4
```
