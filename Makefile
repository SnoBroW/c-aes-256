all:
		@gcc -c -g *.c
		@gcc *.o -g -o aes
		@echo Done!

clean:
		@rm -rf aes *.o
		@echo Done!
