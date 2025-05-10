# Imports for graph generation, pdf report generation, and pdf2png conversion
import matplotlib.pyplot as plt
from reportlab.lib.pagesizes import letter
from reportlab.pdfgen import canvas
from pdf2image import convert_from_path
import fitz  # PyMuPDF
import sys, os
from datetime import datetime

# Global constant values
graph_width = 500
graph_height = 300
dpi = 100

# Parse the given file_path for the user information
def get_user(file_path):
  # Get file contents
  contents = ""
  with open(file_path) as file:
      contents = file.read()

  user = {}
  count = 0

  # Search for only user info
  for line in contents.split("\n"):
    if line == "Accounts":
      break

    if line == "User":
      continue

    # Get user info
    match count:
      case 0: user["firstName"] = line.split("=")[1]
      case 1: user["lastName"] = line.split("=")[1]
      case 2: user["username"] = line.split("=")[1]
      case 3: user["email"] = line.split("=")[1]

    count += 1

  return user

# Parse given file for accounts filtered by the unix timestamp start and end
def get_accounts(file_path, start, end):
  # Get file contents
  contents = ""
  with open(file_path) as file:
    contents = file.read()

  # Parse the contents for accounts
  accounts = []
  parse = False
  count = 0
  account = {}
  for line in contents.split("\n"):
    if line == "Transactions":
      if account.__len__() > 0: # Edge case
        # Time check if needed
        time = int(datetime.strptime(account["timeCreated"][:10], "%Y-%m-%d").timestamp())
        if start != "0" and time >= int(start) and time <= int(end): 
          accounts.append(account)
        else:
          accounts.append(account)
      break

    if line == "Accounts":
      parse = True
      continue

    if parse:
      if count == 4:
        # Time check if needed
        time = int(datetime.strptime(account["timeCreated"][:10], "%Y-%m-%d").timestamp())
        if start != "0" and time >= int(start) and time <= int(end):
          accounts.append(account)
        else:
          accounts.append(account)
        account = {}
        count = 0

      match count:
        case 0: account["name"] = line.split("=")[1]
        case 1: account["type"] = line.split("=")[1]
        case 2: account["amount"] = line.split("=")[1]
        case 3: account["timeCreated"] = line.split("=")[1]

      count += 1

  return accounts

# Parse given file for transactions filtered by the unix timestamp start and end
def get_transactions(file_path, start, end):
  # Get file contents
  contents = ""
  with open(file_path) as file:
      contents = file.read()

  transactions = []
  transaction = {}
  count = 0
  parse = False

  # Parse for transactions
  lines = contents.split("\n")
  lines.pop()
  for line in lines:
    if line == "Transactions":
      parse = True
      continue

    if parse:
      if count == 4:
        # Time check if needed
        time = int(datetime.strptime(transaction["timeCreated"][:10], "%Y-%m-%d").timestamp())
        if start != "0" and time >= int(start) and time <= int(end):
          transactions.append(transaction)
        else:
          transactions.append(transaction)
        transaction = {}
        count = 0

      match count:
        case 0: transaction["from"] = line.split("=")[1]
        case 1: transaction["to"] = line.split("=")[1]
        case 2: transaction["amount"] = line.split("=")[1]
        case 3: transaction["timeCreated"] = line.split("=")[1]

      count += 1
 
  # Time check if needed
  time = int(datetime.strptime(transaction["timeCreated"][:10], "%Y-%m-%d").timestamp())
  if start != "0" and time >= int(start) and time <= int(end):
    transactions.append(transaction)
  else:
    transactions.append(transaction)
  return transactions

# Parse given file for goals filtered by the unix timestamp start and end
def get_goals(goals_path, start, end):
  # Get file contents
  contents = ""
  with open(goals_path) as file:
    contents = file.read()

  # Parse goals
  goals = []
  lines = contents.split("\n")
  lines.pop()
  for line in lines:
    goal = {}
    for idx, item in enumerate(line.split(",")):
      match idx:
        case 0: goal["name"] = item
        case 1: goal["amount"] = item
        case 2: goal["interestPeriod"] = item
        case 3: goal["interestAmount"] = item
        case 4: goal["downpayment"] = item
        case 5: goal["priority"] = item
        case 6: goal["years"] = item
        case 7: goal["months"] = item
        case 8: goal["payedOff"] = item
        case 9: goal["timeCreated"] = item

    # Time check if needed
    time = int(datetime.strptime(goal["timeCreated"], "%Y-%m-%d %H:%M:%S").timestamp())
    if start != "0" and time >= int(start) and time <= int(end):
      goals.append(goal)
    else:
      goals.append(goal)

  return goals

# Uses matplotlib to generate a graph with a relation between accounts and transactions
# and saves it to the output path as a .png
def generate_graph(accounts, transactions, output_path):
  x_values = list(range(len(transactions)+1))

  # Extract 'raw' data from accounts and transactions
  data = {}
  for account in accounts:
    name = account["name"]
    balance_over_time = [0.0]
    current_balance = 0.0

    for transaction in transactions:
      if transaction["from"] == name:
        current_balance -= float(transaction["amount"])
      elif transaction["to"] == name:
        current_balance += float(transaction["amount"])
      balance_over_time.append(current_balance)

    data[name] = balance_over_time

  image_path = f"{output_path}/graph.png"

  # Plotting the data
  plt.figure(figsize=(graph_width/dpi, graph_height/dpi), dpi=dpi)

  for account_name, balances in data.items():
    plt.plot(x_values, balances, marker="o", label=account_name)

  plt.title("Account Balances Over Transactions")
  plt.xlabel("Transaction Index")
  plt.ylabel("Balance")
  plt.grid(True)
  plt.legend()
  plt.tight_layout()
  plt.savefig(image_path)
  plt.close()

  return image_path

