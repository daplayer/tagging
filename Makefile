all: compile test

compile:
	node-gyp configure build --verbose

.PHONY: test
test:
	mocha
