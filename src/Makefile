# Makefile RC 2021/2022 (Grupo 18) @ IST

# Compiler and flags
CC=gcc
CFLAGS= -Wall -g

# Executables' names
CLIENT_EXEC = user
SERVER_EXEC = DS

# Object directory's name
ODIR = obj

# Common dependencies
DEPS = centralizedmsg-api.h centralizedmsg-api-constants.h
# Add client dependencies
DEPS += client/centralizedmsg-client-api.h
# Add server dependencies
DEPS += server/centralizedmsg-server-api.h server/ds-api/ds-operations.h server/ds-api/ds-udpandtcp.h

_OBJ1 += centralizedmsg-api.o centralizedmsg-client.o centralizedmsg-client-api.o 
OBJ1 = $(patsubst %,$(ODIR)/%,$(_OBJ1))
_OBJ2 += centralizedmsg-api.o centralizedmsg-server.o centralizedmsg-server-api.o ds-operations.o ds-udpandtcp.o
OBJ2 = $(patsubst %,$(ODIR)/%,$(_OBJ2))


.PHONY: all clean run

all: $(CLIENT_EXEC) $(SERVER_EXEC)

# Compile client
$(CLIENT_EXEC): $(OBJ1)
	@$(CC) $(CFLAGS) -o $@ $^ 
	$(info Client compiled successfully!)
	$(info To run client -> ./$(CLIENT_EXEC) [-n DSIP] [-p DSPORT])


# Compile server
$(SERVER_EXEC): $(OBJ2)
	@$(CC) $(CFLAGS) -o $@ $^
	$(info Server compiled successfully!)
	$(info To run server -> ./$(SERVER_EXEC) [-p DSport] [-v])

# Create .o for all .c inside the main src2 directory
$(ODIR)/%.o: %.c $(DEPS)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c -o $@ $< 

# Create .o for all .c inside the client directory
$(ODIR)/%.o: client/%.c $(DEPS)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c -o $@ $< 

# Create .o for all .c inside the server directory
$(ODIR)/%.o: server/%.c $(DEPS)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c -o $@ $< 

# Create .o for all .c inside ds-api directory
$(ODIR)/%.o: server/ds-api/%.c $(DEPS)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c -o $@ $< 

# Delete the objects' directory and both executables
clean:
	@rm -rf $(ODIR)
	@rm -f *~ core $(INCDIR)/*~ $(CLIENT_EXEC) $(SERVER_EXEC)
	$(info Cleaned successfully!)
