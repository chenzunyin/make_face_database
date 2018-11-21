CXX = g++
CFLAGS= -fPIC -shared -I ./inc
TARGET = libface_db.so 
SINGLE = ./face_db.c

single:
	$(CXX) $(CFLAGS) -o $(TARGET) $(SINGLE)  
install:
	cp $(TARGET) ../lib

.PHONY:clean
clean:
	-rm -rf $(TARGET) 
