%: %.o
	@echo "Checking.."
	${CC} ${LINKERFLAG} $< -o $@

%.o: %.c # % 匹配任何的目标，但是如果makefile只有该文件是不行的
	@echo "Creating object.."
	${CC} -c $< -o $@

SRCS := $(wildcard *.c) # 遍历所有的后缀名为.c 的文件
BINS := $(SRCS:%.c=%) # 这里的意思是把SRCS中的.c 去掉 相当于substr

BINS_2 := $(SRCS:%.c=$$(pwd)/%.cpp)

BINS_1 := $(basename hello.c)  # 这里会去掉后缀，

all: ${BINS}
	@echo ${SRCS}
	@echo ${BINS}
	@echo ${BINS_1}
	@echo "BINS_2:"${BINS_2}
	-mkdir test
	#查看 () 和{} 的区别，结论：在makefile里面是没有区别的，只在执行传递给shell的时候有区别
	@echo ${pwd}
	@echo $${pwd}
	@echo $(pwd)
	@echo $$(pwd) # 这里会执行pwd，其他不会

ifeq ("x${x}", "x")
	@echo "judge success"
endif

content :=$(shell pwd)
t1:
	@echo ${content}
	@echo $$(pwd)

t2:
	-@if [ x = x"${xx}" ] ; then \
		echo "same"; \
	else \
		echo "different"; \
	fi
