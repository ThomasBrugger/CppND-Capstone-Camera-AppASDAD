#include "App.h"
#include <fstream>
#include <cstdio>
#include <chrono>
#include <thread>

//Define WxEvents for MyFrame
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_CLOSE(MyFrame::OnClose)
EVT_MENU(App_Quit, MyFrame::OnQuit)
EVT_MENU(App_About, MyFrame::OnAbout)
EVT_MENU(App_Save, MyFrame::OnSave)
EVT_MENU(App_Load, MyFrame::OnLoad)
EVT_MENU(App_StartCamera, MyFrame::OnStartCamera)
EVT_MENU(App_CloseCamera, MyFrame::OnCloseCamera)
EVT_MENU(App_DetectFace, MyFrame::OnDetectFace)
EVT_MENU(App_MakeRobotFace, MyFrame::OnMakeRobotFace)
EVT_BUTTON(App_StartCamera, MyFrame::OnStartCamera)
EVT_BUTTON(App_CloseCamera, MyFrame::OnCloseCamera)
EVT_BUTTON(App_Snap, MyFrame::OnSnap)
EVT_BUTTON(App_MakeRobotFaceBtn, MyFrame::OnMakeRobotFaceBtn)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    wxInitAllImageHandlers();
    MyFrame* pMyFrame = new MyFrame("Webcam App");
    pMyFrame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title)
{
    // set the frame icon
    SetIcon(wxICON(sample));

    // create a menu bar
    wxMenu* fileMenu = new wxMenu;
    wxMenu* openCVMenu = new wxMenu;
    wxMenu* helpMenu = new wxMenu;

    helpMenu->Append(App_About, "&About\tF1", "Show about dialog");

    fileMenu->Append(App_Save, "&Save\tCtrl-S", "Save File");
    fileMenu->Append(App_Load, "&Load\tCtrl-L", "Load File");
    fileMenu->AppendSeparator();
    fileMenu->Append(App_Quit, "&Exit\tCtrl-Q", "Quit this program");

    openCVMenu->Append(App_StartCamera, "&Start Camera\tCtrl-T", "Start Camera");
    openCVMenu->Append(App_CloseCamera, "&Close Camera\tCtrl-R", "Close Camera");
    openCVMenu->AppendSeparator();
    openCVMenu->Append(App_DetectFace, "&Detect Face \tCtrl-J", "Detect Face");
    openCVMenu->Append(App_MakeRobotFace, "&Make Robot Face \tCtrl-I", "Make Robot Face");

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(openCVMenu, "&OpenCV");
    menuBar->Append(helpMenu, "&Help");
    SetMenuBar(menuBar);
    
    m_pCamera = new Camera(this);
    m_pImgProcessor = new Imageprocessor();
    setLayout();
}

void MyFrame::setLayout() {
    wxPanel* pLiveImageViewPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    pLiveImageViewPanel->SetBackgroundColour(wxColour(0, 0, 0));

    wxPanel* pSnapImageViewPanel = nullptr;
    //Check if there is a file
    bool exist = MyFrame::checkExistance("image.jpg");
    //If there is a file, add show image, else show empty panel
    if (exist) {
        pSnapImageViewPanel = new wxImagePanel(this, wxT("image.jpg"), wxBITMAP_TYPE_JPEG);
    }
    else {
        pSnapImageViewPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
        pLiveImageViewPanel->SetBackgroundColour(wxColour(128, 128, 128));
    }

    wxBoxSizer* pMainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* pImageSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* pButtonSizer = new wxBoxSizer(wxHORIZONTAL);

    pMainSizer->Add(pImageSizer, 10, wxEXPAND | wxALL, 5);
    pMainSizer->Add(pButtonSizer, 1, wxEXPAND | wxALL, 5);

    pImageSizer->Add(pLiveImageViewPanel, 1, wxEXPAND | wxALL, 5);
    pImageSizer->Add(pSnapImageViewPanel, 1, wxEXPAND | wxALL, 5);
    pLiveImageViewPanel->Hide();

    //Add Button layout
    m_pButtonStartCamera = new wxButton(this, App_StartCamera, _T("Start Camera"), wxDefaultPosition, wxDefaultSize, 0);
    m_pButtonCloseCamera = new wxButton(this, App_CloseCamera, _T("Close Camera"), wxDefaultPosition, wxDefaultSize, 0);
    m_pButtonSnap = new wxButton(this, App_Snap, _T("Snap!"), wxDefaultPosition, wxDefaultSize, 0);
    
    wxBitmap bitmap;
    bitmap.LoadFile("RobotBtn.bmp", wxBITMAP_TYPE_BMP);
    m_pButtonMakeRobot = new wxBitmapButton(this, App_MakeRobotFaceBtn, bitmap , wxDefaultPosition, wxDefaultSize,0);

    this -> refreshButtonLayout(m_bButtonFlag);

    pButtonSizer->Add(m_pButtonStartCamera, 1, wxEXPAND | wxALL, 5);
    pButtonSizer->Add(m_pButtonCloseCamera, 1, wxEXPAND | wxALL, 5);
    pButtonSizer->Add(m_pButtonSnap, 1, wxEXPAND | wxALL, 5);
    pButtonSizer->Add(m_pButtonMakeRobot, 1, wxEXPAND | wxALL, 5);
    this->SetSizerAndFit(pMainSizer);
    Layout();
}
 
