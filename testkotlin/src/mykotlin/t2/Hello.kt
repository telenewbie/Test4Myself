@file:JvmName("Example")
package mykotlin.t2 // 在IDEA 中包名不允许为kotlin

import myjava.HelloJava // 在IDEA 中包名不允许为java
import java.util.concurrent.locks.Lock
import java.util.concurrent.locks.ReentrantLock
import kotlin.reflect.KClass
import mykotlin.t1.KotlinClass

// 连类名都可以没有
fun main(args: Array<String>) {

    println("hello kotlin!")

    // String 和String? 时两个不同的类型
    // var 和 val 表示 可变变量 和 不可变变量
    var name: String? = null
    var name1: String

    var hello: HelloJava? = null          // 只有变量为 ? 这个类型的才可以赋值null
    name = hello?.java("name")     // 不为空的时候会执行后面
    hello = HelloJava()             // 实例化 可以不用写 new 来实例化一个对象
    hello ?: return                   // 为空的时候会执行后面

    name = hello?.java("name")   // !! 表示强转
    name1 = name!!

    //调用 java的类名  需要使用 XXX::class.java 来调用 kotlin的类 就不需要 .java 来区别对待了
    println(hello::class.java.simpleName)

    // 调用java的函数或者变量为 kotlin的关键字，需要使用 反单引号进行处理
    println(HelloJava().`in`)

    //list 的打印
    var list = ArrayList<Int>()
    list.add(1)
    list.add(11)
    list.add(12)
    // 可以直接使用lambda 进行 打印
    list.let { e -> e + 2 }.filter { it > 10 }.map { e -> e * e }.forEach { println(it) }


    // 可以使用 类似 python 的写法
    for (i in 1..2) {
        print(i).apply { print(" ") }
    }

    // 参数为函数的调用方式
    println(lock(ReentrantLock(), 1) { "who$it" })
    // 如果参数和返回值和某一个函数声明的方式是一样的则可以直接调用
    println(lock(ReentrantLock(), 1, ::testClosure))

    println(testClassName(HelloJava::class.java))
    println(testClassName(KotlinClass::class))

    // 调用Kotlin 类里面的静态函数
    KotlinClass.StringUtils.isEmpty("")
    //去查看 java 怎么调用kotlin里面的静态函数
}


// Any 就相当于 Object 了
// 可以为 函数参数设置默认值
fun dowork(caseVal: Any = 1) {
    // case 的写法 不用关心 有多种不同的类型了
    when (caseVal) {
        1 -> println("1 哦")
        is Long -> println("Long ")
        "String" -> println("String")
        is String -> println("is String:${caseVal.length}") // 可以直接使用方法
        !is String -> println("not is String")
        else -> println()
    }
}

fun hasEmpty(vararg strArray: String?): Boolean {
    for (str in strArray) {
        str ?: return true
    }
    return false
}

/**
 * 调用的方式为 mykotlin.t2.lock (){} 这种方式哦，看起来就跟函数声明差不多（java）
 */
fun <T> lock(lock: Lock, value: Int, body: (i: Int) -> T): T {
    lock.lock()
    try {
        return body(1)  // 这里可以把函数直接丢进来，函数的类型是 ()->T
    } finally {
        lock.unlock()
    }
}
// 可以直接作为 上面的函数的 方法参数
fun testClosure(value: Int): String {
    return "who $value"
}

fun <T> testClassName(clazz: Class<T>):String= clazz.simpleName
fun <T : Any> testClassName(clazz: KClass<T>): String? = clazz.simpleName