package mykotlin.t3

fun test(){
    GlobalScope.launch {
        // it doesn't block a thread, but only suspends the coroutine itself
        sleep(1000)
        println("协程1：线程id: ${Thread.currentThread().id} current time:" + (SystemClock.elapsedRealtimeNanos() - start) / 1000_1000)
    }
    GlobalScope.launch {
        // it doesn't block a thread, but only suspends the coroutine itself
        sleep(1000)
        println("协程2：线程id: ${Thread.currentThread().id} current time:" + (SystemClock.elapsedRealtimeNanos() - start) / 1000_1000)
    }
}