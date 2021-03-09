package com.elevoc.bluetoothtest.dao

import ArrayUtil
import kotlin.collections.ArrayList
import kotlin.collections.HashMap
import kotlin.concurrent.thread

/**
 * 返回：

指令头 + 总长度 + 总包数 + 当前包数 + app会话id + 耳机会话id + 主从 + TLV数量 + 类型 + 长度 + 数值 + 校验 + 指令尾

2字节      2字节      2字节       2字节        2字节        2字节          1字节     2字节      2字节    2字节    n字节    1字节    2字节

指令头：0xAA 0x55
指令尾：  0xFE 0xFE

校验：和校验，校验值前面的值全部加起来取低8位即可
 */

const val LEN_CHECK_SUM: Int = 1
const val LEN_TAIL: Int = 2
const val LEN_HEAD: Int = 2
const val LEN_TOTAL_SIZE: Int = 2
const val LEN_TOTAL_PACKAGE: Int = 2
const val LEN_CUR_PACKAGE: Int = 2
const val LEN_SESSION_ID: Int = 2
const val LEN_EAR_SESSION_ID: Int = 2
const val LEN_TLV_TYPE: Int = 2
const val LEN_TLV_SUM: Int = 2
const val LEN_TLV_LEN: Int = 2

const val OFFSET_HEAD: Int = 0
const val OFFSET_TOTAL_SIZE: Int = OFFSET_HEAD + LEN_HEAD
const val OFFSET_TOTAL_PACKAGE: Int = OFFSET_TOTAL_SIZE + LEN_TOTAL_SIZE
const val OFFSET_CUR_PACKAGE: Int = OFFSET_TOTAL_PACKAGE + 2
const val OFFSET_SESSION_ID: Int = OFFSET_CUR_PACKAGE + 2
const val OFFSET_EAR_SESSION_ID: Int = OFFSET_SESSION_ID + 2
const val OFFSET_ROLE: Int = OFFSET_EAR_SESSION_ID + 2
const val OFFSET_TLV_SUM: Int = OFFSET_ROLE + 1
const val OFFSET_TLV_TYPE: Int = OFFSET_TLV_SUM + 2
const val OFFSET_TLV_LENGTH: Int = OFFSET_TLV_TYPE + LEN_TLV_TYPE
const val OFFSET_TLV_VALUE: Int = OFFSET_TLV_LENGTH + LEN_TLV_LEN


