#include <string.h>
#include <glib.h>
#include <stdlib.h>
#include <glib/gstdio.h>
#include <sqlite3.h>
#include "dupfinder.h"

gboolean is_folder_excluded (gchar *excluded_path, gchar *folder_path)
{
    gboolean err;
    GError *gerr = NULL;
    gchar *contents;
    gchar **lines = NULL;
    gint i;
    
    err = g_file_get_contents (excluded_path, &contents, NULL, &gerr);
    if (!err)
    {
        g_printerr ("%s\n", gerr->message);
        exit (-1);
    }
    
    lines = g_strsplit (contents, "\n", -1);
    for (i = 0; lines != NULL && lines[i] != NULL; i++)
    {
        if (g_strcmp0 (lines[i], folder_path) == 0)
            return TRUE;
    }
    
    g_free (contents);
    g_strfreev (lines);
    
    return FALSE;
}
