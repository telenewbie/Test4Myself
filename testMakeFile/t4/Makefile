
#SRCS := main.c test.c
SRCS := $(wildcard *.c)
#OBJS := $(patsubst %.c,%.o,$(SRCS))
OBJS := $(SRCS:.c=.o)
SRCS_DEPS := $(patsubst %.c,.%.o.d,$(SRCS))

main:$(OBJS)
	@echo $(SRCS_DEPS)
	$(CC) -o $@ $^

#main.o:defs.h
%.o:%.c
	#$(CC) -c $< -o $@ -MD -MF "$(dir $@).$(notdir $@).d" -MT $@

	# 显示依赖关系的有 -M -MM -MD 
	# -M 所有的依赖 生成 预处理后的所有依赖结果包括间接依赖
	# -MM 直接的依赖
	# -MD 类似于-MM 但是可以加 编译参数，其他的则不行，例如本例里面的 `-c`
	# 在此处，我们需要生成 .o 文件 只有 -MD 满足我们 想要获取依赖 并且想要 调整编译参数
	# -MT 表示 生成的 依赖项 作为 $@[即*.o]的依赖项，相当于 main.o : main.c defs.h test.h app.h
	# -MF 因为默认会生成 项目名称去掉后缀之后加上.d 后缀 ：相当于 main.o 变成 main.d
	$(CC) -c $< -o $@ -MD  -MT $@

.PHONY:clean
clean:
	@echo "clean"
	-@rm -rf *.o
	-@rm main
	@echo "what"
	@$(RM) $(SRCS_DEPS)
-include $(SRCS_DEPS)
