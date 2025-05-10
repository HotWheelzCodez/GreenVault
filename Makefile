.PHONY: all setup venv java server build run clean kill

JAVA_SRC = src/server
JAVA_FILES = $(JAVA_SRC)/Dotenv.java $(JAVA_SRC)/Encryption.java $(JAVA_SRC)/Sync.java $(JAVA_SRC)/Main.java
JAVA_CP = ./libs/mysql.jar:.

EXECUTABLE = FinanceTracker

all: setup server build run kill

setup: venv

venv:
	cd scripts && python3 -m venv .venv && . .venv/bin/activate

server: $(JAVA_FILES)
	javac -cp "$(JAVA_CP)" $(JAVA_FILES)
	cd $(JAVA_SRC) && java -cp "../../libs/mysql.jar:." Main &

build:
	rm -f $(EXECUTABLE)
	rm -rf build
	mkdir build
	cd build && cmake ..
	cd build && make -j8
	mv build/$(EXECUTABLE) .

run:
	./$(EXECUTABLE)
	sleep 1

kill:
	@if [[ "$$OSTYPE" == "darwin"* ]]; then \
	  PID=$$(lsof -ti :8080); \
	else \
	  PID=$$(ss -tulpn | awk '/:8080 / {gsub(".*pid=", "", $$NF); gsub(",.*", "", $$NF); print $$NF}'); \
	fi; \
	if [ -n "$$PID" ]; then \
	  kill $$PID; \
	fi
