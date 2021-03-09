public class HelloJava {
    public static volatile  HelloJava instance;
    public int mA;
    private HelloJava(int a){
        mA = a;
    }
    public static HelloJava getInstance(int a){
        if(instance == null){
            synchronized (HelloJava.class){
                if(instance == null){
                    instance = new HelloJava(a);
                }
            }
        }
        return instance;
    }
}
