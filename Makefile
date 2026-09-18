GREEN = \033[32m
RED = \033[31m
YELLOW = \033[33m
RESET = \033[0m

OBJ_FILES = obj/handle_client_functions.o \
            obj/server.o \
            obj/timestamp.o \
            obj/server_functions.o \
            obj/handle_client.o

TARGET = bin/server

.PHONY: setup compile remove

setup:
	@if [ -d logs ]; then \
		printf "$(YELLOW) [INFO] logs/ directory already exists!\n$(RESET)"; \
	else \
		printf "$(YELLOW) [INFO] making logs/ directory and generating server.log file...\n$(RESET)"; \
		mkdir -p logs; \
		touch logs/server.log; \
		printf "$(GREEN) [OK] logs/ directory made and server.log generated!\n$(RESET)"; \
	fi

	@if [ -d obj ]; then \
		printf "$(YELLOW) [INFO] obj/ directory already exists!\n$(RESET)"; \
	else \
		printf "$(YELLOW) [INFO] making obj/ directory...\n$(RESET)"; \
		mkdir -p obj; \
		printf "$(GREEN) [OK] obj/ made!\n$(RESET)"; \
	fi

	@if [ -d bin ]; then \
		printf "$(YELLOW) [INFO] bin/ directory already exists!\n$(RESET)"; \
	else \
		printf "$(YELLOW) [INFO] making bin/ directory...\n$(RESET)"; \
		mkdir -p bin; \
		printf "$(GREEN) [OK] bin/ made!\n$(RESET)"; \
	fi

	@if [ -d tmp ]; then \
		printf "$(YELLOW) [INFO] tmp/ directory already exists!\n$(RESET)"; \
	else \
		printf "$(YELLOW) [INFO] making tmp/ directory...\n$(RESET)"; \
		mkdir -p tmp; \
		printf "$(GREEN) [OK] tmp/ made!\n$(RESET)"; \
	fi

compile:
	@printf "$(YELLOW) [INFO] compiling source code...\n$(RESET)"
	$(MAKE) $(TARGET)
	@printf "$(GREEN) [OK] compilation done.\n$(RESET)"

$(TARGET): $(OBJ_FILES)
	gcc $(OBJ_FILES) -o $(TARGET)

obj/server.o: src/server.c
	gcc -c src/server.c -o obj/server.o

obj/handle_client_functions.o: src/handle_client_functions.c
	gcc -c src/handle_client_functions.c -o obj/handle_client_functions.o

obj/timestamp.o: src/timestamp.c
	gcc -c src/timestamp.c -o obj/timestamp.o

obj/server_functions.o: src/server_functions.c
	gcc -c src/server_functions.c -o obj/server_functions.o

obj/handle_client.o: src/handle_client.c
	gcc -c src/handle_client.c -o obj/handle_client.o

remove:
	@printf "$(YELLOW) [INFO] removing compiled program...\n$(RESET)"
	rm -f obj/*.o obj/*.d $(TARGET)
	@printf "$(GREEN) [OK] removal done.\n$(RESET)"