#SUBDIRS = src tests

default:
	make -C ./src

run:
	make -C ./src run

all:
	make -C ./src
	make -C ./test

test:
	make -C ./test
