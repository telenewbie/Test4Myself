fun main() {


    val a:Byte = 0x95.toByte()
    val b:Byte = 0x00.toByte()
    // 0095
    val c = b.toInt().shl(8) or (a.toInt().shl(0) and  0xFF)
    println(c)
}