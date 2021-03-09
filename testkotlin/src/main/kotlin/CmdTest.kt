import kotlin.experimental.xor

fun sendData(data: ByteArray) {
    val startIndex = 0
    val endIndex = data.size / Byte.MAX_VALUE + 1

    for (index in startIndex until endIndex) {

        val len = if (((index + 1) * Byte.MAX_VALUE) > data.size) {
            (data.size % Byte.MAX_VALUE).toString().toByte()
        } else {
            Byte.MAX_VALUE
        }
        val byte = ByteArray(11 + len)
        var i = 0
        byte[i++] = 'E'.toByte()
        byte[i++] = 'L'.toByte()
        byte[i++] = 'E'.toByte()
        byte[i++] = 0x20 // type
        byte[i++] = 1 // reqid
        byte[i++] = 0 // respid

        byte[i++] = index.toByte()
        byte[i++] = startIndex.toByte()
        byte[i++] = endIndex.toByte()

        byte[i++] = len
        // respid
        System.arraycopy(data, (index * Byte.MAX_VALUE), byte, i, len.toInt())
        i += len
        byte[i] = checkSum(byte, i)
        println(toHex(byte))
        println(toASCII(byte))
    }
}

fun toASCII(data: ByteArray): String? {
    val buffer = StringBuffer()
    for (i in data.indices) {
        buffer.append(data[i].toChar())
    }
    return buffer.toString()
}

fun toHex(data: ByteArray): String? {
    val buffer = StringBuffer()
    for (i in data.indices) {
        buffer.append(String.format("%02x", data[i])).append(",")
    }
    return buffer.toString()
}

fun checkSum(data: ByteArray, len: Int): Byte {
    var sum = 0.toByte()
    for (i in 0 until len) {
        sum = sum xor data[i]
    }
    return sum
}

fun main() {
    sendData("cmake -DCMAKE_TOOLCHAIN_FILE=D:\\env\\android-ndk-r20b/build/cmake/android.toolchain.cmake -DANDROID_NDK=D:\\env\\android-ndk-r20b  -DCMAKE_BUILD_TYPE=Release   -DANDROID_ABI=armeabi-v7a -DCMAKE_GENERATOR=Ninja   -DANDROID_PLATFORM=android-21  -DBUILD_SHARED_LIBS=True   -DPRJ_NAME=innertest   -DCMAKE_VERBOSE_MAKEFILE=ON D:\\workspace\\AndroidStudio\\singlechannel\\SingleChannelEngine\\mk\\cmakeBuild".toByteArray())
}