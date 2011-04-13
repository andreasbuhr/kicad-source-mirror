/**
 * @file kicad/menubar.cpp
 * @brief (Re)Create the project manager menubar for KiCad
 */
#include "fctsys.h"
#include "appl_wxstruct.h"
#include "common.h"
#include "kicad.h"
#include "macros.h"
#include "bitmaps.h"

/* Menubar and toolbar event table */
BEGIN_EVENT_TABLE( WinEDA_MainFrame, EDA_BASE_FRAME )
    /* Window events */
    EVT_SIZE( WinEDA_MainFrame::OnSize )
    EVT_CLOSE( WinEDA_MainFrame::OnCloseWindow )

    /* Sash drag events */
    EVT_SASH_DRAGGED( ID_LEFT_FRAME, WinEDA_MainFrame::OnSashDrag )

    /* Toolbar events */
    EVT_TOOL( ID_NEW_PROJECT, WinEDA_MainFrame::OnLoadProject )
    EVT_TOOL( ID_LOAD_PROJECT, WinEDA_MainFrame::OnLoadProject )
    EVT_TOOL( ID_SAVE_PROJECT, WinEDA_MainFrame::OnSaveProject )
    EVT_TOOL( ID_SAVE_AND_ZIP_FILES, WinEDA_MainFrame::OnArchiveFiles )

    /* Menu events */
    EVT_MENU( ID_SAVE_PROJECT, WinEDA_MainFrame::OnSaveProject )
    EVT_MENU( wxID_EXIT, WinEDA_MainFrame::OnExit )
    EVT_MENU( ID_TO_EDITOR, WinEDA_MainFrame::OnOpenTextEditor )
    EVT_MENU( ID_BROWSE_AN_SELECT_FILE, WinEDA_MainFrame::OnOpenFileInTextEditor )
    EVT_MENU( ID_SELECT_PREFERED_EDITOR, EDA_BASE_FRAME::OnSelectPreferredEditor )
    EVT_MENU( ID_SELECT_DEFAULT_PDF_BROWSER, WinEDA_MainFrame::OnSelectDefaultPdfBrowser )
    EVT_MENU( ID_SELECT_PREFERED_PDF_BROWSER, WinEDA_MainFrame::OnSelectPreferredPdfBrowser )
    EVT_MENU( ID_SELECT_PREFERED_PDF_BROWSER_NAME, WinEDA_MainFrame::OnSelectPreferredPdfBrowser )
    EVT_MENU( ID_SAVE_AND_ZIP_FILES, WinEDA_MainFrame::OnArchiveFiles )
    EVT_MENU( ID_READ_ZIP_ARCHIVE, WinEDA_MainFrame::OnUnarchiveFiles )
    EVT_MENU( ID_PROJECT_TREE_REFRESH, WinEDA_MainFrame::OnRefresh )
    EVT_MENU( ID_GENERAL_HELP, WinEDA_MainFrame::GetKicadHelp )
    EVT_MENU( wxID_ABOUT, WinEDA_MainFrame::GetKicadAbout )

    /* Range menu events */
    EVT_MENU_RANGE( ID_LANGUAGE_CHOICE, ID_LANGUAGE_CHOICE_END, WinEDA_MainFrame::SetLanguage )

    EVT_MENU_RANGE( wxID_FILE1, wxID_FILE9, WinEDA_MainFrame::OnFileHistory )

    /* Button events */
    EVT_BUTTON( ID_TO_PCB, WinEDA_MainFrame::OnRunPcbNew )
    EVT_BUTTON( ID_TO_CVPCB, WinEDA_MainFrame::OnRunCvpcb )
    EVT_BUTTON( ID_TO_EESCHEMA, WinEDA_MainFrame::OnRunEeschema )
    EVT_BUTTON( ID_TO_GERBVIEW, WinEDA_MainFrame::OnRunGerbview )
    EVT_BUTTON( ID_TO_BITMAP_CONVERTER, WinEDA_MainFrame::OnRunBitmapConverter )

    EVT_UPDATE_UI( ID_SELECT_DEFAULT_PDF_BROWSER, WinEDA_MainFrame::OnUpdateDefaultPdfBrowser )
    EVT_UPDATE_UI( ID_SELECT_PREFERED_PDF_BROWSER, WinEDA_MainFrame::OnUpdatePreferredPdfBrowser )