class Helper//  检验通过，开始组装数据
// 获取TLV的总数量
// notify
// 组装数据// 读数据// 创建一个线程
() {
    val objMaps: HashMap<Short, DataRecv> = HashMap(2)

    interface IReadObserver {
        fun onRead(data: DataRecv)
    }

    val readObservers: ArrayList<IReadObserver> = ArrayList()

    var ringBuffer: RingBuffer

    init {
        ringBuffer = RingBuffer(4096)
        thread {
            // 读数据
            val data: ByteArray = ByteArray(4096)
            var lastOffset = 0
            while (true) {
                val retLen = ringBuffer.read(data, lastOffset, 256)

//                println("readSize $retLen[${ArrayUtil.toHex(data)}]")
                if (retLen > 0 || lastOffset > 0) {
                    // 组装数据
                    var ret1 = checkHead(data, OFFSET_HEAD, retLen)
                    if (ret1) {
                        var totalSize = checkTotalSize(data, OFFSET_TOTAL_SIZE, retLen)
                        println("totalSize:$totalSize")
                        var ret2 = totalSize > 0
                        var ret3 = checkTail(data, totalSize - LEN_TAIL, retLen)
                        var ret4 = checkchksum(
                                data,
                                totalSize - LEN_TAIL - LEN_CHECK_SUM,
                                retLen,
                                totalSize
                        )

                        println("totalSize:$totalSize $ret1 $ret2 $ret3 $ret4")

                        if (ret1 && ret2 && ret3 && ret4
                        ) {
                            // 获取当前的包数，以及总包数
                            val currPkg = getDataWithLen(data, OFFSET_CUR_PACKAGE, LEN_CUR_PACKAGE)
                            val totalPkg = getDataWithLen(data, OFFSET_TOTAL_PACKAGE, LEN_TOTAL_PACKAGE)
                            println("curr:$currPkg total:$totalPkg")

                            //  检验通过，开始组装数据
                            // 获取TLV的总数量
                            val tlvCount = getTLVCount(data, OFFSET_TLV_SUM, retLen)
                            var tlvOffset = OFFSET_TLV_TYPE
                            val innerSessionId = getSessionId(data, OFFSET_SESSION_ID, retLen)
                            var recv: DataRecv? = objMaps.get(innerSessionId)
                            if (currPkg != 0 && recv == null) {
                                println("something error . [${innerSessionId}] current pkg $currPkg/$totalPkg,but not got zero package")
                            }
                            if (currPkg != (totalPkg - 1) && recv == null) {
                                recv = DataRecv()
                                objMaps.put(innerSessionId, recv)
                            } else if (recv == null) {
                                recv = DataRecv()
                            }
                            if ((recv.currPkg + 1) != currPkg) {
                                println("something error . [${innerSessionId}] may not got full package, last pkg index is ${recv.currPkg} ,current pkg is $currPkg")
                            }
                            recv.currPkg = currPkg

                            for (index in 0 until tlvCount) {
                                val tlvPackage: TLVPackage = getTLVPackage(data, tlvOffset)
                                tlvOffset += LEN_TLV_TYPE
                                tlvOffset += LEN_TLV_LEN
                                tlvOffset += tlvPackage.len
                                recv.tlvPackage.add(tlvPackage)
                            }
                            recv.tlvCount = (recv.tlvCount + tlvCount).toShort()
                            recv.earSessionId = getEarSessionId(data, OFFSET_EAR_SESSION_ID, retLen)
                            recv.sessionId = innerSessionId
                            if (currPkg == (totalPkg - 1)) {
                                // notify
                                readObservers.forEach { it.onRead(recv) }
                                objMaps.remove(innerSessionId) // 清除
                            }

                            // 偏移
                            if ((lastOffset + retLen) > totalSize) {
                                lastOffset = (lastOffset + retLen) - totalSize
                                System.arraycopy(data, totalSize, data, 0, lastOffset)
                            } else {
                                lastOffset = 0
                            }
                        } else {
                            lastOffset = (lastOffset + retLen) - (LEN_HEAD)
                            System.arraycopy(data, (LEN_HEAD), data, 0, lastOffset)
                        }
                    } else {
                        // 检验失败，则往后偏移 一个字节
                        lastOffset = (lastOffset + retLen) - 1
                        println("check head error, offset + 1,retry lastOffset:$lastOffset")
                        System.arraycopy(data, 1, data, 0, lastOffset)
                    }
                }
                Thread.sleep(10)
            }
        }
    }

    private fun getSessionId(data: ByteArray, offset: Int, retLen: Int): Short {
        return getDataWithLen(data, offset, LEN_SESSION_ID).toShort()
    }

    private fun getEarSessionId(data: ByteArray, offset: Int, retLen: Int): Short {
        return getDataWithLen(data, offset, LEN_EAR_SESSION_ID).toShort()
    }

    private fun getTLVPackage(data: ByteArray, tlvOffset: Int): TLVPackage {
        val tlvtype = getDataWithLen(data, tlvOffset, LEN_TLV_LEN)
        val tlvLen = getDataWithLen(data, tlvOffset + LEN_TLV_TYPE, LEN_TLV_LEN)
        val tlvPackage =
                TLVPackage(enumValues<TLVPackage.Type>().find { it.tag == tlvtype.toShort() }
                        ?: TLVPackage.Type.FEQ_ELSE, tlvLen.toShort())
        tlvPackage.value = ByteArray(tlvLen)
        System.arraycopy(
                data,
                tlvOffset + LEN_TLV_LEN + LEN_TLV_TYPE,
                tlvPackage.value!!,
                0,
                tlvLen
        )
        return tlvPackage
    }

    private fun getTLVCount(data: ByteArray, offset: Int, retLen: Int): Short {
        return getDataWithLen(data, offset, LEN_TLV_SUM).toShort()
    }

    private fun getDataWithLen(data: ByteArray, offset: Int, needLen: Int): Int {
        if (offset < 0) return 0
        var value = 0
        for (i in 0 until needLen) {
            value = value or (data[offset + i].toInt().shl(8 * (needLen - 1 - i)) and 0xFF)
        }
        return value
    }

    private fun checkchksum(data: ByteArray, offset: Int, retLen: Int, totalSize: Int): Boolean {
        if (offset < 0) return false
        return ArrayUtil.checkSum(data, offset) == data[offset]
    }

    private fun checkTail(data: ByteArray, offset: Int, retLen: Int): Boolean {
        if (offset < 0) return false
        return data[offset] == 0xFE.toByte() && data[offset + 1] == 0xFE.toByte()
    }

    private fun checkTotalSize(data: ByteArray, offset: Int, retLen: Int): Int {
        // 如果ret 过大 则有问题
        return getDataWithLen(data, offset, LEN_TOTAL_SIZE)
    }

    private fun checkHead(data: ByteArray, offset: Int, retLen: Int): Boolean {
        if (offset < 0) return false
        return data[offset] == 0xAA.toByte() && data[offset + 1] == 0x55.toByte()
    }


    //单例
    companion object Instance {
        private var instance: Helper? = null
        fun getInstance(): Helper {
            return instance ?: synchronized(this) {
                instance ?: Helper().also { instance = it }
            }
        }
    }

    fun write(byteArray: ByteArray?) {
        if (byteArray != null) {
            ringBuffer.write(byteArray, 0, byteArray.size)
        }
    }

    //    新增订阅者
    fun addReaderObserver(observer: IReadObserver) {
        readObservers.add(observer)
    }

}