//Define WxEvents for ImagePanel
BEGIN_EVENT_TABLE(wxImagePanel, wxPanel)
EVT_PAINT(wxImagePanel::paintEvent)
EVT_SIZE(wxImagePanel::OnSize)
END_EVENT_TABLE()

 wxImagePanel::wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format) : wxPanel(parent)
{
    // load the file... ideally add a check to see if loading was successful
    image.LoadFile(file, format);
    w = -1;
    h = -1;
}

void wxImagePanel::paintEvent(wxPaintEvent& evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void wxImagePanel::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

void wxImagePanel::render(wxDC& dc)
{
    int neww, newh;
    dc.GetSize(&neww, &newh);

    if (neww != w || newh != h)
    {
        resized = wxBitmap(image.Scale(neww, newh /*, wxIMAGE_QUALITY_HIGH*/));
        w = neww;
        h = newh;
        dc.DrawBitmap(resized, 0, 0, false);
    }
    else {
        dc.DrawBitmap(resized, 0, 0, false);
    }
}

void wxImagePanel::OnSize(wxSizeEvent& event) 
{
    Refresh();
    //skip the event.
    event.Skip();
}

//Frame event handlers

void MyFrame::OnClose(wxCloseEvent& event)
{
    if ( event.CanVeto() && !m_bFileSaved )
    {
        if ( wxMessageBox("The file has not been saved... continue closing?",
                          "Please confirm",
                          wxICON_QUESTION | wxYES_NO) != wxYES )
        {
            event.Veto();
            return;
        }
    }
    std::remove("image.jpg");
    Destroy();

}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    if ( !m_bFileSaved )
    {
        if ( wxMessageBox("The file has not been saved... continue closing?",
                          "Please confirm",
                          wxICON_QUESTION | wxYES_NO) == wxYES )
        {
            std::remove("image.jpg");
            Close(true);
        }
    }
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format
    (
        "Welcome to %s!\n"
        "\n"
        "This is my OpenCV Test App\n"
        "running under %s.",
        wxVERSION_STRING,
        wxGetOsDescription()
    ),
        "About my OpenCv Test App ",
        wxOK | wxICON_INFORMATION,
        this);
}

//Currently, this function only renames and saves the Image.jpg file. In future this file should no longer exist, the image should be past as pointer 
//between Camera and app.
void MyFrame::OnSave(wxCommandEvent& WXUNUSED(event))
{
    wxTextEntryDialog* nameDlg = new wxTextEntryDialog(this, wxString("Please name Your image"), wxString("Save As"));
    if ( nameDlg -> ShowModal() == wxID_OK )
    {
        wxString wxName =  nameDlg -> GetValue();
        std::string sName = std::string(wxName.mb_str()) + ".jpg";

        if (checkExistance("image.jpg")){
            std::ifstream input( "image.jpg", std::ios::binary );
            std::ofstream output(sName, std::ios::binary );
            std::copy(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>( ), std::ostreambuf_iterator<char>(output));
            m_bFileSaved = true;
        }
        else{
            wxMessageDialog* errorDlg = new wxMessageDialog(this, wxString("No file in Buffer!"), wxString("Error"), wxOK|wxCENTRE);
            errorDlg -> ShowModal();
        }
    }
 
}

void MyFrame::OnLoad(wxCommandEvent& WXUNUSED(event))
{
    wxTextEntryDialog* nameDlg = new wxTextEntryDialog(this, wxString("Please type the name of the file, you want to load"), wxString("Load"));
    if ( nameDlg -> ShowModal() == wxID_OK )
    {
        wxString wxName =  nameDlg -> GetValue();
        std::string sName = std::string(wxName.mb_str())+ ".jpg";
        if(checkExistance(sName)){
            std::ifstream input(sName, std::ios::binary );
            std::ofstream output("image.jpg", std::ios::binary);
            std::copy(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>( ), std::ostreambuf_iterator<char>(output));
            m_bFileSaved = false;
        }
        else{
            wxMessageDialog* errorDlg = new wxMessageDialog(this, wxString("File does not exist!"), wxString("Error"), wxOK|wxCENTRE);
            errorDlg -> ShowModal();
        }
        refreshLayout();
    }
}