END_EVENT_TABLE()


/**
 * @brief (Re)Create the menubar
 */
void WinEDA_MainFrame::ReCreateMenuBar()
{
    // Create and try to get the current  menubar
    wxMenuItem* item;
    wxMenuBar*  menuBar = GetMenuBar();

    if( !menuBar )
        menuBar = new wxMenuBar();

    // Delete all existing menus so they can be rebuilt.
    // This allows language changes of the menu text on the fly.
    menuBar->Freeze();
    while( menuBar->GetMenuCount() )
        delete menuBar->Remove( 0 );

    // Recreate all menus:

    // Menu File:
    wxMenu* fileMenu = new wxMenu;

    // Open
    ADD_MENUITEM_WITH_HELP( fileMenu,
                            ID_LOAD_PROJECT,
                            _( "&Open\tCtrl+O" ),
                            _( "Open an existing project" ),
                            open_project_xpm );

    // Open Recent submenu
    wxMenu* openRecentMenu = new wxMenu();
    wxGetApp().m_fileHistory.AddFilesToMenu( openRecentMenu );
    ADD_MENUITEM_WITH_HELP_AND_SUBMENU( fileMenu, openRecentMenu,
                                        wxID_ANY,
                                        _( "Open &Recent" ),
                                        _( "Open a recent opened schematic project" ),
                                        open_project_xpm );

    // New
    ADD_MENUITEM_WITH_HELP( fileMenu, ID_NEW_PROJECT,
                            _( "&New\tCtrl+N" ),
                            _( "Start a new project" ),
                            new_project_xpm );

    // Save
    ADD_MENUITEM_WITH_HELP( fileMenu,
                            ID_SAVE_PROJECT,
                            _( "&Save\tCtrl+S" ),
                            _( "Save current project" ),
                            save_project_xpm );

    // Archive
    fileMenu->AppendSeparator();
    ADD_MENUITEM_WITH_HELP( fileMenu,
                            ID_SAVE_AND_ZIP_FILES,
                            _( "&Archive" ),
                            _( "Archive project files in zip archive" ),
                            zip_xpm );

    // Unarchive
    ADD_MENUITEM_WITH_HELP( fileMenu,
                            ID_READ_ZIP_ARCHIVE,
                            _( "&Unarchive" ),
                            _( "Unarchive project files from zip file" ),
                            unzip_xpm );

    // Separator
    fileMenu->AppendSeparator();

    // Quit
    ADD_MENUITEM_WITH_HELP( fileMenu,
                            wxID_EXIT,
                            _( "&Quit" ),
                            _( "Quit KiCad" ),
                            exit_xpm );

    // Menu Browse:
    wxMenu* browseMenu = new wxMenu();

    // Text editor
    ADD_MENUITEM_WITH_HELP( browseMenu,
                            ID_TO_EDITOR,
                            _( "Text E&ditor" ),
                            _( "Launch preferred text editor" ),
                            editor_xpm );

    // View file
    ADD_MENUITEM_WITH_HELP( browseMenu,
                            ID_BROWSE_AN_SELECT_FILE,
                            _( "&View File" ),
                            _( "View, read or edit file with a text editor" ),
                            browse_files_xpm );

    // Menu Preferences:
    wxMenu* preferencesMenu = new wxMenu;

    // Text editor
    ADD_MENUITEM_WITH_HELP( preferencesMenu,
                            ID_SELECT_PREFERED_EDITOR,
                            _( "&Text Editor" ),
                            _( "Select your preferred text editor" ),
                            editor_xpm );

    // PDF Viewer submenu:System browser or user defined checkbox
    wxMenu* SubMenuPdfBrowserChoice = new wxMenu;

    // Default
    item = new wxMenuItem( SubMenuPdfBrowserChoice,
                           ID_SELECT_DEFAULT_PDF_BROWSER,
                           _( "Default" ),
                           _( "Use system default PDF viewer used to browse datasheets" ),
                           wxITEM_CHECK );

    SETBITMAPS( datasheet_xpm );

    SubMenuPdfBrowserChoice->Append( item );
    SubMenuPdfBrowserChoice->Check( ID_SELECT_DEFAULT_PDF_BROWSER,
                                    wxGetApp().m_PdfBrowserIsDefault );

    // Favourite
    item = new wxMenuItem( SubMenuPdfBrowserChoice,
                           ID_SELECT_PREFERED_PDF_BROWSER,
                           _( "Favourite" ),
                           _( "Use your favourite PDF viewer used to browse datasheets" ),
                           wxITEM_CHECK );

    SETBITMAPS( preference_xpm );

    SubMenuPdfBrowserChoice->Append( item );
    SubMenuPdfBrowserChoice->AppendSeparator();
    SubMenuPdfBrowserChoice->Check( ID_SELECT_PREFERED_PDF_BROWSER,
                                    !wxGetApp().m_PdfBrowserIsDefault );

    // Append PDF Viewer submenu to preferences
    ADD_MENUITEM_WITH_HELP( SubMenuPdfBrowserChoice,
                            ID_SELECT_PREFERED_PDF_BROWSER_NAME,
                            _( "PDF Viewer" ),
                            _( "Select your favourite PDF viewer used to browse datasheets" ),
                            datasheet_xpm );

    // PDF viewer submenu
    ADD_MENUITEM_WITH_HELP_AND_SUBMENU( preferencesMenu,
                                        SubMenuPdfBrowserChoice, -1,
                                        _( "PDF Viewer" ),
                                        _( "PDF viewer preferences" ),
                                        datasheet_xpm );

    // Language submenu
    preferencesMenu->AppendSeparator();
    wxGetApp().AddMenuLanguageList( preferencesMenu );

    // Menu Help:
    wxMenu* helpMenu = new wxMenu;

    // Version info
    AddHelpVersionInfoMenuEntry( helpMenu );

    // Contents
    ADD_MENUITEM_WITH_HELP( helpMenu,
                            ID_GENERAL_HELP,
                            _( "&Contents" ),
                            _( "Open the kicad manual" ),
                            online_help_xpm );

    // Separator
    helpMenu->AppendSeparator();

    // About
    ADD_MENUITEM_WITH_HELP( helpMenu,
                            wxID_ABOUT,
                            _( "&About KiCad" ),
                            _( "About kicad project manager" ),
                            info_xpm );

    // Create the menubar and append all submenus
    menuBar->Append( fileMenu, _( "&File" ) );
    menuBar->Append( browseMenu, _( "&Browse" ) );
    menuBar->Append( preferencesMenu, _( "&Preferences" ) );
    menuBar->Append( helpMenu, _( "&Help" ) );

    menuBar->Thaw();

    // Associate the menu bar with the frame, if no previous menubar
    if( GetMenuBar() == NULL )
        SetMenuBar( menuBar );
    else
        menuBar->Refresh();
}


