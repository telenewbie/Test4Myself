#.DEFAULT_GOAL := generate

# 如果这里注释掉，则会导致say_hello 不执行，因为有同名文件存在
.PHONY: all say_hello generate clean

all: say_hello generate

say_hello:
	@echo "hello world"
	
generate:
	@echo "create empty"
	touch file-{1..10}.txt

clean:
	@echo "Clean up"
	rm *.txt
