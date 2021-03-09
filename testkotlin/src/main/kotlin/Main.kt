import kotlin.concurrent.thread

class Main {
    var flag = true
}


fun main() {
    val bytes:ByteArray = ByteArray(100)
    val data:ByteArray = ByteArray(20)
    System.arraycopy(data,0,bytes,0,data.size)
    val main = Main()
    val t1 = thread {
        println("start1")
        while (main.flag) {
//            print("execute")
        }
        println("end1")
    }
    val t2 = thread {
        println("start2")
        main.flag = false
        println("end2")
    }
    t2.join()
    t1.join()
    println("over")
}