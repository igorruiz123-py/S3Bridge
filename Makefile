GREEN = \033[32m
RED = \033[31m
YELLOW = \033[33m
RESET = \033[0m

.PHONY: setup

setup:
	
	@if [ -d logs ]; then \
		echo "${YELLOW} [INFO] logs/ directory already exists! $(RESET)\n"; \
	else \
		echo "$(YELLOW) [INFO] making logs/ directory and generating server.log file... $(RESET)\n"; \
		mkdir logs/; \
		touch logs/server.log; \
		echo "$(GREEN) [OK] logs/ directory made and server.log generated! $(RESET)\n"; \
	fi

	@if [ -d obj ]; then \
		echo "$(YELLOW) [INFO] obj/ directory already exists! $(RESET)\n"; \
	else \
		echo "$(YELLOW) [INFO] making obj/ directory... $(RESET)\n"; \
		mkdir obj/; \
		echo "$(GREEN) [OK] obj/ made! $(RESET)\n"; \
	fi

	@if [ -d bin ]; then \
		echo "$(YELLOW) [INFO] bin/ directory already exists! $(RESET)\n"; \
	else \
		echo "$(YELLOW) [INFO] making bin/ directory... $(RESET)\n"; \
		mkdir bin/; \
		echo "$(GREEN) [OK] bin/ made! $(RESET)\n"; \
	fi


