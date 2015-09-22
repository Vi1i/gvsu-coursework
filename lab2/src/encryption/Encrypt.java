package encryption;

import javax.crypto.Cipher;
import javax.crypto.CipherOutputStream;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import java.io.FileInputStream;
import java.io.FileOutputStream;

/**
 * Created by lawrence on 9/22/15.
 */
public class Encrypt {
    private       SecretKey    key;
    private       KeyGenerator keyGen;
    private       Cipher       cipher;
    private byte[]             iv;


    FileInputStream    ctf;
    FileOutputStream   etf;
    CipherOutputStream cos;

    public Encrypt(String clearTextFile)
    {
        try {
            this.ctf = new FileInputStream(clearTextFile);
            String cipherTextFile = clearTextFile + ".aes";
            this.etf = new FileOutputStream(cipherTextFile);
        }catch(java.io.FileNotFoundException e) {
            System.out.println(e.getMessage());
            e.printStackTrace(System.out);
            System.exit(1);
        }

        try {
            this.cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
            this.keyGen = KeyGenerator.getInstance("AES");
        } catch (java.security.NoSuchAlgorithmException e) {
            System.out.println(e.getMessage());
            e.printStackTrace(System.out);
            System.exit(1);
        }catch(javax.crypto.NoSuchPaddingException e) {
            System.out.println(e.getMessage());
            e.printStackTrace(System.out);
            System.exit(1);
        }

        this.cos = new CipherOutputStream(this.etf, this.cipher);
    }

    public void createKey() {
        this.keyGen.init(128);
        this.key = keyGen.generateKey();
    }

    public void encrypt() {
        try {
            this.cipher.init(Cipher.ENCRYPT_MODE, this.key);
            this.iv = this.cipher.getIV();
        }catch(java.security.InvalidKeyException e){
            System.out.println(e.getMessage());
            e.printStackTrace(System.out);
            System.exit(1);
        }
        byte[] block = new byte[16];
        int i;
        try {
            while ((i = this.ctf.read(block)) != -1) {
                this.cos.write(block, 0, i);
            }
            this.cos.close();
        }catch(java.io.IOException e) {
            System.out.println(e.getMessage());
            e.printStackTrace(System.out);
        }
        //TODO Output IV
        try {
            FileOutputStream out = new FileOutputStream("iv");
            out.write(this.iv);
            out.close();
        }catch(java.io.FileNotFoundException e) {
            System.out.println(e.getMessage());
            e.printStackTrace(System.out);
            System.exit(1);
        }catch(java.io.IOException e) {
            System.out.println(e.getMessage());
            e.printStackTrace(System.out);
            System.exit(1);
        }
        //TODO Output the Key
        try {
            FileOutputStream out = new FileOutputStream("key");
            out.write(this.key.getEncoded());
            out.close();
        }catch(java.io.FileNotFoundException e) {
            System.out.println(e.getMessage());
            e.printStackTrace(System.out);
            System.exit(1);
        }catch(java.io.IOException e) {
            System.out.println(e.getMessage());
            e.printStackTrace(System.out);
            System.exit(1);
        }
    }

    public static void main(String[] args){
        System.out.println(args[0]);
        Encrypt enc = new Encrypt(args[0]);
        enc.createKey();
        enc.encrypt();
    }
}
