package mykotlin.t1

class KotlinClass {
    object Test {
        fun getInstance(): KotlinClass {
            return KotlinClass()
        }
    }
    object StringUtils {
        @JvmStatic fun isEmpty(str: String): Boolean {
            return "" == str
        }

        fun isEmpty2(str: String): Boolean {
            return "" == str
        }
    }
}