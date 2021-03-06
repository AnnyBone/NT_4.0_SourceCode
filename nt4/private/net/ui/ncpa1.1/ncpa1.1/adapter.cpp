//----------------------------------------------------------------------------
//
//  File: Adapater.cpp
//
//  Contents: This file contains the PropertyPage for the Adapter config
//
//  Notes:
//
//  History:
//      April 21, 1995  MikeMi - Created
// 
//
//----------------------------------------------------------------------------

#include "pch.hxx"
#pragma hdrstop

static const MAX_TEMP = 1023;

//-------------------------------------------------------------------
//
//  Function: OnDialogInit
//
//  Synopsis: initialization of the dialog
//
//  Arguments:
//		hwndDlg [in]	- handle of Dialog window 
//
//  Return;
//		TRUE - let Windows assign focus to a control
//      FALSE - we want to set the focus
//
//  Notes:
//
//  History:
//      April 28, 1995 MikeMi - Created
//
//
//-------------------------------------------------------------------

static BOOL OnDialogInit( HWND hwndDlg, NCP* pncp )
{
    HWND hwndListView;
    hwndListView = GetDlgItem( hwndDlg, IDC_LISTVIEW );
    RECT rc;
    LV_COLUMN lvc;

    // prepare listview
    ListView_SetImageList( hwndListView, g_hil, LVSIL_SMALL );
                      
    GetClientRect( hwndListView, &rc );
    lvc.mask = LVCF_FMT | LVCF_WIDTH;
    lvc.fmt = LVCFMT_LEFT;
    lvc.cx = rc.right - GetSystemMetrics( SM_CXVSCROLL ) ;
    ListView_InsertColumn(hwndListView, 0, &lvc);

    ListViewRefresh( hwndDlg, hwndListView, pncp->QueryAdapterList(), ILI_NETCARD );

    if (!pncp->CanModify())
    {
        EnableWindow( GetDlgItem( hwndDlg, IDC_ADD ), FALSE );
        EnableWindow( GetDlgItem( hwndDlg, IDC_REMOVE ), FALSE );
        EnableWindow( GetDlgItem( hwndDlg, IDC_PROPERTIES ), FALSE );
        EnableWindow( GetDlgItem( hwndDlg, IDC_UPDATE ), FALSE );
		SendMessage( 
				GetDlgItem( hwndDlg, IDC_DESCRIPTION ), 
				EM_SETREADONLY, 
				(WPARAM)TRUE, 
				0 );
    }

    SetFocus( hwndListView );

    return( FALSE ); // we want to set focus
}



//-------------------------------------------------------------------
//
//  Function: OnItemChanged
//
//  Synopsis: Handle the notification that a listview item had changed
//
//  Arguments:
//		hwndDlg [in]	- handle of Dialog window 
//      hwndLV [in]     - handle of the ListView window
//      pnmlv [in]      - notification structure
//      pncp [in]   - the binery object
//
//  Return;
//      True - Handled this message
//      False - not handled
//
//  Notes:
//
//  History:
//      April 28, 1995 MikeMi - Created
//
//
//-------------------------------------------------------------------

static BOOL OnItemChanged( HWND hwndDlg, 
        HWND hwndLV, 
        NM_LISTVIEW* pnmlv,
        NCP* pncp )
{
    BOOL frt = FALSE;

    // only interested in state change
    if (pnmlv->uChanged & LVIF_STATE)
    {
        // only interested in new focus set
        if ( (LVIS_SELECTED == (pnmlv->uNewState & LVIS_SELECTED)) &&
                (0 == (pnmlv->uOldState & LVIS_SELECTED)) )
        {
            COMPONENT_DLIST* pcdl = pncp->QueryAdapterList();
            REG_KEY* prkSel;
            LPTSTR pszDesc;

            frt = TRUE;
            // update the decription text (lParam contains the list index)
            prkSel = pcdl->QueryNthItem( pnmlv->lParam );
            pncp->QueryValueString( prkSel,
                    RGAS_COMPONENT_DESC,
                    &pszDesc ) ;
            SetDlgItemText( hwndDlg, IDC_DESCRIPTION, pszDesc );
            delete []pszDesc;

            // allow buttons to be used
            if (pncp->CanModify())
            {
                DWORD dwOpFlags;

                EnableWindow( GetDlgItem( hwndDlg, IDC_DESCRIPTIONSTATIC ), TRUE );
                EnableWindow( GetDlgItem( hwndDlg, IDC_DESCRIPTION ), TRUE );


                if (ERROR_SUCCESS != prkSel->QueryValue( RGAS_SOFTWARE_OPSUPPORT, 
                        &dwOpFlags ))
                {
                    dwOpFlags = NCOS_UNSUPPORTED;
                }

                if (dwOpFlags & NCOS_REMOVE)
                {
                    EnableWindow( GetDlgItem( hwndDlg, IDC_REMOVE ), TRUE );
                }
                if (dwOpFlags & NCOS_PROPERTIES)
                {
                    EnableWindow( GetDlgItem( hwndDlg, IDC_PROPERTIES ), TRUE );
                }
                if (dwOpFlags & NCOS_UPDATE)
                {
                    EnableWindow( GetDlgItem( hwndDlg, IDC_UPDATE ), TRUE );
                }
            }
        }
        else if ( (0 == (pnmlv->uNewState & LVIS_SELECTED)) &&
                ( LVIS_SELECTED == (pnmlv->uOldState & LVIS_SELECTED)) )
        {
            // since the always select doesn't always have a selection
            // no selecion, no buttons
            SetDlgItemText( hwndDlg, IDC_DESCRIPTION, TEXT("") );
            EnableWindow( GetDlgItem( hwndDlg, IDC_DESCRIPTIONSTATIC ), FALSE );
            EnableWindow( GetDlgItem( hwndDlg, IDC_DESCRIPTION ), FALSE );
            EnableWindow( GetDlgItem( hwndDlg, IDC_REMOVE ), FALSE );
            EnableWindow( GetDlgItem( hwndDlg, IDC_PROPERTIES ), FALSE );
            EnableWindow( GetDlgItem( hwndDlg, IDC_UPDATE ), FALSE );

        }

    }
    return( frt );
}

