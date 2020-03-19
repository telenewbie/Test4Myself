#!/bin/bash
# output random number
echo ${RANDOM}

# 参数大于1
if [ $# -gt 1 ];
then
  echo "hello"
fi

#获取第一个参数

# 获取第二个参数
libfile=$1
#libsym=$2
echo $libfile
#echo $libsym

OBJDUMP=${OBJDUMP:="objdump"}
READELF=${READELF:="readelf"}

#头文件
header_file="cryption_addr.h"



#lib_sym_offset=`${READELF} -s ${libfile} | grep ${libsym} | awk '{print $2}'`

#echo $lib_sym_offset

lib_sec_offset=`${READELF} -SW ${libfile} | grep .text | awk '{print $(NF-7)}'`;

echo $lib_sec_offset

lib_dat_offset=`${READELF} -SW ${libfile} | grep .text | awk '{print $(NF-6)}'`;

lib_dat_size=`${READELF} -SW ${libfile} | grep .text | awk '{print $(NF-5)}'`;

echo  $lib_dat_offset
echo $lib_dat_size

libmacro=`echo ${libname} | sed 's/[^0-9a-zA-Z\_]/___/g'`;

echo $libmacro

# ${READELF}  --dyn-syms  libhelloOri.so |grep helloB |awk '{print $2}'

#IN_F=$libfile
#OUT_F=./tmp1.bin
#SECTION=.text

#${OBJDUMP} -h $IN_F |
#  grep $SECTION |
#    awk '{print "dd if='$IN_F' of='$OUT_F' bs=1 count=$[0x" $3 "] skip=$[0x" $6 "]" }' |
#    bash

# 获取.text 段的大小
lib_text_size=`${READELF} -S -W ${libfile}  |grep .text|awk '{print $(NF-5)}'`
# 获取 frame_dummy 的位置
lib_frame_dummy_offset=`${OBJDUMP} -d -j .text ${libfile} |grep "frame_dummy>:"|awk '{print $1}'`

## 如果没有该段则认为整段.text 都可以加密
echo $lib_frame_dummy_offset

echo $lib_text_size


if [ -z  $lib_frame_dummy_offset ];then
  # 长度为0
  lib_start_offset=${lib_dat_offset}
  echo "#define START_INDEX (0x${lib_dat_offset})"> ${header_file}
  echo "#define END_INDEX (0x${lib_dat_offset}+0x${lib_text_size})">> ${header_file}

  echo "if"
else
  # 覆写
  echo "#define START_INDEX (0x${lib_frame_dummy_offset}+44)" >${header_file}
  # 添加
  echo "#define END_INDEX (0x${lib_dat_offset}+0x${lib_text_size})">> ${header_file}

  echo "else"
fi


# 预估下一个函数的开始位置 [+44]




echo "world"
