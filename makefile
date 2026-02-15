PROGRAM_NAME= headify
MAIN= main
CC= gcc

all:

	${CC} -o ${PROGRAM_NAME} ${MAIN}.c

clean:
	rm -f headify main
