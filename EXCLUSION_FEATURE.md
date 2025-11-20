# Exclusion Tracking Feature

## Overview
The exclusion feature allows you to skip tracking specific applications by exe name or window title using regex patterns. Excluded items won't be tracked (no database entry) but will be logged to debug.log with the skip reason.

## Implementation Summary

### Files Modified
1. **src/procrastitracker.cpp**
   - Added regex, vector, and string headers
   - Added global `exclusionspath` variable and `ExclusionPattern` struct
   - Implemented `read_exclusions_file()` (reads raw file content)
   - Implemented `save_exclusions()` (saves content and reloads patterns)
   - Implemented `load_exclusions()` (parses patterns from raw content)
   - Implemented `is_excluded()` (checks if exe/title matches any pattern)
   - Initialize exclusionspath and load exclusions on startup

2. **src/timercallback.h**
   - Added exclusion check after extracting exe name and title
   - If excluded, logs with status="SKIPPED" and reason showing the matching pattern
   - Skips `addtodatabase()` call for excluded items

3. **src/prefview.h**
   - Added code to load exclusions from file into edit box on dialog open
   - Added code to save exclusions from edit box to file on OK button

4. **procrastitracker/resource.h**
   - Added control IDs: IDC_EDIT_EXCLUSIONS, IDC_STATIC_EXCLUSIONS, IDC_STATIC_EXCLUSIONS_HELP

5. **procrastitracker/procrastitracker.rc**
   - Expanded Advanced Settings dialog from 168 to 280 pixels in height
   - Added label, multiline edit box, help text, and OK/Cancel buttons

## Usage

### Accessing the Feature
1. Right-click on the ProcrastiTracker tray icon
2. Select "Advanced Settings..."
3. Scroll down to the "Exclusion Patterns" section at the bottom

### Pattern Format
- One regex pattern per line
- Prefix with `exe:` to match against executable name (e.g., `exe:pwsh`)
- Prefix with `title:` to match against window title (e.g., `title:.*password.*`)
- Lines without prefix default to exe matching
- Empty lines and lines starting with `#` are ignored (comments)

### Example Patterns

```
# Exclude PowerShell
exe:pwsh

# Exclude any window with "password" in the title (case-insensitive)
title:.*password.*

# Exclude notepad
exe:notepad

# Exclude any window title starting with "sqlf_pwdw"
title:^sqlf_pwdw.*

# Exclude Chrome (without exe: prefix, defaults to exe)
chrome

# Exclude any private/incognito browsing windows
title:.*(private|incognito).*
```

### Storage Location
Exclusions are stored in:
`%APPDATA%\procrastitrackerdbs\exclusions.txt`

This file is separate from the database, so:
- No database version incompatibilities
- Can be manually edited if needed
- Won't break existing installations

## Debug Logging
When debug.log exists, excluded items are logged with:
- Status: "SKIPPED"
- Reason: "excluded_exe:pattern" or "excluded_title:pattern"
- All other tracking info (exe name, window title, etc.)

Example log entry:
```
2025-11-10 14:30:45.123	SKIPPED	excluded_title:.*password.*		pwsh		Password Manager - Chrome	0	5	2	0	0	0	0	5
```

## Testing
To test the feature:
1. Create or touch the debug.log file to enable logging:
   - Path: `%APPDATA%\procrastitrackerdbs\debug.log`
2. Add exclusion patterns in Advanced Settings
3. Switch to excluded applications
4. Check debug.log to see SKIPPED entries
5. Verify excluded items don't appear in statistics

## Technical Details
- Uses C++11 `<regex>` with case-insensitive matching
- Invalid regex patterns are silently ignored (won't crash)
- Patterns are reloaded after saving in the dialog
- Memory is properly managed with RAII containers (std::vector, std::string)

