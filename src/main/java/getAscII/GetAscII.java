package getAscII;

import com.sun.jna.Library;
import com.sun.jna.Native;

import java.io.File;

public interface GetAscII extends Library {
    File dll = new File("./user/getAscII.dll");
    GetAscII INSTANCE = (GetAscII) Native.loadLibrary(dll.getAbsolutePath(), GetAscII.class);

    int getAscII(String str);
}