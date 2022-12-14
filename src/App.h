#ifndef APP_H_
#define APP_H_

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/textdlg.h>
#include "Camera.h"
#include "Imageprocessor.h"

//Forward Declaration
class Camera;

class MyApp : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;

private:
  
};

class wxImagePanel : public wxPanel
{
    wxImage image;
    wxBitmap resized;
    int w, h;

public:
    wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format);

private:
    void paintEvent(wxPaintEvent& evt);
    void paintNow();
    void OnSize(wxSizeEvent& event);
    void render(wxDC& dc);

    DECLARE_EVENT_TABLE()
};


class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title);
    void ImageReady();
    void setButtonLayout();
    static bool checkExistance(const std::string& name);

private: 
    void close(); 
    void setLayout();
    void refreshLayout();
    void refreshButtonLayout(bool state);

    Camera* m_pCamera{ nullptr };
    Imageprocessor* m_pImgProcessor{ nullptr };
    bool m_bButtonFlag{ false };
    bool m_bFileSaved{ false };
  
    wxImagePanel* m_pSnapImageViewPanel{ nullptr };
    wxButton* m_pButtonStartCamera{ nullptr };
    wxButton* m_pButtonCloseCamera{ nullptr };
    wxButton* m_pButtonSnap{ nullptr };
    wxBitmapButton* m_pButtonMakeRobot{ nullptr };

    //Events Handling
    void OnClose(wxCloseEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnLoad(wxCommandEvent& event);
    void OnStartCamera(wxCommandEvent& event);
    void OnCloseCamera(wxCommandEvent& event);
    void OnDetectFace(wxCommandEvent& event);
    void OnMakeRobotFace(wxCommandEvent& event);
    void OnMakeRobotFaceBtn(wxCommandEvent& event);
    void OnSnap(wxCommandEvent& event);
    
    DECLARE_EVENT_TABLE()
};

enum
{
    App_Quit = wxID_EXIT,
    App_About = wxID_ABOUT,
    App_Save = 1,
    App_Load,
    App_StartCamera = 10,
    App_CloseCamera,
    App_DetectFace,
    App_MakeRobotFace,
    App_Snap,
    App_MakeRobotFaceBtn
};

#endif /* APP_H_ */