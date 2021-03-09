package com.elevoc.bluetoothtest.dao

import ArrayUtil.checkSum
import ArrayUtil.toHex

/**
 * 指令结构：

发送：

指令头 + 总长度 + 总包数 + 当前包数 + app会话id + 耳机会话id + 主从 + TLV数量 + 类型 + 长度 + 数值 + 校验 + 指令尾

2字节     2字节    2字节  2字节       2字节       2字节    1字节    2字节   2字节   2字节   n字节 1字节  2字节

总长度：至少22 + n 字节，TLV可扩展

指令头：0xAA 0x55
指令尾：  0xFE 0xFE

校验：和校验，校验值前面的值全部加起来取低8位即可
 */
object CmdUtils {

    val testSize = 4

    var testError = true
    fun sendData(
            type: Int,
            data: ByteArray,
            sessionId: Int,
            earSessionId: Int
    ) {
        val startIndex = 0
        val endIndex = data.size / Byte.MAX_VALUE + 1

        for (index in startIndex until endIndex) {

            val len = if (((index + 1) * Byte.MAX_VALUE) > data.size) {
                (data.size % Byte.MAX_VALUE).toString().toByte()
            } else {
                Byte.MAX_VALUE
            }
            val byte = ByteArray(22 + len/* +testSize+4*/)
            var i = 0

            byte[i++] = 0xAA.toByte()
            byte[i++] = 0x55.toByte()


            //总长度  2字节
            i += LEN_TOTAL_SIZE// 先跳过
            // 总包数 2字节
            setInt(byte, i, LEN_TOTAL_PACKAGE, endIndex)
            i += LEN_TOTAL_PACKAGE
            // 当前包数 2字节
            setInt(byte, i, LEN_CUR_PACKAGE, index)
            i += LEN_CUR_PACKAGE
            // + app会话id   2字节
            setInt(byte, i, LEN_SESSION_ID, sessionId)
            i += LEN_SESSION_ID
            // + 耳机会话id  2字节
            setInt(byte, i, LEN_EAR_SESSION_ID, earSessionId)
            i += LEN_EAR_SESSION_ID
            // 主从  1字节
            byte[i++] = 0x01
            // + TLV数量   2字节
            val tlvCount = 1
            setInt(byte, i, LEN_TLV_SUM, tlvCount)
            i += LEN_TLV_SUM

/*            setInt(byte, i, LEN_TLV_TYPE, 0x0000)
            i += LEN_TLV_TYPE
            setInt(byte, i, LEN_TLV_LEN, testSize)
            i += LEN_TLV_LEN
            val test ="test"
            System.arraycopy(test.toByteArray(), 0, byte, i, testSize)
            i += testSize*/

            // + 类型  2字节
            setInt(byte, i, LEN_TLV_TYPE, type)
            i += LEN_TLV_TYPE
            // + 长度 2字节
            val lenInt: Int = len.toInt()
            setInt(byte, i, LEN_TLV_LEN, lenInt)
            i += LEN_TLV_LEN
            // + 数值 n字节
            System.arraycopy(data, (index * Byte.MAX_VALUE), byte, i, lenInt)
            i += lenInt

            // 补充 总长
            byte[2] = ((i + LEN_TAIL + LEN_CHECK_SUM).shr(8) and 0xff).toByte()
            byte[3] = ((i + LEN_TAIL + LEN_CHECK_SUM) and 0xff).toByte()

            // 校验  1字节
            byte[i++] = checkSum(byte, i)

            byte[i++] = 0xFE.toByte()
            byte[i] = 0xFE.toByte()


            println("send package:[${toHex(byte)}]")
//            println(toASCII(byte))
            Helper.getInstance().write(byte)
//            return byte
        }
    }

    /**
     * Caused by: java.lang.ArrayIndexOutOfBoundsException: length=6; index=9
    at com.elevoc.bluetoothtest.CmdUtils.setInt(CmdUtils.kt:107)
    at com.elevoc.bluetoothtest.CmdUtils.sendData(CmdUtils.kt:65)
    at com.elevoc.bluetoothtest.MainActivity.testSend(MainActivity.kt:131)
     */
    private fun setInt(data: ByteArray, offset: Int, len: Int, value: Int) {
        for (i in 0 until len) {
            data[offset + len - 1 - i] = (value.shr(8 * i) and 0xFF).toByte()
        }
    }
}