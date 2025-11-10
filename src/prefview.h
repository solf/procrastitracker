
// Forward declarations for exclusion functions
extern void save_exclusions(const char *content);
extern char* read_exclusions_file();

INT_PTR CALLBACK Prefs(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
        case WM_INITDIALOG: {
            SendMessageA(hDlg, WM_SETICON, 0,
                         (LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_PROCRASTITRACKER)));
            loop(i, NUM_PREFS) prefs[i].seteditbox(hDlg);
            
            // Load exclusions from file into edit box
            char *buffer = read_exclusions_file();
            if (buffer) {
                SetDlgItemTextA(hDlg, IDC_EDIT_EXCLUSIONS, buffer);
                free(buffer);
            }
            
            return (INT_PTR)TRUE;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDOK: {
                    // Save exclusions from edit box to file
                    int len = GetWindowTextLengthA(GetDlgItem(hDlg, IDC_EDIT_EXCLUSIONS));
                    if (len > 0) {
                        char *buffer = (char *)malloc(len + 1);
                        if (buffer) {
                            GetDlgItemTextA(hDlg, IDC_EDIT_EXCLUSIONS, buffer, len + 1);
                            save_exclusions(buffer);
                            free(buffer);
                        }
                    } else {
                        // Empty text, save empty file
                        save_exclusions("");
                    }
                    EndDialog(hDlg, LOWORD(wParam));
                    return (INT_PTR)TRUE;
                }
                case IDCANCEL: {
                    EndDialog(hDlg, LOWORD(wParam));
                    return (INT_PTR)TRUE;
                }
                default: {
                    loop(i, NUM_PREFS) prefs[i].handleeditbox(LOWORD(wParam));
                    return (INT_PTR)TRUE;
                }
            }
            break;
        }
    }
    return (INT_PTR)FALSE;
}
