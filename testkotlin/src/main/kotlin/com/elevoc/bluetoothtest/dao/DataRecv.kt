package com.elevoc.bluetoothtest.dao

import java.util.*

/**
 * 返回：

指令头 + 总长度 + 总包数 + 当前包数 + app会话id + 耳机会话id + 主从 + TLV数量 + 类型 + 长度 + 数值 + 校验 + 指令尾

2字节   2字节  2字节       2字节        2字节      2字节    1字节    2字节     2字节   2字节   n字节    1字节    2字节

指令头：0xAA 0x55
指令尾：  0xFE 0xFE

校验：和校验，校验值前面的值全部加起来取低8位即可
 */

class DataRecv {

    var tlvPackage: LinkedList<TLVPackage> = LinkedList()

    var tlvCount: Short = 0

    var sessionId: Short = 0

    var earSessionId: Short = 0

    var currPkg: Int = -1
}