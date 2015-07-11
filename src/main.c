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
    const gchar *fname;
    const gchar *homedir;

    homedir = g_get_home_dir ();

    /* - for obj in dir:
     *  -- if obj is dir -> save_for_later
     *  -- else crc32, size and store inside db
     * - for dir in save_for_later:
     *   -- if dir is dir: -> save_for_later
     *   -- else crc32, size and store inside db
     */
    dir = g_dir_open(homedir, 0, &err);
    if (err != NULL)
        g_printerr ("%s\n", err->message);

    while ((fname = g_dir_read_name (dir)))
    {
        gchar *path = g_strconcat (homedir, "/", fname, NULL);        
        type = is_dir (path);
        if (type == 1)
            g_print ("FILE\t%s\n", fname);
        else
        {
            if (type == 0)
                g_print ("DIR\t%s\n", fname);
            else
                g_print ("SYMLINK\t%s\n", fname);
    
        }
        g_free (path);
    }

    g_dir_close (dir);

    return 0;
}