# Uses matplotlib to generate a bar graph with of all goals and their completion
# and saves it to the output path as a .png
def generate_goals_graph(goals, output_path):
  goal_names = []
  completeness = []

  # Get goal completness ralative to goal names
  for goal in goals:
    name = goal.get("name")
    try:
      amount = float(goal.get("amount"))
      payed_off = float(goal.get("payedOff"))
      percent_complete = (payed_off / amount) if amount > 0 else 0
    except ValueError:
      amount = 0
      payed_off = 0
      percent_complete = 0

    goal_names.append(name)
    completeness.append(percent_complete)

  # Plot the data
  plt.figure(figsize=(graph_width/dpi, graph_height/dpi), dpi=dpi)

  bars = plt.bar(goal_names, completeness, color="skyblue")
  plt.ylim(0, 1)
  plt.ylabel("Completeness (Paid / Total)")
  plt.title("Goal Completion Status")

  for bar, percent in zip(bars, completeness):
    plt.text(bar.get_x() + bar.get_width() / 2, bar.get_height() + 0.02, f"{percent:.0%}", ha="center", va="bottom", fontsize=10)

  plt.xticks(rotation=45, ha="right")
  plt.tight_layout()

  image_path = f"{output_path}/goals-graph.png"
  plt.savefig(image_path)
  plt.close()

  return image_path

# Uses pdf2png to convert a .pdf to a .png
def convert_pdf_to_png(pdf_path, output_path):
  doc = fitz.open(pdf_path)
  page = doc.load_page(0)
  pix = page.get_pixmap(dpi=200)
  pix.save(output_path)

# main entry point
def main():
  # Get command line arguments
  file_path = sys.argv[1]
  name = sys.argv[2]
  output_path = sys.argv[3]
  goals_path = sys.argv[4]
  transaction_graph = sys.argv[5]
  goal_graph = sys.argv[6]
  start = sys.argv[7]
  end = sys.argv[8]

  # Get all need user data
  user = get_user(file_path)
  accounts = get_accounts(file_path, start, end)
  transactions = get_transactions(file_path, start, end)
  goals = get_goals(goals_path, start, end)

  # Process some data, i.e make graphs, format, etc...
  graph_path = generate_graph(accounts, transactions, "../../data/reports")

  f_first_name = str.capitalize(user["firstName"])
  f_last_name = str.capitalize(user["lastName"])
  f_last_init = str.upper(user["lastName"][0])

  pdf_path = f"{output_path}{name}.pdf"

  # Begin building the pdf
  c = canvas.Canvas(pdf_path, pagesize=letter)
  width, height = letter

  # Header / title section
  title = f"{f_first_name}, {f_last_init}'s Report"
  c.setFont("Helvetica-Bold", 16)
  c.drawString((width/2)-c.stringWidth(title)/2, height-50, title)

  info_name = f"Name: {f_first_name}, {f_last_name}"
  info_username = f"Username: {user["username"]}"
  info_email = f"Email: {user["email"]}"

  c.setFont("Helvetica", 15)
  c.drawString(10, height-100, info_name)
  c.drawString(10, height-120, info_username)
  c.drawString(10, height-140, info_email)

  # Accounts section
  account_title = "Accounts"
  c.setFont("Helvetica-Bold", 16)
  c.drawString((width/2)-c.stringWidth(account_title)/2, height-200, account_title)

  y = height - 240
  for account in accounts:
    if y < 100:
      c.showPage()
      y = height - 100

    account_name = f"{account["name"]} - Type: {account["type"]}"
    account_created = f"Created: {account["timeCreated"]}"

    c.setFont("Helvetica-Bold", 16)
    c.drawString(10, y, account_name)

    y -= 20
    c.setFont("Helvetica", 15)
    c.drawString(20, y, account_created)

    for transaction in transactions:
      if transaction["from"] == account["name"]:
        account["amount"] = str(float(account["amount"]) - float(transaction["amount"]))
      elif transaction["to"] == account["name"]:
        account["amount"] = str(float(account["amount"]) + float(transaction["amount"]))

    y -= 20
    account_balance = f"Balance: {account["amount"]}"
    c.drawString(20, y, account_balance)

    y -= 40

  # Will wrap to a new page if needed
  if y-graph_height < 50:
    c.showPage()
    y = height - 50

  # Only show the graph is told to
  if transaction_graph == "True":
    c.drawImage(graph_path, (width/2)-(graph_width/2), y-graph_height)

  # Start with the goals now
  c.showPage()
  y = height - 100

  # Show the goals graph if told to
  graph_path = generate_goals_graph(goals, "../../data/reports")

  goals_title = "Goals"
  c.setFont("Helvetica-Bold", 16)
  c.drawString((width/2)-c.stringWidth(goals_title)/2, y, goals_title)
  y -= 50

  if goal_graph == "True":
    c.drawImage(graph_path, (width/2)-(graph_width/2), y-graph_height)
  c.save()

  convert_pdf_to_png(pdf_path, f"../../data/reports/{name}.png")
  print(f"{output_path}{name}.pdf")

# Runnable script
if __name__ == "__main__":
  main()
