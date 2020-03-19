## val vs var
```Kotlin
    // String 和String? 时两个不同的类型
    // var 和 val 表示 可变变量 和 不可变变量
    var name: String? = null
    var name1: String
```
## `?:` `?.`
```Kotlin
    var hello: HelloJava? = null          // 只有变量为 ? 这个类型的才可以赋值null
    name = hello?.java("name")     // 不为空的时候会执行后面
    hello = HelloJava()             // 实例化 可以不用写 new 来实例化一个对象
    hello ?: return                   // 为空的时候会执行后面

    name = hello?.java("name")   
    name1 = name!!              // !! 表示强转
```
## 关键字
```kotlin
    // 调用java的函数或者变量为 kotlin的关键字，需要使用 反单引号进行处理
    println(HelloJava().`in`)
```
## lambda
```kotlin
    //list 的打印
    var list = ArrayList<Int>()
    list.add(1)
    list.add(11)
    list.add(12)
    // 可以直接使用lambda 进行 打印
    list.let { e -> e + 2 }.filter { it > 10 }.map { e -> e * e }.forEach { println(it) }

```
## ..
```kotlin
    // 可以使用 类似 python 的写法
    for (i in 1..2) {
        print(i).apply { print(" ") }
    }
```
## 闭包参数
```kotlin
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

    // 参数为函数的调用方式
    println(lock(ReentrantLock(), 1) { "who$it" })
    // 如果参数和返回值和某一个函数声明的方式是一样的则可以直接调用
    println(lock(ReentrantLock(), 1, ::testClosure))
```
## switch vs when
```kotlin
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
```
## vararg
```kotlin
fun hasEmpty(vararg strArray: String?): Boolean {
    for (str in strArray) {
        str ?: return true
    }
    return false
}
```
## java类 VS Kotlin类
```kotlin
// 声明的函数为Class 类型
fun <T> testClassName(clazz: Class<T>):String= clazz.simpleName
// 声明的函数为KClass 类型  表明是个kotlin的类
fun <T : Any> testClassName(clazz: KClass<T>): String? = clazz.simpleName

    //kotlin调用Java的类调用方式
    println(testClassName(HelloJava::class.java))
    println(testClassName(KotlinClass::class))
```
## kotlin 包级别函数改类名 @file:JvmName("Example")
```kotlin
@file:JvmName("Example")
//其他代码
```
### 静态函数 @JvmStatic
```kotlin
class KotlinClass {
    object StringUtils {
        @JvmStatic fun isEmpty(str: String): Boolean {
            return "" == str
        }
    }
}
```
## 调用静态函数
```kotlin
// 调用Kotlin 类里面的静态函数
KotlinClass.StringUtils.isEmpty("")
//去查看 java 怎么调用kotlin里面的静态函数
```
```java
// java 调用 kotlin 的静态方法
System.out.println(KotlinClass.StringUtils.INSTANCE.isEmpty2("Kotlin"));
```

## java调用kotlin函数
```java
    // 调用 kotlin 的包级别函数
    // 默认会在文件名称+Kt的类名下
    //
    String lock = Example.lock(new ReentrantLock(), 1, integer -> "" + integer);
```