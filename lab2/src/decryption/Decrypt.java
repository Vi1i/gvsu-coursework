package decryption;


import javax.crypto.Cipher;
import javax.crypto.CipherInputStream;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.nio.file.Files;
import java.nio.file.Paths;

/**
 * Created by lawrence on 9/22/15.
 */
public class Decrypt {
    private FileInputStream   etf;
    private FileOutputStream  ctf;
    private CipherInputStream cis;

    private byte[]          iv;
    IvParameterSpec         params;

    private SecretKey       key;


    private Cipher cipher;

    public Decrypt(String cipherTextFile)
    {
        try {
            // Creates a file stream from the encrypted file
            this.etf = new FileInputStream(cipherTextFile);

            String clearTextFile = cipherTextFile + ".dec";

            // Creates a file stream to the new clear text file
            this.ctf = new FileOutputStream(clearTextFile);
        } catch (java.io.FileNotFoundException e) {
            System.out.println(e.getMessage());
            e.printStackTrace(System.out);
            System.exit(1);
        }

        try {
            // Creates a new ciher with AES/CBC/PKCS5Padding settings
            this.cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
        }catch (java.security.NoSuchAlgorithmException e) {
            System.out.println(e.getMessage());
            e.printStackTrace(System.out);
            System.exit(1);
        }catch(javax.crypto.NoSuchPaddingException e) {
            System.out.println(e.getMessage());
            e.printStackTrace(System.out);
            System.exit(1);
        }

        try {
            // Reads in all data of IV
            this.iv = Files.readAllBytes(Paths.get("iv"));

            // Reads in all data of key and recreates the key
            this.key = new SecretKeySpec(Files.readAllBytes(Paths.get("key")), "AES");
        }catch(java.io.IOException e) {
            System.out.println(e.getMessage());
            e.printStackTrace(System.out);
            System.exit(1);
        }

    }

    public void decrypt() {
        try {
            // Initial the cipher into decrypt mode with the key and iv
            this.cipher.init(Cipher.DECRYPT_MODE, this.key, new IvParameterSpec(this.iv));
        }catch(java.security.InvalidKeyException e) {
            System.out.println(e.getMessage());
            e.printStackTrace(System.out);
            System.exit(1);
        }catch(java.security.InvalidAlgorithmParameterException e) {
            System.out.println(e.getMessage());
            e.printStackTrace(System.out);
            System.exit(1);
        }
        // Creates the cipher stream to decrypt
        this.cis = new CipherInputStream(this.etf, this.cipher);

        byte[] block = new byte[32];  // Block read size
        int i;
        try {
            // Reads the encrypted file in
            while ((i = cis.read(block)) != -1) {
                // Writes the file out
                this.ctf.write(block, 0, i);
            }
            this.ctf.close();
        }catch(java.io.IOException e){
            System.out.println(e.getMessage());
            e.printStackTrace(System.out);
            System.exit(1);
        }
    }

    public static void main(String[] args){
        System.out.println(args[0]);
        Decrypt dec = new Decrypt(args[0]);
        dec.decrypt();
    }
}