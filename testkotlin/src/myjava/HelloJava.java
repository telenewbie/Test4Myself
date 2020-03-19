package myjava;

import mykotlin.t1.KotlinClass;
import mykotlin.t2.Example; // 通过在Hello.kt 这个没有类的概念的头加上 `@file:JvmName("Example")` 即可更改默认以 文件名称+Kt的方式来命名的class文件

import java.util.concurrent.locks.ReentrantLock;

public class HelloJava {
    public final String in = "in";

    public String java(String name) {
        return name;
    }


    public static void main(String[] args) {
        System.out.println("java");
        // java 调用 kotlin 的静态方法
        System.out.println(KotlinClass.StringUtils.INSTANCE.isEmpty2("Kotlin"));
        // 调用 kotlin 的包级别函数
        // 默认会在文件名称+Kt的类名下
        String lock = Example.lock(new ReentrantLock(), 1, integer -> "" + integer);
        System.out.println(lock);
    }
}
