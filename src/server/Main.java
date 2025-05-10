import com.sun.net.httpserver.HttpServer;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpExchange;
import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.util.Map;
import java.util.stream.Collectors;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.nio.charset.StandardCharsets;

//import javax.crypto.Cipher;
//import javax.crypto.KeyGenerator;
//import javax.crypto.SecretKey;
//import javax.crypto.spec.SecretKeySpec;
//import java.util.Base64;

//import java.sql.BatchUpdateException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
//import java.sql.SQLIntegrityConstraintViolationException;
//import java.sql.Savepoint;
import java.sql.Statement;

/* Main class - entry point
 * 
 * This is used to startup the HTTP server and establish a secure connection to the remote
 * mysql database
 */
public class Main 
{
  private static final boolean DEBUG = false;
  private static Connection connection;

  public static void main(String[] args) throws IOException 
  {
    Dotenv dotenv = new Dotenv();

    // Start the HTTP server
    int port = 8080;
    HttpServer server = HttpServer.create(new InetSocketAddress(port), 0);
    server.createContext("/auth-user", new AuthUser());
    server.createContext("/health", new Health());
    server.createContext("/add-user", new AddUser());
    server.createContext("/user-status", new UserStatus());
    server.createContext("/decrypt", new Decrypt());
    server.createContext("/sync", new SyncManager());
    server.createContext("/generate-report", new ReportGenerator());
    server.createContext("/get-api-key", new GetAPIKey());
    server.setExecutor(null);
    server.start();

    String url = "jdbc:mysql://"+dotenv.GetValue("DB_DOMAIN")+":3306/"+dotenv.GetValue("DB_NAME")+"?connectTimeout=30000";
    try 
    {
      Class.forName("com.mysql.cj.jdbc.Driver");
      connection = DriverManager.getConnection(url, dotenv.GetValue("DB_USER"), dotenv.GetValue("DB_PASS"));
    } 
    catch (ClassNotFoundException | SQLException err) 
    {
      System.err.println("SQL Error: " + err);
    }
  }

  // Mangaes the data synchronization of local and remote data
  static class SyncManager implements HttpHandler
  {
    @Override
    public void handle(HttpExchange exchange) throws IOException
    {
      if ("GET".equals(exchange.getRequestMethod()))
      {
        Sync sync = new Sync();
        String response = "";

        if (sync.Synchronize(connection))
          response = "synchronized";
        else
          response = "not_synchronized";

        exchange.sendResponseHeaders(200, response.length());
        OutputStream os = exchange.getResponseBody();
        os.write(response.getBytes());
        os.close();
      }
      else
      {
        exchange.sendResponseHeaders(405, -1);
      }
    }
  }

  // Will get the user status 
  static class AuthUser implements HttpHandler 
  {
    @Override
    public void handle(HttpExchange exchange) throws IOException 
    {
      if ("GET".equals(exchange.getRequestMethod())) 
      {
        // Parse the query
        String query = exchange.getRequestURI().getQuery();
        Map<String, String> params = parseQuery(query);

        String givenUsername = params.get("username");
        String givenPassword = params.get("password");

        // Default response
        String response = "user_does_not_exist";

        try (Statement statement = connection.createStatement())
        {
          ResultSet result = statement.executeQuery("select * from users;");

          // Get the results
          while (result.next())
          {
            // Get result information
            String username = result.getString("username");
            String encryptedPassword = result.getString("password");

            // Determine user status
            if (username.equals(givenUsername))
            {
              String password = Encryption.Decrypt(encryptedPassword);
              if (password.equals(givenPassword))
              {
                response = "user_valid";
                break;
              }
              response = "password_incorrect";
              continue;
            }
            response = "user_does_not_exist";
          }
        } 
        catch (SQLException err)
        {
          System.err.println("SQL Error: " + err);
        }

        // Return response
        exchange.sendResponseHeaders(200, response.length());
        OutputStream os = exchange.getResponseBody();
        os.write(response.getBytes());
        os.close();
      } 
      else 
      {
        exchange.sendResponseHeaders(405, -1);
      }
    }
  }

  // Helper to get the status / health of the server
  static class Health implements HttpHandler 
  {
    @Override
    public void handle(HttpExchange exchange) throws IOException 
    {
      if ("GET".equals(exchange.getRequestMethod()))
      {
        String response = "server_is_up";

        exchange.sendResponseHeaders(200, response.length());
        OutputStream os = exchange.getResponseBody();
        os.write(response.getBytes());
        os.close();
      }
    }
  }

  // Used to take plain text, and return the decrypted text
  static class Decrypt implements HttpHandler 
  {
    @Override
    public void handle(HttpExchange exchange) throws IOException 
    {
      if ("GET".equals(exchange.getRequestMethod()))
      {
        String query = exchange.getRequestURI().getQuery();
        Map<String, String> params = parseQuery(query);

        String encryptedText = params.get("encrypted_text");
        String decryptedText = Encryption.Decrypt(encryptedText);
        String response = decryptedText;

        exchange.sendResponseHeaders(200, response.length());
        OutputStream os = exchange.getResponseBody();
        os.write(response.getBytes());
        os.close();
      }
    }
  }

