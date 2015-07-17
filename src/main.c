#include <string.h>
#include <glib.h>
#include <glib/gstdio.h>
#include "dupfinder.h"

gint
main (  gint argc,
        gchar **argv)
{
    GDir *dir;
    GError *err = NULL;
    gint type;
    goffset fsize;
    const gchar *fname;
    const gchar *homedir;
    gchar *cksum;

    homedir = g_get_home_dir ();

    dir = g_dir_open(homedir, 0, &err);
    if (err != NULL)
    {
        g_printerr ("%s\n", err->message);
        return -1;
    }

    while ((fname = g_dir_read_name (dir)))
    {
        if (strlen (fname) > 512)
        {
            g_printerr ("Seriously? A name longer than 512 chars?");
            return -1;
        }
        gchar *path = g_strconcat (homedir, "/", fname, NULL);        
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
                    //put into the 'next_dir' list
                    g_print ("DIR\t%s\n", fname);
            }
            else
                g_print ("SYMLINK\t%s\n", fname);
    
        }
        g_free (path);
    }

    g_dir_close (dir);

    return 0;
}