//-------------------------------------------------------------------
//
//  Function: OnSaveDescription
//
//  Synopsis: 
//
//  Arguments:
//		hwndDlg [in]	- handle of Dialog window 
//      pncp [in]   - the binery object
//
//  Return;
//      True - Handled this message
//      False - not handled
//
//  Notes:
//
//  History:
//      August 25, 1995 MikeMi - Created
//
//
//-------------------------------------------------------------------

const INT MAX_DESCRIPTION_LEN = 1024;

static BOOL OnSaveDescription( HWND hwndDlg, NCP* pncp, INT iItem )
{
    BOOL frt = TRUE;
    COMPONENT_DLIST* pcdl = pncp->QueryAdapterList();
    REG_KEY* prkSel;
    TCHAR pszDesc[MAX_DESCRIPTION_LEN+1];

    if (-1 != iItem)
    {
        GetDlgItemText( hwndDlg, IDC_DESCRIPTION, pszDesc, MAX_DESCRIPTION_LEN );

        // update the decription text
        prkSel = pcdl->QueryNthItem( iItem );
        pncp->SetValueString( prkSel,
                RGAS_COMPONENT_DESC,
                pszDesc ) ;
        
    }
    return( frt );
}

static BOOL OnChangeDescription( HWND hwndDlg, NCP* pncp, INT& iItem )
{
    HWND hwndLV = GetDlgItem( hwndDlg, IDC_LISTVIEW );
    LV_ITEM lvi;
    BOOL frt = FALSE;
    iItem = -1;

    // get selected items, lparam (the list index)
    lvi.iItem = ListView_GetNextItem( hwndLV, -1, LVNI_SELECTED );

    if (lvi.iItem >= 0)
    {
        lvi.iSubItem = 0;
        lvi.mask = LVIF_PARAM;

        ListView_GetItem( hwndLV, &lvi );

        iItem = lvi.lParam;
        PropSheet_CancelToClose( GetParent( hwndDlg ) );
        frt = TRUE;
    }
    return( frt );
}


//-------------------------------------------------------------------
//
//  Function: dlgProcNetCard
//
//  Synopsis: the dialog proc ofr the netcard propertysheet
//
//  Arguments:
//		hwndDlg [in]	- handle of Dialog window 
//		uMsg [in]		- message                       
// 		lParam1 [in]    - first message parameter
//		lParam2 [in]    - second message parameter       
//
//  Return;
//		message dependant
//
//  Notes:
//
//  History:
//      April 27, 1995 MikeMi - Created
//
//
//-------------------------------------------------------------------

