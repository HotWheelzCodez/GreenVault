import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;
import java.nio.charset.StandardCharsets;
//import java.io.*;
import java.nio.file.*;
import java.util.Base64;

/* Encryption class
 * 
 * Used to encrypt and decrypt information using AES-256 encryption
 */
public class Encryption
{
  private static String KEY_FILE = "key.txt";

  // Read the key information from a file, or generate one if one does not exist
  public static SecretKey GetKey()
  {
    try
    {
      if (Files.exists(Paths.get(KEY_FILE)))
      {
        String encodedKey = Files.readString(Paths.get(KEY_FILE));
        byte[] decodedKey = Base64.getDecoder().decode(encodedKey);
        return new SecretKeySpec(decodedKey, "AES");
      }
      else
      {
        KeyGenerator keyGenerator = KeyGenerator.getInstance("AES");
        keyGenerator.init(256);
        SecretKey secretKey = keyGenerator.generateKey();
        String encodedKey = Base64.getEncoder().encodeToString(secretKey.getEncoded());

        Files.write(Paths.get(KEY_FILE), encodedKey.getBytes());
        return secretKey;
      }
    }
    catch (Exception err)
    {
      System.err.println("Encryption Error - Key: "+err);
    }

    return null;
  }

  // Use the key to encrypt the 'plainText' string
  public static String Encrypt(String plainText)
  {
    try 
    {
      Cipher cipher = Cipher.getInstance("AES");
      cipher.init(Cipher.ENCRYPT_MODE, GetKey());
      byte[] encryptedBytes = cipher.doFinal(plainText.getBytes(StandardCharsets.UTF_8));
      return Base64.getEncoder().encodeToString(encryptedBytes);
    } 
    catch (Exception err) 
    {
      System.err.println("Encryption Error: "+err);
    } 
    return "";
  }

  // Use the key to decrypt the 'encryptedText' string
  public static String Decrypt(String encryptedText)
  {
    try
    {
      Cipher cipher = Cipher.getInstance("AES");
      cipher.init(Cipher.DECRYPT_MODE, GetKey());
      byte[] decodedBytes = Base64.getDecoder().decode(encryptedText);
      byte[] decryptedBytes = cipher.doFinal(decodedBytes);
      return new String(decryptedBytes, StandardCharsets.UTF_8);
    }
    catch (Exception err)
    {
      System.err.println("Decryption Error: "+err);
    }
    return "";
  }
}