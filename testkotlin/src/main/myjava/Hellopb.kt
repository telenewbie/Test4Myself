package myjava

import com.google.protobuf.Message
import muduo.TestPb


fun main() {
    println("hello")
    // 需要将byte [] 转换成为 pb对象


    val quer:TestPb.Query

    quer = TestPb.Query.newBuilder().build()

    val findFieldByNumber = quer.descriptorForType.findFieldByNumber(1)
//    println(findFieldByNumber)
    //通过Description 进行 设置参数
    quer.toBuilder().setField(findFieldByNumber, 1)



    println(quer.inT32)

}