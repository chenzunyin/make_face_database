CXX = g++
CFLAGS = -Wall -I./inc -std=gnu++11
LDFLAGS_CV += `pkg-config --cflags --libs opencv`  -lopencv_highgui

FUNC = ./func/
OUTPUT = ./output/
DATABASE = ./database/
SINGLE = $(FUNC)add_single.c
SINGLE_OUT = $(OUTPUT)add_single
CYCLE = $(FUNC)cycle_add_person.c
CYCLE_OUT = $(OUTPUT)cycle_add_person
FACE_DB = $(DATABASE)tiny_face.db
TMP_DB = $(DATABASE)tmp_face.db

all: single cycle

single:
	$(CXX) $(CFLAGS) -o $(SINGLE_OUT) $(SINGLE) -L ./lib -lface_db -lReadFace -lopenblas $(LDFLAGS_CV) 
cycle:
	$(CXX) $(CFLAGS) -o $(CYCLE_OUT) $(CYCLE) -L ./lib -lface_db -lReadFace -lopenblas $(LDFLAGS_CV) 

.PHONY:clean
clean:
	-rm -rf $(SINGLE_OUT) $(CYCLE_OUT) $(FACE_DB) $(TMP_DB) 
