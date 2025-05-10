import java.util.ArrayList;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import java.nio.file.*;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

/* Sync class
 * 
 * Used to syncrhonize locally stored data with the remote database
 */
public class Sync
{
  public Sync() {}

  public boolean Synchronize(Connection connection)
  {
    User user = new User();
    ArrayList<Account> accounts = new ArrayList<Account>();
    ArrayList<Transaction> transactions = new ArrayList<Transaction>();

    // Parse the localy stored user data file
    String filePath = "../../data/user.djb";
    try (BufferedReader reader = new BufferedReader(new FileReader(filePath)))
    {
      byte parseMode = 0; // Parse mode 0 = user, 1 = accounts, 2 = transactions
      Account accountTemp = new Account(); // Used as a temp value for parsing
      Transaction transactionTemp = new Transaction(); // Used as a temp value for parsing

      String line;
      while ((line = reader.readLine()) != null)
      {
        if (line.equals("User"))
          continue;

        if (line.equals("Accounts"))
        {
          parseMode = 1; // Now parse accounts;
          continue;
        }
        if (line.equals("Transactions"))
        {
          parseMode = 2; // Now parse transactions
          continue;
        }

        String[] lineData = line.split("="); // string[0] = field, string[1] = value
        switch (parseMode)
        {
          case 0:
            switch (lineData[0])
            {
              case "FirstName":
                user.firstName = lineData[1];
                break;
              case "LastName":
                user.lastName = lineData[1];
                break;
              case "Username":
                user.username = lineData[1];
              case "Email":
                user.email = lineData[1];
                break;
              case "Password":
                user.password = lineData[1];
                break;
            }
            break; 
          case 1:
            switch (lineData[0])
            {
              case "Name":
                accountTemp.name = lineData[1];
                break;
              case "Type":
                accountTemp.type = lineData[1];
                break;
              case "TimeCreated":
                accountTemp.timeCreated = lineData[1];
                accounts.add(accountTemp);
                accountTemp = new Account();
                break;
            }
            break;
          case 2:
            switch (lineData[0])
            {
              case "From":
                transactionTemp.from = lineData[1];
                break;
              case "To":
                transactionTemp.to = lineData[1];
                break;
              case "Amount":
                try
                {
                  double amount = Double.parseDouble(lineData[1]);
                  transactionTemp.amount = amount;
                }
                catch (NumberFormatException err)
                {
                  return false; 
                }
                break;
              case "TimeCreated":
                transactionTemp.timeCreated = lineData[1];
                transactions.add(transactionTemp);
                transactionTemp = new Transaction();
                break;
            }
        }
      }
    }
    catch (IOException err)
    {
      System.err.println("Syncrhonization Error: "+err);
    }

    // Get the user id
    String sqlQuery = "select id from users where username = ?;";
    int id = -1;
    try (PreparedStatement statement = connection.prepareStatement(sqlQuery))
    {
      statement.setString(1, user.username);

      try (ResultSet result = statement.executeQuery())
      {
        while (result.next()) { id = result.getInt("id"); }
      }
      catch (SQLException err)
      {
        System.err.println("SQL Syncrhonization Error: "+err);
      }

      System.out.println("ID: "+id);
    }
    catch (SQLException err)
    {
      System.err.println("SQL Syncrhonization Error: "+err);
    }

    if (id == -1)
      return false;

    // Process sql query to get remote accounts
    ArrayList<Account> remoteAccounts = new ArrayList<Account>();
    sqlQuery = "select * from accounts where user_id = ?";
    try (PreparedStatement statement = connection.prepareStatement(sqlQuery))
    {
      statement.setInt(1, id);

      try (ResultSet result = statement.executeQuery())
      {
        while (result.next())
        {
          String name = result.getString("name");
          String type = result.getString("type");
          String timeCreated = result.getString("time_created");

          Account remoteAccount = new Account(name, type, timeCreated);
          remoteAccounts.add(remoteAccount);
        }
      }
      catch (SQLException err)
      {
        System.err.println("SQL Syncrhonization Error: "+err);
      }
    }
    catch (SQLException err)
    {
      System.err.println("SQL Syncrhonization Error: "+err);
    }

    // Update remote accounts based on local accounts
    // Adding local accounts to remote accounts
    for (Account localAccount : accounts)
    {
      boolean exists = false;
      for (Account remoteAccount : remoteAccounts)
      {
        if (localAccount.equals(remoteAccount))
        {
          exists = true;
          break;
        }
      }

      if (exists)
        continue;

      remoteAccounts.add(localAccount);
    }

    // Removing remote accounts that no longer exists
    for (Account remoteAccount : remoteAccounts)
    {
      boolean exists = false;
      for (Account localAccount : accounts)
      {
        if (remoteAccount.equals(localAccount))
        {
          exists = true;
          break;
        }
      }

      if (exists)
        break;

      remoteAccounts.remove(remoteAccount);
    }

    // Process sql query to add need acounts
    sqlQuery = "insert into accounts (name, type, time_created) values (?, ?. ?) where user_id = ?";
    try (PreparedStatement statement = connection.prepareStatement(sqlQuery))
    {
      for (Account remoteAccount : remoteAccounts)
      {
        statement.setString(1, remoteAccount.name);
        statement.setString(2, remoteAccount.type);
        statement.setString(3, remoteAccount.timeCreated);
        statement.setInt(4, id);
        statement.execute();
      }
    }
    catch (SQLException err)
    {
      System.err.println("SQL Syncrhonization Error: "+err);
    }

    return true;
  }

  // Acts as struct
  private class User
  {
    public User() { }
    public User(String firstName, String lastName, String username, String email, String password)
    {
      this.firstName = firstName;
      this.lastName = lastName;
      this.username = username;
      this.email = email;
      this.password = password;
    }
      
    public String firstName;
    public String lastName;
    public String username;
    public String email;
    public String password;
  }

  // Acts as struct
  private class Account 
  {
    public Account() { }
    public Account(String name, String type, String timeCreated)
    {
      this.name = name;
      this.type = type;
      this.timeCreated = timeCreated;
    }

    public String name;
    public String type;
    public String timeCreated;
  }

  // Acts as struct
  private class Transaction
  {
    public Transaction() { }
    public Transaction(String from, String to, double amount, String timeCreated)
    {
      this.to = to;
      this.from = from;
      this.amount = amount;
      this.timeCreated = timeCreated;
    }

    public String from;
    public String to;
    public double amount;
    public String timeCreated;
  }
}