BOOL CALLBACK dlgprocNetcard( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    BOOL frt = FALSE;
    static NCP* pncp;
    static INT crefHourGlass = 0;
    static BOOL fDescriptionChanged = FALSE;
    static INT iItem = -1;

    switch (uMsg)
    {
    case WM_INITDIALOG:
        {
            LPPROPSHEETPAGE ppsp;
            ppsp = (LPPROPSHEETPAGE) lParam;
            pncp = (NCP*)ppsp->lParam;
        }
        frt = OnDialogInit( hwndDlg, pncp );
        break;

    case WM_COMMAND:
        switch (HIWORD(wParam))
        {
		case EN_KILLFOCUS:
			switch (LOWORD(wParam))
			{
            case IDC_DESCRIPTION:
                if (fDescriptionChanged)
                {
                    frt = OnSaveDescription( hwndDlg, pncp, iItem );
                    fDescriptionChanged = FALSE;
                    iItem = -1;
                }
                break;
            default:
                frt = FALSE;
                break;
            }
			break;

        case EN_CHANGE:
            switch (LOWORD(wParam))
            {
            case IDC_DESCRIPTION:
                fDescriptionChanged = TRUE;
                frt = OnChangeDescription( hwndDlg, pncp, iItem );
                break;
            default:
                frt = FALSE;
                break;
            }
            break;

        case BN_CLICKED:

            switch (LOWORD(wParam))
            {
            case IDC_ADD:
                frt =   OnAdd(hwndDlg, 
                    ADAPTER, 
                    pncp->QueryAdapterList(), 
                    pncp);
                break;

            case IDC_REMOVE:
                frt = OnConfigure( hwndDlg,
                        pncp->QueryAdapterList(),
                        pncp,  
                        NCFG_REMOVE );
                break;

            case IDC_PROPERTIES:
                frt = OnConfigure( hwndDlg, 
                        pncp->QueryAdapterList(),
                        pncp,  
                        NCFG_CONFIGURE );
                break;

            case IDC_UPDATE:
                frt = OnConfigure( hwndDlg, 
                        pncp->QueryAdapterList(),
                        pncp,  
                        NCFG_UPDATE );
                break;
            default:
                frt = FALSE;
                break;
            }
            break;

        default:
            frt = FALSE;
            break;
        }
        break;

    case WM_NOTIFY:
        {
            LPNMHDR pnmh = (LPNMHDR)lParam;

            switch (pnmh->code)
            {
            // propsheet notification
            case PSN_HELP:
                break;

            case PSN_SETACTIVE:
                HandleBindingDeactivate( hwndDlg, pncp );
                // SetWindowLong( hwndDlg, DWL_MSGRESULT, 0 );
                frt = TRUE;
                break;

            case PSN_KILLACTIVE:
                // ok to loose being active
                SetWindowLong( hwndDlg, DWL_MSGRESULT, FALSE );
                frt = TRUE;
                break;

            // list view notification
            case LVN_ITEMCHANGED:
                if (fDescriptionChanged)
                {
                    frt = OnSaveDescription( hwndDlg, pncp, iItem );
                    fDescriptionChanged = FALSE;
                    iItem = -1;
                }
                frt = OnItemChanged( hwndDlg, 
                        pnmh->hwndFrom, 
                        (NM_LISTVIEW*)lParam,
                        pncp );
                break;

            case NM_DBLCLK:
                if (pncp->CanModify())
                {
                    frt = OnConfigure( hwndDlg, 
                            pncp->QueryAdapterList(),
                            pncp,  
                            NCFG_CONFIGURE );
                }
                break;

            default:
                frt = FALSE;
                break;
            }
        }
        break;    

    case PWM_REFRESHLIST:
        ListViewRefresh( hwndDlg, GetDlgItem( hwndDlg, IDC_LISTVIEW ), 
                        pncp->QueryAdapterList(),
                        ILI_NETCARD );
        break;

    case WM_CONTEXTMENU:
        frt = OnComponentContextMenu( hwndDlg, 
                (HWND)wParam, 
                LOWORD( lParam ), 
                HIWORD( lParam ), 
                pncp, 
                pncp->QueryAdapterList(),
                amhidsAdapter );
        break;

    case WM_HELP:
        {
            LPHELPINFO lphi;

            lphi = (LPHELPINFO)lParam;
            if (lphi->iContextType == HELPINFO_WINDOW)   // must be for a control
            {
                WinHelp( (HWND)lphi->hItemHandle, 
                        PSZ_NETWORKHELP, 
                        HELP_WM_HELP, 
                        (DWORD)(LPVOID)amhidsAdapter );
            }
        }
        break;

    case PWM_CURSORWAIT:
        frt = HandleCursorWait( hwndDlg, (BOOL)lParam, crefHourGlass );
        break;

    case WM_SETCURSOR:
        frt = HandleSetCursor( hwndDlg, LOWORD(lParam), crefHourGlass );
        break;

    case WM_QUERYENDSESSION:
        OnQueryEndSession( hwndDlg, pncp );
        frt = TRUE;
        break;

    case PWM_WARNNOENDSESSION:
        OnWarnEndSession( hwndDlg, pncp );
        frt = TRUE;
        break;

    case WM_ENDSESSION:
        OnEndSession( hwndDlg, wParam, pncp );
        frt = TRUE;
        break;

    default:
        frt = FALSE;
        break;
    }

    return( frt );
}

//-------------------------------------------------------------------
//
//  Function: GetNcpNetcardHPage
//
//  Synopsis: This will create a handle to property sheet for the netcard
//      page.
//
//  Arguments:
//
//  Returns:
//      a handle to a newly created propertysheet; NULL if error
//
//  Notes:
//
//  History:
//      April 27, 1995 MikeMi - Created
//
//
//-------------------------------------------------------------------

HPROPSHEETPAGE GetNcpNetcardHPage( const NCP& ncp )
{
    HPROPSHEETPAGE hpsp;
    PROPSHEETPAGE psp;

    psp.dwSize = sizeof( PROPSHEETPAGE );
    psp.dwFlags = 0;
    psp.hInstance = g_hinst;
    psp.pszTemplate = MAKEINTRESOURCE( IDD_NETCARD );
    psp.hIcon = NULL;
    psp.pfnDlgProc = dlgprocNetcard;
    psp.lParam = (LONG)&ncp;

    hpsp = CreatePropertySheetPage( &psp );
    return( hpsp );
}
