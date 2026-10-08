CC      := gcc
MPICC   := mpicc
CFLAGS  := -O2 -Wall -Wextra -std=c11 -pedantic
LDFLAGS :=

BIN_DIR := build

.PHONY: all sequential mpi test clean

all: sequential mpi test

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

sequential: $(BIN_DIR)/vector_dot_sequential

mpi: $(BIN_DIR)/vector_dot_mpi

test: $(BIN_DIR)/mpi_test

$(BIN_DIR)/vector_dot_sequential: src/vector_dot_sequential.c | $(BIN_DIR)
	$(CC) $(CFLAGS) src/vector_dot_sequential.c -o $@ $(LDFLAGS)

$(BIN_DIR)/vector_dot_mpi: src/vector_dot_mpi.c | $(BIN_DIR)
	$(MPICC) $(CFLAGS) src/vector_dot_mpi.c -o $@ $(LDFLAGS)

$(BIN_DIR)/mpi_test: src/mpi_test.c | $(BIN_DIR)
	$(MPICC) $(CFLAGS) src/mpi_test.c -o $@ $(LDFLAGS)

clean:
	rm -rf $(BIN_DIR)