/**
 * @brief (Re)Create the horizontal toolbar
 */
void WinEDA_MainFrame::RecreateBaseHToolbar()
{
    // Check if toolbar is not already created
    if( m_HToolBar != NULL )
        return;

    // Allocate memory for m_HToolBar
    m_HToolBar = new EDA_TOOLBAR( TOOLBAR_MAIN, this, ID_H_TOOLBAR, TRUE );

    // New
    m_HToolBar->AddTool( ID_NEW_PROJECT, wxEmptyString,
                         wxBitmap( new_project_xpm ),
                         _( "Start a new project" ) );

    // Load
    m_HToolBar->AddTool( ID_LOAD_PROJECT, wxEmptyString,
                         wxBitmap( open_project_xpm ),
                         _( "Load existing project" ) );

    // Save
    m_HToolBar->AddTool( ID_SAVE_PROJECT, wxEmptyString,
                         wxBitmap( save_project_xpm ),
                         _( "Save current project" ) );

    // Separator
    m_HToolBar->AddSeparator();

    // Archive
    m_HToolBar->AddTool( ID_SAVE_AND_ZIP_FILES, wxEmptyString,
                         wxBitmap( zip_xpm ),
                         _( "Archive all project files" ) );

    // Separator
    m_HToolBar->AddSeparator();

    // Refresh project tree
    m_HToolBar->AddTool( ID_PROJECT_TREE_REFRESH, wxEmptyString,
                         wxBitmap( reload_xpm ),
                         _( "Refresh project tree" ) );

    // Create m_HToolBar
    m_HToolBar->Realize();
}
