class HelloJava private constructor(var mA: Int) {

    companion object {
        @Volatile
        var instance: HelloJava? = null
        fun getInstance(a: Int): HelloJava? {
            instance?:
                synchronized(this) {
                    instance ?: HelloJava(a).also { instance = it }
                }

            if (instance == null) {
                synchronized(HelloJava::class.java) {
                    if (instance == null) {
                        instance = HelloJava(a)
                    }
                }
            }
            return instance
        }
    }

}