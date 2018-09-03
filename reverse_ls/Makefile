CC=gcc
TARGET = rev_ls
TARGET_D = rev_ls_dynamic 
CTARGET = reverse_list
CFLAGS = -DDYNAMIC 

default: $(CTARGET).c
	$(CC) -o $(TARGET) $(CTARGET).c 

dynamic: $(CTARGET).c
	$(CC) $(CFLAGS) -o $(TARGET_D) $(CTARGET).c 

all: default dynamic 

clean:
	$(RM) $(TARGET) $(TARGET_D)
