package myjava;

import com.google.protobuf.AbstractMessage;
import com.google.protobuf.Descriptors;
import com.google.protobuf.Message;
import muduo.TestPb;

import java.lang.reflect.Method;
import java.util.Iterator;
import java.util.Map;

public class HellpPb {
    public static void main(String[] args) {

//        byte []   -> pb<T>
        TestPb.Query build = TestPb.Query.newBuilder().setINT32(1).build();

    }

//    public static <T extends Message> T toPb(byte[] bytes)
//    {
//
//        return new Message.Builder().mergeFrom(data).buildParsed();
//    }

}
