# Debug Logging Feature

## Overview

The debug logging feature helps diagnose time tracking issues by recording detailed information about each timer tick. The logging is controlled by the presence of a `debug.log` file in the database directory and has minimal performance impact when disabled.

## How to Enable Logging

1. Navigate to your ProcrastiTracker database directory:
   - Typically: `%APPDATA%\procrastitrackerdbs\`
   - Or: `C:\Users\YourUsername\AppData\Roaming\procrastitrackerdbs\`

2. Create an empty file named `debug.log` in that directory:
   ```
   cd %APPDATA%\procrastitrackerdbs
   type nul > debug.log
   ```

3. Logging starts immediately on the next timer tick (default: every 5 seconds)

## How to Disable Logging

Simply delete or rename the `debug.log` file:
```
del %APPDATA%\procrastitrackerdbs\debug.log
```

Logging stops on the next timer tick.

## Log Format

The log file is in tab-separated values (TSV) format, suitable for importing into Excel, LibreOffice Calc, or other spreadsheet applications.

### Columns (in order):

1. **Timestamp** - Date and time with milliseconds (YYYY-MM-DD HH:MM:SS.mmm)
2. **Status** - One of:
   - `TRACKED` - Time was successfully attributed to an application
   - `IDLE` - User was idle (no input detected)
   - `SKIPPED` - Tracking was skipped for some reason
3. **Reason** - Empty if tracked, otherwise indicates why:
   - `idle` - Exceeded idle threshold
   - `no_window` - No foreground window detected
   - `open_process_failed` - Could not open process handle
   - `lockapp` - Windows lock screen was active
4. **Attributed_String** - The full string sent to the database (e.g., "chrome - google.com - Google - Chrome")
5. **Exe_Name** - Process executable name (e.g., "chrome", "notepad")
6. **URL** - Website URL for browsers (empty for non-browser apps)
7. **Window_Title** - Window title text
8. **Idle_Seconds** - Seconds since last keyboard/mouse activity
9. **Keys** - Number of key presses during this interval
10. **Mouse_Left** - Number of left mouse button clicks
11. **Mouse_Right** - Number of right mouse button clicks
12. **Scroll** - Number of mouse scroll wheel movements
13. **Fullscreen** - 1 if foreground window is fullscreen, 0 otherwise
14. **Controller** - 1 if game controller is connected, 0 otherwise
15. **Sample_Interval** - Timer interval in seconds (usually 5)

### Example Log Lines:

```
Timestamp	Status	Reason	Attributed_String	Exe_Name	URL	Window_Title	Idle_Seconds	Keys	Mouse_Left	Mouse_Right	Scroll	Fullscreen	Controller	Sample_Interval
2025-11-10 14:32:15.123	TRACKED		chrome - google.com - Google - Chrome	chrome	google.com	Google - Chrome	2	15	3	0	2	0	0	5
2025-11-10 14:32:20.125	IDLE	idle					185	0	0	0	0	0	0	5
2025-11-10 14:32:25.127	SKIPPED	lockapp	lockapp	lockapp				0	0	0	0	0	0	0	5
```

## Analyzing the Logs

### In Excel/LibreOffice Calc:

1. Open the log file or use "Import Data"
2. Select "Tab" as the delimiter
3. Use Excel features to analyze:
   - Filter by Status to find IDLE or SKIPPED entries
   - Sort by Attributed_String to see time distribution
   - Create pivot tables to sum time by application
   - Calculate time gaps between timestamps to detect missed ticks

### Finding Issues:

**Undercounting Problems:**
- Look for many `IDLE` entries when you were active
- Check `Idle_Seconds` values - should be low during active use
- Compare `Keys` and mouse click counts with your actual activity
- Look for time gaps in timestamps (missing ticks)

**Specific Issues:**
- Filter `Status` column for `SKIPPED` to find problematic ticks
- Check `Reason` column to understand why tracking failed
- Empty `Attributed_String` indicates nothing was tracked

**Activity Verification:**
- Sum `Keys`, `Mouse_Left`, `Mouse_Right` columns to verify input detection
- Check if `Fullscreen` and `Controller` flags match your usage
- Verify `URL` extraction works for your browsers

## Performance Impact

- **When Disabled** (no debug.log file): Single file existence check per tick (~microseconds)
- **When Enabled**: File open, write one line, close per tick (~milliseconds)
- The file is NOT kept open, allowing you to rename/delete it anytime
- Special characters (tabs, newlines) in window titles are escaped automatically

## Troubleshooting

**Log file not being created:**
- Ensure you created it in the correct directory (`%APPDATA%\procrastitrackerdbs\`)
- Check that ProcrastiTracker has write permissions to that directory

**No log entries appearing:**
- Check if the timer is actually running (look at system tray icon)
- Verify the sample interval in Advanced Settings

**Log file grows too large:**
- You can safely delete or archive it anytime
- Expected size: ~200-300 bytes per tick
- At 5-second intervals: ~1.7 MB per day

## Example Analysis Queries

### Find when you were marked idle but were actually active:
Filter: `Status = IDLE` AND `Keys > 0`

### Find applications that were skipped:
Filter: `Status = SKIPPED`

### Calculate actual tracked time:
1. Filter: `Status = TRACKED`
2. Count rows × Sample_Interval (usually 5 seconds)

### Find gaps in tracking:
1. Calculate time difference between consecutive timestamps
2. Look for gaps > Sample_Interval + 1 second

