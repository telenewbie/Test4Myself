#!/bin/bash

filename=cryption_addr.h

OBJDUMP=${OBJDUMP:="objdump"}
READELF=${READELF:="readelf"}

libfile=$1

${OBJDUMP} -d -j .text ${libfile} > txz.txt

echo  "#define START_INDEX      \ " >${filename}
echo  "{    \ " >>$filename



for  i in $* ; do
  echo ${i}
  if [ ${i} = $1 ] ; then
    continue
  fi
  L=`cat txz.txt|awk '$2=="<"PARA">:" {print NR}' PARA=${i}`;
  H=`cat txz.txt|awk 'NF== 0 && NR>L {print NR}' L=$L | awk 'NR==1 {print $1}'`;
  #echo $L $H
  begin=`cat txz.txt|awk 'NR==L {print $1}' L=$L`
  end=`cat txz.txt|awk 'BEGIN{FS=":"} NR==H-1 {print $1}' H=$H`
  end=`echo $end | sed 's/^[ ]*//g'`
echo "{    \ ">>${filename}
echo "0x${begin}    \ ">>${filename}
echo ",    \ ">>${filename}
echo "0x${end}    \ " >>${filename}
echo "},    \ " >>${filename}
  echo $begin $end
done



echo "}" >>${filename}

echo """
struct
{
      uint32_t start_index;
          uint32_t end_index;

} cryption_addr_array[] = START_INDEX;

""">>${filename}
