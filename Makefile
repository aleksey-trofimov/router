CC=g++
FLAGS= -s -Os -static
#-static -L/BlueCat/cdt/lib
OBJS= obj/global.o obj/Socket.o obj/ServerSocket.o obj/CmdPacket.o \
      obj/DataPacket.o obj/Message.o obj/MessageQueue.o \
      obj/CacheData.o obj/Semaphore.o obj/Driver.o obj/Thread.o \
      obj/MPSUSerialDriver.o obj/EthernetTCPClient.o obj/EthernetTCPServer.o\
      obj/SerialThread.o obj/Protocol.o obj/ProtocolMPSU.o obj/ProtocolMOUSE.o\
      obj/ProtocolCONFIG.o obj/MPSURecord.o obj/SerialDriver.o obj/MODBUS_TCP.o\
      obj/Address.o obj/Router.o obj/controller.o
      
router : $(OBJS)
	$(CC) $(OBJS) -o bin/router -pthread $(FLAGS)
clean :
	rm obj/*.o bin/router bin/test
pack :
	tar -czf router.tar.gz *
obj/Driver.o : drivers/Driver.cpp drivers/Driver.h definitions.h
	$(CC) -c $(FLAGS) drivers/Driver.cpp
	mv *.o obj/
obj/SerialDriver.o : drivers/SerialDriver.cpp drivers/SerialDriver.h definitions.h
	$(CC) -c $(FLAGS) drivers/SerialDriver.cpp
	mv *.o obj/
obj/MPSUSerialDriver.o : drivers/MPSUSerialDriver.cpp drivers/MPSUSerialDriver.h definitions.h
	$(CC) -c $(FLAGS) drivers/MPSUSerialDriver.cpp
	mv *.o obj/
obj/Thread.o : threads/Thread.cpp threads/Thread.h definitions.h
	$(CC) -c $(FLAGS) threads/Thread.cpp
	mv *.o obj/
obj/EthernetTCPServer.o : threads/EthernetTCPServer.cpp threads/EthernetTCPServer.h definitions.h
	$(CC) -c $(FLAGS) threads/EthernetTCPServer.cpp
	mv *.o obj/
obj/SerialThread.o : threads/SerialThread.cpp threads/SerialThread.h definitions.h
	$(CC) -c $(FLAGS) threads/SerialThread.cpp
	mv *.o obj/
obj/EthernetTCPClient.o : drivers/EthernetTCPClient.cpp drivers/EthernetTCPClient.h definitions.h
	$(CC) -c $(FLAGS) drivers/EthernetTCPClient.cpp
	mv *.o obj/
obj/Protocol.o : protocols/Protocol.cpp protocols/Protocol.h definitions.h
	$(CC) -c $(FLAGS) protocols/Protocol.cpp
	mv *.o obj/
obj/ProtocolFactory.o : protocols/ProtocolFactory.cpp protocols/ProtocolFactory.h definitions.h
	$(CC) -c $(FLAGS) protocols/ProtocolFactory.cpp
	mv *.o obj/
obj/ProtocolMPSU.o : protocols/ProtocolMPSU.cpp protocols/ProtocolMPSU.h protocols/MPSURecord.h protocols/MPSURecord.cpp definitions.h
	$(CC) -c $(FLAGS) protocols/ProtocolMPSU.cpp
	mv *.o obj/
obj/ProtocolMOUSE.o : protocols/ProtocolMOUSE.cpp protocols/ProtocolMOUSE.h definitions.h
	$(CC) -c $(FLAGS) protocols/ProtocolMOUSE.cpp
	mv *.o obj/
obj/ProtocolCONFIG.o : protocols/ProtocolCONFIG.cpp protocols/ProtocolCONFIG.h definitions.h
	$(CC) -c $(FLAGS) protocols/ProtocolCONFIG.cpp
	mv *.o obj/
obj/MPSURecord.o : protocols/MPSURecord.cpp protocols/MPSURecord.h definitions.h
	$(CC) -c $(FLAGS) protocols/MPSURecord.cpp
	mv *.o obj/
obj/MODBUS_TCP.o : protocols/MODBUS_TCP.cpp protocols/MODBUS_TCP.h definitions.h
	$(CC) -c $(FLAGS) protocols/MODBUS_TCP.cpp
	mv *.o obj/
obj/global.o: global.cpp global.h definitions.h
	$(CC) -c $(FLAGS) global.cpp
	mv *.o obj/
obj/Socket.o: Socket.cpp Socket.h definitions.h
	$(CC) -c $(FLAGS) Socket.cpp
	mv *.o obj/
obj/ServerSocket.o: ServerSocket.cpp ServerSocket.h definitions.h
	$(CC) -c $(FLAGS) ServerSocket.cpp
	mv *.o obj/
obj/Semaphore.o: Semaphore.cpp Semaphore.h definitions.h
	$(CC) -c $(FLAGS) Semaphore.cpp
	mv *.o obj/
obj/CacheData.o: CacheData.cpp CacheData.h definitions.h
	$(CC) -c $(FLAGS) CacheData.cpp
	mv *.o obj/
obj/CmdPacket.o : CmdPacket.cpp CmdPacket.h definitions.h
	$(CC) -c $(FLAGS) CmdPacket.cpp
	mv *.o obj/
obj/DataPacket.o : DataPacket.cpp DataPacket.h definitions.h
	$(CC) -c $(FLAGS) DataPacket.cpp
	mv *.o obj/
obj/Message.o : Message.cpp Message.h definitions.h
	$(CC) -c $(FLAGS) Message.cpp
	mv *.o obj/
obj/MessageQueue.o : MessageQueue.cpp MessageQueue.h definitions.h
	$(CC) -c $(FLAGS) MessageQueue.cpp
	mv *.o obj/
obj/Address.o : Address.cpp Address.h definitions.h
	$(CC) -c $(FLAGS) Address.cpp
	mv *.o obj/
obj/Router.o : Router.cpp Router.h definitions.h
	$(CC) -c $(FLAGS) Router.cpp
	mv *.o obj/
obj/controller.o : definitions.h controller.cpp
	$(CC) -c $(FLAGS) controller.cpp
	mv *.o obj/