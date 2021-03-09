import com.elevoc.bluetoothtest.dao.CmdUtils
import com.elevoc.bluetoothtest.dao.DataRecv
import com.elevoc.bluetoothtest.dao.Helper

fun main() {
    Helper.getInstance().addReaderObserver(object : Helper.IReadObserver {
        override fun onRead(data: DataRecv) {
            println("${data.earSessionId} ${data.sessionId} ${data.tlvCount}")
            if (data.tlvPackage != null) {
                for (item in data.tlvPackage!!) {
                    println(ArrayUtil.toASCII(item.value))
//                    println("tlvpackage:${item.len} ${item.types}")
//                    println(ArrayUtil.toHex(item.value))
                }
            } else {
                println("tlvpackage is none")
            }
        }
    })
    CmdUtils.sendData(0x0000,
            "Kotlin ".toByteArray(),
            2, 0)
    CmdUtils.sendData(0x0002,
            "Kotlin does not seem to natively support negative values, but rather treats a negative as -(positive). This is problematic in many situations. Handling of negative hex literals should also be improved.Since it is returning this if it is true, it could be used for chaining the operation. Hence something as below would be improved.".toByteArray(),
            1, 0)
    CmdUtils.sendData(0x0001,
            "".toByteArray(),
            3, 0)
    CmdUtils.sendData(0x0003,
            "May 28, 2018 — ... for the other primitive types as well, such as ByteArray , CharArray , etc.) ... In contrast, Sequence is a new concept in Kotlin to represent a lazily ... but it has fairly clean bytecode compared to the three other methods before it.".toByteArray(),
            4, 0)

    Thread.sleep(10000)

}