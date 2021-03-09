class AAA{

}

val externalMap = mutableMapOf<AAA, String>()

var AAA.newExtensionProperty : String
    get() = externalMap[this] ?: "0"
    set(value:String) { externalMap[this] = value }

fun test(){
    val a = AAA()
    a.newExtensionProperty = "xxx"
    println(a.newExtensionProperty)
}
fun main(){
    test()
    // 创建一个类的实例
    val isEven = object : IntPredicate {
        override fun accept(i: Int): Boolean {
            return i % 2 == 0
        }
    }
    println(isEven)
    AAAA.test()
    BBBB.create()
}

interface IntPredicate {
    fun accept(i: Int): Boolean
}

object AAAA{
     fun test():String{
        println("test")
        return "test"
    }
}
class BBBB{
 companion object Factory{
     fun create():BBBB = BBBB()
 }

}