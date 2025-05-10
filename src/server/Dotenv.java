import java.io.*;

/* Dotenv class
 * 
 * Used to parse .env files
 */
class Dotenv
{
  // Constructor to load .env file, assumes the file is in the current directory
  public Dotenv()
  {
    String filePath = ".env";
    try (BufferedReader reader = new BufferedReader(new FileReader(filePath)))
    {
      String line;
      while ((line = reader.readLine()) != null)
      {
        String[] parts = line.split("=");
        System.setProperty(parts[0], parts[1]);
      }
    }
    catch (IOException err)
    {
      System.out.println("Dotenv Error: "+err);
    }
  }

  // Constructor to load .env file, takes direct filePath
  public Dotenv(String filePath)
  {
    try (BufferedReader reader = new BufferedReader(new FileReader(filePath)))
    {
      String line;
      while ((line = reader.readLine()) != null)
      {
        String[] parts = line.split("=");
        System.setProperty(parts[0], parts[1]);
      }
    }
    catch (IOException err)
    {
      System.out.println("Dotenv Error: "+err);
    }
  }

  // Gets the value
  public String GetValue(String key)
  {
    return System.getProperty(key);
  }
}
