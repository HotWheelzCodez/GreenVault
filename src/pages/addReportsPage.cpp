#include "../../include/pages/addReportsPage.hpp"

void AddReportsPage::Init(float windowWidth, float windowHeight)
{
  // Load logo
  Texture2D logo = LoadTexture("assets/images/vault.png");
  logo.width >>= 1;
  logo.height >>= 1;

  // Animation error keyframes
  std::vector<KeyFrame> errorAnimationKeyframes = { 
    { { 0.5f*windowWidth-(0.25f*windowWidth), -0.12f*windowHeight, 0.5f*windowWidth, 0.1f*windowHeight }, WHITE, 0.0f, 0.0f },
    { { 0.5f*windowWidth-(0.25f*windowWidth), 0.11f*windowHeight, 0.5f*windowWidth, 0.1f*windowHeight }, WHITE, 0.0f, 0.25f },
  };

  // Error animation
  Animation errorAnimation = Animation(errorAnimationKeyframes);

  // Button style(s)
  GUI::ButtonStyle backButtonStyle = { TRANSPARENT, WHITE, TRANSPARENT, TRANSPARENT, GREEN, TRANSPARENT, font, 75, GUI::TEXT_ALIGNMENT_CENTER, 0, 0, 0, false, 1 };
  Color darkGreen = { 0, 100, 0, 255 };
  Color lightGreen = { 0, 150, 0, 255 };
  GUI::ButtonStyle fileLocationStyle = { darkGreen, WHITE, darkGreen, lightGreen, WHITE, darkGreen, font, 25, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 0.1f, 0.2f, true, 1.0f };

  Color alphaGreen = GREEN;
  alphaGreen.a = 100;
  GUI::ButtonStyle submitButtonStyle = { TRANSPARENT, WHITE, SEA_GREEN, alphaGreen, WHITE, GREEN, font, 25, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 0.1f, 0.2f, false, 1.05f };

  GUI::CheckboxStyle checkboxStyle = { TRANSPARENT, SEA_GREEN, SEA_GREEN, 0.1f };

  GUI::Button backButton = GUI::Button(Rectangle{ 0, 0, 0.1f*windowWidth, 0.1f*windowHeight }, backButtonStyle, "<- Back");
  GUI::Button fileLocationButton = GUI::Button(Rectangle{ 0.55f*windowWidth, 0.5f*windowHeight, 0.38f*windowWidth, 0.05f*windowHeight }, fileLocationStyle, "Save File Location");
  GUI::Button submitButton = GUI::Button(Rectangle{ 0.4f*windowWidth, 0.75f*windowHeight, 0.3f*windowWidth, 0.05f*windowHeight }, submitButtonStyle, "Create Report!");

  GUI::ButtonStyle errorStyle = { RED, WHITE, PINK, RED, WHITE, PINK, font, 30, GUI::TEXT_ALIGNMENT_CENTER, 0.25f, 2, 1.5f, false, 1 };
  GUI::Button errorButton = GUI::Button(Rectangle{ 0 }, errorStyle, "");

  // Text style(s)
  GUI::TextStyle headerStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 150, GUI::TEXT_ALIGNMENT_CENTER, { 0 } };
  GUI::TextStyle fileLocationTextStyle = { TRANSPARENT, WHITE, TRANSPARENT, WHITE, font, 20, GUI::TEXT_ALIGNMENT_LEFT, { 0 } };

  GUI::Text header = GUI::Text(Rectangle{ 0, 0.2f*windowHeight, windowWidth, 0.1f*windowHeight }, headerStyle, "Create a Report");
  GUI::Text fileLocationText = GUI::Text(Rectangle{ 0.55f*windowWidth, 0.5f*windowHeight+(0.05f*windowHeight), 0.45f*windowWidth, 0.1f*windowHeight }, fileLocationTextStyle, "");

  // Input style(s)
  GUI::InputStyle itemNameStyle = { TRANSPARENT, SEA_GREEN, WHITE, GRAY, TRANSPARENT, SEA_GREEN, WHITE, GRAY, TRANSPARENT, GREEN, WHITE, BLUE, font, 30, 0.25f, 0.1f, 0.2f, false, false };

  GUI::Input reportNameInput = GUI::Input(Rectangle{ 0.15f*windowWidth, 0.5f*windowHeight, 0.38f*windowWidth, 0.05f*windowHeight }, itemNameStyle, "Report name...");

  GUI::InputStyle dateInputStyle = itemNameStyle;
  dateInputStyle.fontSize *= 0.75f;
  GUI::Input reportStartInput = GUI::Input(Rectangle{ 0.15f*windowWidth, 0.65f*windowHeight, 0.38f*windowWidth, 0.05f*windowHeight }, dateInputStyle, "Start period... (leave empty to get all dates) format YYYY/MM/DD");
  GUI::Input reportEndInput = GUI::Input(Rectangle{ 0.55f*windowWidth, 0.65f*windowHeight, 0.38f*windowWidth, 0.05f*windowHeight }, dateInputStyle, "End period... (leave empty to get all dates) format YYYY/MM/DD");

  // Checkboxes
  GUI::Checkbox checkboxTransactionsGraph({ 0.15f*windowWidth, 0.6f*windowHeight, 0.025f*windowWidth, 0.025f*windowWidth }, checkboxStyle);
  GUI::Checkbox checkboxGoalsGraph({ 0.55f*windowWidth, 0.6f*windowHeight, 0.025f*windowWidth, 0.025f*windowWidth }, checkboxStyle);

  GUI::Text checkboxInfo = GUI::Text({ 0 }, fileLocationTextStyle, "");

  // Load all components into the GUI 'structure'
  buttonElements["back"] = backButton;
  buttonElements["fileLocation"] = fileLocationButton;
  buttonElements["submit"] = submitButton;
  buttonElements["error"] = errorButton;

  textElements["header"] = header;
  textElements["fileLocation"] = fileLocationText;
  textElements["checkboxInfo"] = checkboxInfo;

  inputElements["reportNameInput"] = reportNameInput;
  inputElements["reportStartInput"] = reportStartInput;
  inputElements["reportEndInput"] = reportEndInput;

  checkboxElements["addTransactionsGraph"] = checkboxTransactionsGraph;
  checkboxElements["addGoalsGraph"] = checkboxGoalsGraph;

  images["logo"] = logo;

  animations["error"] = errorAnimation;
}

