fun main() {
    println("hello1${"%.2f".format(1.0f)}")

    val numbers = listOf(212, 1)
    println("${numbers.size}")
//    val entrees = mutableListOf<String>()
    val entrees: MutableList<String> = mutableListOf()
    println("Add noodles: ${entrees.add("noodles")}")
    println("$entrees")
    val moreItems = listOf("ravioli", "lasagna", "fettuccine")
    println("Add list: ${entrees.addAll(moreItems)}")
    println("Entrees: $entrees")
    println("Add spaghetti: ${entrees.add("spaghetti")}")
    println("Entrees: $entrees")

//    entrees.add(10)
    for (item in 0 until entrees.size step 2) {
        println(entrees[item])
    }

    println(A1(1, "an"))

    AA3().draw()

    val person = Person("do")
    val jane = User("Jane", 35,person)
    val (name, age,myperson) = jane
    println("$name, $age years of age ,person = ${myperson.name}") // 输出 "Jane, 35 years of age"

    val a10:A10 = A10Impl(9)
    when(a10){
        is A10Impl -> {
            println(a10.name)
        }
        is A10obj ->{
            println(a10.test().testA())
//            a10.name= "123"
//            println(a10.name)
//            testA10(a10)
        }
    }
    var myA10:A10obj? = null
    println(myA10.abc())
    println()
}

abstract class D(private var res: Int) {
    abstract val build: String
    abstract val capacity: Int

    fun hasRandom(): Boolean {
        return res < capacity
    }
}

class S(private var r: Int, override val build: String, override val capacity: Int) : D(r) {

}

open class A1 {

    var s_name: String? = null
    var s_age: Int? = null

    constructor(name: String) {
        s_name = name
    }

    constructor(age: Int, name: String) : this(name) {
        s_age = age
    }
    open fun testA(){

    }

    override fun toString(): String {
        var content = "name:"
        content += s_name
        content += " age:"
        content += s_age ?: 1
        return content
    }
}

class A2 :A1(""){
override fun testA(){

    }
}

open class A3{
    open fun draw(){
        println("A3")
    }
}
interface IA3{
    fun draw(){
        println("IA3")
    }
}
class AA3: A3(), IA3 {
    override fun draw() {
        super<A3>.draw()
        super<IA3>.draw()
    }

}

data class Person(val name: String) {
    var age: Int = 0
}
data class User(val name: String = "", val age: Int = 0 ,var person: Person)

sealed class A10

data class A10Impl(var age:Int) : A10() {
    var name :String = "default"
}


fun String.testA(){
    println("$this testA")
}
fun Any?.abc() : String{
    if(this == null){
        return "null"
    }
    return  toString()
}

object A10obj :A10(){
    fun test():String{
        println("test")
        return "test"
    }
}

var stringRepresentation: String =""
    get() = field
    set(value) {
        field = value
    }
//var A10obj.name :String
//    get() = this.name
//    set(value) {
//        this.name = value
//    }

//fun testA10(a:A10obj){
//    println(a.name)
//    a.name = "10"
//    println(a.name)
//}

