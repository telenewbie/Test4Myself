package com.elevoc.bluetoothtest.dao

class TLVPackage(val types: Type, val len: Short) {
    enum class Type(val tag: Short) {
        UID_GET(0x0000), // 获取设备UID

        FEQ_GET(0x0001), // 获取设备当前主频
        FEQ_SET(0x0002), // 设置设备当前主频
        FEQ_ELSE(0xFFFF.toShort()), // 啥情况
    }

    var value: ByteArray? = null

    fun writeData(data: ByteArray?) {
        value = data
    }
}