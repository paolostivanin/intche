#include <string.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <sqlite3.h>
#include "dupfinder.h"

GSList
*list_files_and_compute_chksum (const gchar *fd_path, GDir *dir)
{
    GSList *list = NULL;
    gchar *cksum;
    gint type;
    goffset fsize;
    const gchar *fname;
    
    while ((fname = g_dir_read_name (dir)))
    {
        if (strlen (fname) > 512)
        {
            g_printerr ("Seriously? A name longer than 512 chars?");
            return NULL;
        }
        gchar *path = g_strconcat (fd_path, "/", fname, NULL);        
        type = is_dir (path);
        if (type == 1)
        {
            fsize = get_file_size (path);
            if (fsize > 0)
            {
                cksum = compute_crc32 (path, fsize);
                //insert into db
                g_print ("FILE\t%s - %s\n", fname, cksum);
                g_free (cksum);
            }
        }
        else
        {
            if (type == 0)
            {
                if (*fname == '.')
                    continue;
                else
                    //the the dir path into the list
                    list = g_slist_append (list, g_strdup (path));
            }
            else
                //file is symlink, skip it.
                continue;

        }
        g_free (path);
    }
    return list;
}