  // Get's a user's status
  static class UserStatus implements HttpHandler 
  {
    @Override
    public void handle(HttpExchange exchange) throws IOException 
    {
      if ("GET".equals(exchange.getRequestMethod()))
      {
        // Parse the query
        String query = exchange.getRequestURI().getQuery();
        Map<String, String> params = parseQuery(query);

        // Default response
        String response = "not_exists";
        String givenUsername = params.get("username");

        // Process a sql query
        try (Statement statement = connection.createStatement())
        {
          ResultSet result = statement.executeQuery("select * from users;");

          while (result.next())
          {
            String username = result.getString("username");

            if (username.equals(givenUsername))
              response = "exists";
          }
        }
        catch (SQLException err)
        {
          System.err.println("SQL Exception - user status: "+err);
        }

        exchange.sendResponseHeaders(200, response.length());
        OutputStream os = exchange.getResponseBody();
        os.write(response.getBytes());
        os.close();
      }
    }
  }

  // Add's a user to the remote database
  static class AddUser implements HttpHandler 
  {
    @Override
    public void handle(HttpExchange exchange) throws IOException 
    {
      if ("GET".equals(exchange.getRequestMethod()))
      {
        // Parse query
        String query = exchange.getRequestURI().getQuery();
        Map<String, String> params = parseQuery(query);

        String response = "";

        // Get info
        String firstName = params.get("first_name");
        String lastName = params.get("last_name");
        String username = params.get("username");
        String email = params.get("email");
        String password = params.get("password");

        String encryptedPassword = Encryption.Encrypt(password);
        if (encryptedPassword.equals(""))
        {
          exchange.sendResponseHeaders(405, -1);
          return;
        }

        // Return the encrypted password
        response = encryptedPassword;

        // Early return if not database connection
        if (connection == null) {
          exchange.sendResponseHeaders(200, response.length());
          OutputStream os = exchange.getResponseBody();
          os.write(response.getBytes());
          os.close();
          return;
        }

        // Process mysql query to add user
        String sqlQuery = "insert into users(first_name, last_name, username, email, password) values(?, ?, ?, ?, ?);";
        try (PreparedStatement statement = connection.prepareStatement(sqlQuery, Statement.RETURN_GENERATED_KEYS))
        {
          statement.setString(1, firstName);
          statement.setString(2, lastName);
          statement.setString(3, username);
          statement.setString(4, email);
          statement.setString(5, encryptedPassword);

          statement.executeUpdate();
        }
        catch (SQLException err)
        {
          System.err.println("SQL Error: "+err);
        }

        exchange.sendResponseHeaders(200, response.length());
        OutputStream os = exchange.getResponseBody();
        os.write(response.getBytes());
        os.close();
      }
    }
  }

  // Spawns a python child process to generate a graph
  static class ReportGenerator implements HttpHandler 
  {
    @Override
    public void handle(HttpExchange exchange) throws IOException 
    {
      if ("GET".equals(exchange.getRequestMethod()))
      {
        // Parse query
        Map<String, String> params = parseQuery(exchange.getRequestURI().getQuery());
        String name = params.get("name");
        String outputPath = params.get("output");
        String transactionGraph = params.get("transaction_graph");
        String goalGraph = params.get("goal_graph");
        String start = params.get("start");
        String end = params.get("end");

        String response = new String();

        System.out.println("Parsed query");

        // Build process
        ProcessBuilder reportGenerator = new ProcessBuilder("python3", "../../scripts/reportGenerator.py", "../../data/user.djb", name, outputPath, "../../data/goals.djb", transactionGraph, goalGraph, start, end);
        try 
        {
          // Spawn process
          Process process = reportGenerator.start();
          System.out.println("Started process");

          // Read output
          BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
          String line;
          while ((line = reader.readLine()) != null) 
          {
            if (DEBUG)
            {
              System.out.println("[PYTHON] "+line);
            }
            else
            {
              response += line;
            }
          }
        } 
        catch (IOException err) 
        {
          err.printStackTrace();
        }

        exchange.sendResponseHeaders(200, response.length());
        OutputStream os = exchange.getResponseBody();
        os.write(response.getBytes());
        os.close();
      }
    }
  }

  // Read's the openai api key from the .env file
  static class GetAPIKey implements HttpHandler 
  {
    @Override
    public void handle(HttpExchange exchange) throws IOException 
    {
      if ("GET".equals(exchange.getRequestMethod()))
      {
        Dotenv dotenv = new Dotenv();
        String response = new String();

        String apiKey = dotenv.GetValue("API_KEY");
        response = apiKey;

        exchange.sendResponseHeaders(200, response.length());
        OutputStream os = exchange.getResponseBody();
        os.write(response.getBytes());
        os.close();
      }
    }
  }

  // Used to parse queries in the form of 'name=value&name_again=another_value'
  private static Map<String, String> parseQuery(String query) 
  {
    if (query == null || query.isEmpty()) return Map.of();
    return java.util.Arrays.stream(query.split("&"))
      .map(param -> param.split("="))
      .filter(pair -> pair.length == 2)
      .collect(Collectors.toMap(pair -> pair[0], pair -> pair[1]));
  }
}
