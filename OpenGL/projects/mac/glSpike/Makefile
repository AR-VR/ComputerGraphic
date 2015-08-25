PROJECT := glSpike
SCRIPT :=$(PROJECT) #add arguments

#set the compiler option
CFLAGS := -Wall \
				  -pedantic \
				  -Wextra \
				  -Wno-missing-field-initializers \

CC := clang++

OBJECT_DIR :=obj/
SOURCE_DIR :=src/

#header include
INCLUDES := -I/usr/local/include \
						-I/opt/X11/include \
						-I/opt/X11/lib \
						-I./$(SOURCE_DIR) \
						

#library link
LD_LIBS := -L/usr/local/lib -lGLUT -framework OpenGL

SOURCES :=$(wildcard $(SOURCE_DIR)*.cpp) 
OBJECTS :=$(SOURCES:$(SOURCE_DIR)%.cpp=$(OBJECT_DIR)%.o)

$(PROJECT): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(PROJECT) $(OBJECTS) $(LD_LIBS)

$(OBJECT_DIR)%.o: $(SOURCE_DIR)%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

debug: CFLAGS += -DDEBUG -g
debug: all

all: $(PROJECT) $(OBJECTS)

run: all
	./$(SCRIPT)

clean:
		rm -rf $(OBJECT_DIR)*.o $(PROJECT)
