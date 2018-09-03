CC=gcc
TARGET = swap_ls
TARGET_D = swap_ls_dynamic 
CTARGET = swap_ls
CFLAGS = -DDYNAMIC 

default: $(CTARGET).c
	$(CC) -o $(TARGET) $(CTARGET).c 

dynamic: $(CTARGET).c
	$(CC) $(CFLAGS) -o $(TARGET_D) $(CTARGET).c 

all: default dynamic 

clean:
	$(RM) $(TARGET) $(TARGET_D)