int AddReportsPage::Render(float windowWidth, float windowHeight, GUI::MouseState& mouseState)
{
  if (init)
  {
    Init(windowWidth, windowHeight);
    init = false;
  }

  if (buttonElements["back"].UpdateAndRender(mouseState))
    return 3; // Main page

  // Unselect all input boxes when mouse pressed
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
  {
    for (auto it = inputElements.begin(); it != inputElements.end(); it++)
      it->second.SetSelected(false);
  }

  // Render GUI 'structure'

  DrawTexture(images["logo"], (0.5f*windowWidth)-(images["logo"].width/2.0f), 0.05f*windowHeight, WHITE);

  textElements["header"].UpdateAndRender(mouseState);

  inputElements["reportNameInput"].UpdateAndRender(mouseState);
  inputElements["reportStartInput"].UpdateAndRender(mouseState);
  inputElements["reportEndInput"].UpdateAndRender(mouseState);

  if (buttonElements["fileLocation"].UpdateAndRender(mouseState))
  {
    const char* filePath = tinyfd_selectFolderDialog("Your Save Location", NULL);

    if (filePath)
      textElements["fileLocation"].SetText(filePath);
  }

  textElements["fileLocation"].UpdateAndRender(mouseState);

  checkboxElements["addTransactionsGraph"].UpdateAndRender(mouseState);
  checkboxElements["addGoalsGraph"].UpdateAndRender(mouseState);
  
  textElements["checkboxInfo"].SetBounds({ 0.15f*windowWidth+(0.03f*windowWidth), 0.6f*windowHeight+(0.01f*windowHeight), 0.9f*windowWidth, 0.1f*windowHeight });
  textElements["checkboxInfo"].SetText("Add graph for spending");
  textElements["checkboxInfo"].UpdateAndRender(mouseState);

  textElements["checkboxInfo"].SetBounds({ 0.55f*windowWidth+(0.03f*windowWidth), 0.6f*windowHeight+(0.01f*windowHeight), 0.9f*windowWidth, 0.1f*windowHeight });
  textElements["checkboxInfo"].SetText("Add graph for goal(s) completetion");
  textElements["checkboxInfo"].UpdateAndRender(mouseState);

  if (buttonElements["submit"].UpdateAndRender(mouseState) || IsKeyPressed(KEY_ENTER))
  {
    std::string name = inputElements["reportNameInput"].GetText();
    std::string savePath = textElements["fileLocation"].GetText();

    // Input sanatazation
    if (name.empty())
    {
      etc["errorMessage"] = "No name given!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;
    }
    if (savePath.empty())
    {
      etc["errorMessage"] = "No file path given!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;
    }

    if (!Database::GetAccounts().size())
    {
      etc["errorMessage"] = "No accounts exit, so no report to make!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;
    }
    if (!Database::GetTransactions().size())
    {
      etc["errorMessage"] = "No transactions exit, so no report to make!";
      animations["error"].Reset();
      animations["error"].Play();
      return 0;
    }

    bool valid = false;
    if (!inputElements["reportStartInput"].GetText().empty())
    {
      if (!Util::IsValidDate(inputElements["reportStartInput"].GetText()))
      {
        etc["errorMessage"] = "Start date is not valid!";
        animations["error"].Reset();
        animations["error"].Play();
        return 0;
      }
      if (!Util::IsValidDate(inputElements["reportEndInput"].GetText()))
      {
        etc["errorMessage"] = "End date is not valid!";
        animations["error"].Reset();
        animations["error"].Play();
        return 0;
      }
      valid = true;
    }

    std::string startValue = "0";
    std::string endValue = "0";
    if (valid)
    {
      startValue = std::to_string(Util::GetUnixTimestamp(inputElements["reportStartInput"].GetText()));
      endValue = std::to_string(Util::GetUnixTimestamp(inputElements["reportEndInput"].GetText()));
    }
      
    name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
    savePath = savePath.substr(0, savePath.length()-1);

    // Get input and prepare it to be sent to the server
    bool transactionsGraph = checkboxElements["addTransactionsGraph"].GetChecked();
    bool goalsGraph = checkboxElements["addGoalsGraph"].GetChecked();

    std::string transactionsStr = transactionsGraph ? "True" : "False";
    std::string goalsStr = goalsGraph ? "True" : "False";

    std::string end = "?name="+name+"&output="+savePath+"&transaction_graph="+transactionsStr+"&goal_graph="+goalsStr+"&start="+startValue+"&end="+endValue;
    std::string url = "http://127.0.0.1:8080/generate-report"+end;

    // Update local data to reflect server actions
    Database::ServerResponse serverResponse = Database::GetServerResponse(url);
    if (serverResponse.success)
    {
      std::fstream file("data/reports.djb", std::ios::app);
      file << serverResponse.response << '\n';
      file.close();
    }

    DrawRectangle(0, 0, windowWidth, windowHeight, THEME_COLOR);
    DeInit();
    return 3; // Main page
  }

  // Display / play error animation
  animations["error"].Animate();
  Rectangle errorAnimationBounds = animations["error"].GetCurrentBounds();

  buttonElements["error"].SetText(etc["errorMessage"]);
  buttonElements["error"].SetBounds(errorAnimationBounds);

  if (buttonElements["error"].UpdateAndRender(mouseState))
    animations["error"].Reset();

  return 0;
}
