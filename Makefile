all: test

compile:
	node-gyp configure build --verbose

test: compile
	mocha