void MyFrame::OnStartCamera(wxCommandEvent& WXUNUSED(event))
{
    if (!m_pCamera) {
        return;
    }

    if (!(m_pCamera->isCameraOn())) {
        m_pCamera->startCameraThread();
        //m_pCamera->openCamera();
    }
}

void MyFrame::OnCloseCamera(wxCommandEvent& WXUNUSED(event))
{
    if (!m_pCamera) {
        return;
    }

    if (m_pCamera->isCameraOn()) {
        m_pCamera->closeCamera();
    }
}

void MyFrame::OnDetectFace(wxCommandEvent& WXUNUSED(event))
{
    if (!m_pImgProcessor) {
        return;
    }

    wxTextEntryDialog* nameDlg = new wxTextEntryDialog(this, wxString("Please type the name of the file, you want to load"), wxString("Load"));

    if ( nameDlg -> ShowModal() == wxID_OK )
    {
        wxString wxFName =  nameDlg -> GetValue();
        std::string fileName= std::string(wxFName.mb_str()) + ".jpg";
        if (MyFrame::checkExistance(fileName)){
            m_pImgProcessor -> detectFace(fileName,true);
        }
        else{
            wxMessageDialog* errorDlg = new wxMessageDialog(this, wxString("File does not exist!"), wxString("Error"), wxOK|wxCENTRE);
            errorDlg -> ShowModal();
        }
    }
}

void MyFrame::OnMakeRobotFace(wxCommandEvent& WXUNUSED(event))
{
    if (!m_pImgProcessor) {
        return;
    }

    wxTextEntryDialog* nameDlg = new wxTextEntryDialog(this, wxString("Please type the name of the file, you want to load"), wxString("Load"));

    if ( nameDlg -> ShowModal() == wxID_OK )
    {
        wxString wxFName =  nameDlg -> GetValue();
        std::string fileName = std::string(wxFName.mb_str()) + ".jpg";
        if (MyFrame::checkExistance(fileName)){
            m_pImgProcessor -> makeRobotFace(fileName, true, false);
        }
        else{
            wxMessageDialog* errorDlg = new wxMessageDialog(this, wxString("File does not exist!"), wxString("Error"), wxOK|wxCENTRE);
            errorDlg -> ShowModal();
        }
    }
}

void MyFrame::OnMakeRobotFaceBtn(wxCommandEvent& WXUNUSED(event))
{
    if (!m_pCamera || !m_pImgProcessor) {
        return;
    }
    if (checkExistance("image.jpg")) {
        m_pImgProcessor->makeRobotFace("image.jpg", false, true);
        refreshLayout();
        m_bFileSaved = false;
    }
}

void MyFrame::OnSnap(wxCommandEvent& WXUNUSED(event))
{
    if (!m_pCamera) {
        return;
    }
    if (m_pCamera->isCameraOn()) {
        m_pCamera->snap(true);
        m_bFileSaved = false;
    }
}

void MyFrame::ImageReady()
{
    refreshLayout();
}

void MyFrame::refreshLayout()
{
    this->DestroyChildren();
    setLayout();
}

void MyFrame::setButtonLayout()
{
    m_bButtonFlag = (m_bButtonFlag == true) ? false : true;
    refreshButtonLayout(m_bButtonFlag);
}

void MyFrame::refreshButtonLayout(bool state)
{
    if (m_bButtonFlag) {
        m_pButtonStartCamera->SetBackgroundColour(wxColour(*wxLIGHT_GREY));
        m_pButtonCloseCamera->SetBackgroundColour(wxColour(*wxRED));
        m_pButtonSnap->SetBackgroundColour(wxColour(*wxGREEN));
        m_pButtonMakeRobot->SetBackgroundColour(wxColour(*wxGREEN));
    }
    else {
        m_pButtonStartCamera->SetBackgroundColour(wxColour(*wxGREEN));
        m_pButtonCloseCamera->SetBackgroundColour(wxColour(*wxLIGHT_GREY));
        m_pButtonSnap->SetBackgroundColour(wxColour(*wxLIGHT_GREY));
        m_pButtonMakeRobot->SetBackgroundColour(wxColour(*wxLIGHT_GREY));
    }
}

bool MyFrame::checkExistance(const std::string& name) 
{
    std::ifstream f(name.c_str());
    return f.good();
}
