package com.elevoc.bluetoothtest.dao

import kotlin.math.min

class RingBuffer(private val capacity: Int) {

    @Volatile
    var readIndex = 0

    @Volatile
    var writeIndex = 0

    val bytes: ByteArray = ByteArray(capacity)

    fun write(data: ByteArray?, offset: Int, len: Int): Boolean {
        if (data != null && !isFull()) { // not full
            if (remainSize() >= len) {
                if (writeIndex + len > capacity) {
                    val paddingLen = (capacity - writeIndex)
                    System.arraycopy(data, offset, bytes, writeIndex, paddingLen)
                    System.arraycopy(data, offset + paddingLen, bytes, 0, len - paddingLen)
                } else {
                    System.arraycopy(data, offset, bytes, writeIndex, len)
                }
                writeIndex = (writeIndex + len) % capacity
//                println("writeIndex $writeIndex")
                return true
            }
        }
        return false
    }

    fun size(): Int = (writeIndex - readIndex + capacity) % capacity


    fun remainSize(): Int = capacity - (size() + 1)


    fun isEmpty(): Boolean = size() == 0


    fun isFull(): Boolean = remainSize() == 0


    fun read(outBuffer: ByteArray, offset: Int, len: Int): Int {
        if (!isEmpty()) { //not empty
            val minLen = min(size(), len)
            if (readIndex + minLen > capacity) {
                val paddingLen = (capacity - readIndex)
                System.arraycopy(bytes, readIndex, outBuffer, offset, paddingLen)
                System.arraycopy(bytes, 0, outBuffer, offset + paddingLen, minLen - paddingLen)
            } else {
                System.arraycopy(bytes, readIndex, outBuffer, offset, minLen)
            }
            readIndex = (readIndex + minLen) % capacity
//            println("readIndex $readIndex return:$minLen")
            return minLen
        }
        return 0
    }

}