all: test

compile:
	node-gyp configure build

test: compile
	mocha
