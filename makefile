compile: controller.c writer.c
	gcc controller.c -o controller; gcc writer.c -o writer

clean:
	rm controller
	rm writer
	rm *~
