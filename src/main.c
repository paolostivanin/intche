#include <string.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <sqlite3.h>
#include "dupfinder.h"


gint
main (  gint argc,
        gchar **argv)
{
	if (argc > 1)
	{
		if ((g_strcmp0 (argv[1], "--version") == 0) || (g_strcmp0 (argv[1], "-v") == 0))
		{
			g_print ("%s v%s\nDeveloped by %s <%s>\n", PROGRAM_NAME, VERSION, DEVELOPER, DEV_EMAIL);
			return 0;
		}
		else
			return -1;
	}
	
    GDir *dir;
    GDir *dir1;
    GSList *list = NULL;
    GSList *list1 = NULL;
    GError *err = NULL;
    const gchar *homedir;
    guint i;
    gboolean excluded;

    homedir = g_get_home_dir ();
    dir = g_dir_open (homedir, 0, &err);
    if (err != NULL)
    {
        g_printerr ("%s\n", err->message);
        return -1;
    }
    
    gchar *excluded_folders_path = g_strconcat (homedir, EXCLUDED_RELATIVE_PATH, NULL);

    g_print ("[!] Checking directory [%s]\n", homedir);
    list = list_files_and_compute_chksum (homedir, dir);
    g_dir_close (dir);

    /* TODO:
     * - liste gestite in modo dinamico e non list, list1, etc XD
     */
    for (i = 0; i < g_slist_length (list); i++)
    {
		excluded = is_folder_excluded (excluded_folders_path, (gchar *)g_slist_nth_data(list, i));
		if (excluded)
		{
			excluded = FALSE;
			break;
		}
		g_print ("[!] Checking directory [%s]\n", (gchar *)g_slist_nth_data(list, i));
		dir1 = g_dir_open((gchar *)g_slist_nth_data(list, i), 0, &err);
		if (err != NULL)
		{
			g_printerr ("%s\n", err->message);
			return -1;
		}
		list1 = list_files_and_compute_chksum ((gchar *)g_slist_nth_data(list, i), dir1);
		g_free (g_slist_nth_data(list, i));
		g_slist_free(list1);
	}
	g_slist_free(list);
	g_free (excluded_folders_path);

    return 0;
}
